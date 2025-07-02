#include "GameRole.h"
#include<iostream>
#include<unistd.h>
#include<random>
#include"ZinxTimer.h"
#include"RandomName.h"
#include<fstream>
#include<hiredis/hiredis.h>
//使用AOI类画一个矩形世界，所有玩家都在这个世界中，所以需要全局静态变量来存储
static AOIWorld world(0, 1000, 0, 1000, 20, 20);
static std::default_random_engine random_engine(time(NULL));
RandomName random_name;

//创建一个定时处理对象，规定定时时间
class ExitTimer :public TimerOutProc
{
    // 通过 TimerOutProc 继承
    void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    int GetTimerSec() override
    {
        return 20;
    }
};
//创建一个全局对象，在全局都有效
static ExitTimer get_exittimer;

//发送ID和名称
GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid *pid =new  pb::SyncPid();
    pid->set_username(sZname);
    pid->set_pid(iPid);
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_LOGIN_ID_NAME, pid);
    return pmsg;
}
//向当前玩家发送周围玩家的位置及信息
GameMsg* GameRole::CreateSrdPlayerPosition()
{
    pb::SyncPlayers* sysplayer = new  pb::SyncPlayers();
    std::list<Player*> srdplayer = world.GetSrdPlayers(this);

    for (auto single : srdplayer)
    {      
        auto pRole = dynamic_cast<GameRole*>(single);
        //addps先开辟内存后添加到父类对象中后在进行赋值，并返回子类指针
        auto player = sysplayer->add_ps();
        player->set_pid(pRole->iPid);
        player->set_username(pRole->sZname);
        //mutable_p先添加到父类对象中后在进行赋值，并返回子类指针
        auto position = player->mutable_p();
        position->set_x(pRole->x);
        position->set_y(pRole->y);
        position->set_z(pRole->z);
        position->set_v(pRole->v);
    }
    
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_SRD_PLAYER, sysplayer);
    return pmsg;
}
//向周围玩家发送自己位置及信息
GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* selfpos = new pb::BroadCast();    
    selfpos->set_pid(iPid);
    selfpos->set_username(sZname);
    selfpos->set_tp(2);
    
    auto position = selfpos->mutable_p();
    position->set_x(x);
    position->set_y(y);
    position->set_z(z);
    position->set_v(v);


    GameMsg* pmsg = new  GameMsg(GameMsg::GAME_MSG_BROADCAST, selfpos);
    return pmsg;
}
//玩家下线发送一些信息
GameMsg* GameRole::CreateIDNameLogoff()
{
    pb::SyncPid* pid = new  pb::SyncPid();
    pid->set_username(sZname);
    pid->set_pid(iPid);
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_LOGIOFF_ID_NAME, pid);
    return pmsg;
}
//设置本玩家将要发送广播的聊天信息
GameMsg* GameRole::CreateSendTalkMsg(std::string contenMsg)
{
    
    //在这个函数内部自己写的注释掉的都有错
    //auto allPlayer = ZinxKernel::Zinx_GetAllRole();
    pb::BroadCast* brdmsg = new pb::BroadCast();
    //auto srdPlayer = world.GetSrdPlayers(this);

    /*for (auto single : srdPlayer)
    {*/
	brdmsg->set_pid(iPid);
	brdmsg->set_username(sZname);
	brdmsg->set_tp(1);
	/*auto content = brdmsg->mutable_content();
	content->assign(contenMsg);*/
    brdmsg->set_content(contenMsg);

	GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_BROADCAST, brdmsg);
    return pmsg;
}
//出现在新的周围玩家视野中
void GameRole::ViewAppear(GameRole* _pRole)
{

    //向自己发送周围玩家信息200号
    auto pmsg = _pRole->CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*pmsg, *m_Protocol);
    //向周围玩家发送自己信息200号
    pmsg = CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_Protocol));
}
void GameRole::ViewLost(GameRole* _pRole)
{
    //向自己发送周围玩家下线的201消息
    GameMsg* pMsg = _pRole->CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *(m_Protocol));
    //向周围玩家发送自己下线的201消息
    pMsg = CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *(_pRole->m_Protocol));
}
//
bool GameRole::Init()
{
    //在初始化的时候将定时任务从定时列表中取出
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
    {
        TimerHandlemng::GetInfomng().deleteTimerproc(&get_exittimer);
    }
    bool pRet = false;
    //将pid赋值为每个连接的socket，
    iPid = m_Protocol->m_Channel->GetFd();
    //添加自身到地图中
    pRet = world.AddPlayers(this);
    //如果自身对象添加成功，则开始进行一些操作
    if (pRet == true)
    {
        //发送ID和名称
        GameMsg* pMsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pMsg, *m_Protocol); 
        //向自己发送周围玩家位置及信息
        pMsg = CreateSrdPlayerPosition();
        ZinxKernel::Zinx_SendOut(*pMsg, *m_Protocol);
        //向周围玩家发送本玩家的位置及信息
        auto srd_player = world.GetSrdPlayers(this);
        for (auto single : srd_player)
        {
            pMsg = CreateSelfPosition();
            auto pRole = dynamic_cast<GameRole*>(single);
            ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Protocol));
        }
    }
    //存姓名使用输出数据流
    /*std::ofstream save_name("/tmp/name_record", std::ios::app);
    save_name << sZname << std::endl;*/
    //使用hiredis进行姓名缓存
    //1.建立hiredis连接
    auto connect = redisConnect("127.0.0.1", 6379);
    //2.判断是否建立成功后，进行使用命令
    if (NULL != connect)
    {
        freeReplyObject(redisCommand(connect, "lpush game_name %s", sZname.c_str()));
        redisFree(connect);
    }

    return pRet;
}

/*接收到的是协议对象传进来的protobuf消息结构*/
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MUltiMsgs, input, _poUserData);

    /*遍历list链表中各个消息内容以及类型*/  
    //收到聊天消息
    for (auto single : input.m_pMsg)
    {
        std::cout << "type is: " << single->enumMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;
        //获取新位置
        auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);
        switch (single->enumMsgType)
        {
            //判断是否是聊天消息<<<< 自己写少了一个判断,还有发送的消息需要自己指定！！！>>>>
        case GameMsg::GAME_MSG_CHAT_CONTENT:
            ProcTalkMsg(dynamic_cast<pb::Talk*> (single->pMsg)->content());
            break;
        case GameMsg::GAME_MSG_NEW_POSITION:           
            ProcMoveMsg(NewPos->x(), NewPos->y(), NewPos->z(), NewPos->v());
            break;
        default:
            break;
        }

    }
    
    return nullptr;
}

void GameRole::ProcTalkMsg(std::string sendcontent)
{
    //向所有玩家发送聊天消息
    auto m_pRole = ZinxKernel::Zinx_GetAllRole();
    for (auto m_role : m_pRole)
    {
        GameMsg* pMsg = CreateSendTalkMsg(sendcontent);

        auto pRole = dynamic_cast<GameRole*>(m_role);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Protocol));
    }
}

void GameRole::ProcMoveMsg(float _x, float _y, float _z, float _v)
{
    //先跨网格视野切换
    //1.获取移动前的周围玩家s1旧
    auto s1 = world.GetSrdPlayers(this);
    //从网格中取出
    world.DelPlayers(this);
    //移动玩家
    x = _x;
    y = _y;
    z = _z;
    v = _v;
    //添加到新的网格中
    world.AddPlayers(this);
    //2.获取移动后的周围玩家s2
    auto s2 = world.GetSrdPlayers(this);

    //新邻居互相能看见
    for (auto newplayer : s2)
    {
        //在旧邻居里没有元素时出现在新邻居内
        if (s1.end() == find(s1.begin(), s1.end(), newplayer))
        {
            //出现在新周围玩家视野中
            ViewAppear(dynamic_cast<GameRole*>(newplayer));
        }
    }
    //旧邻居互相看不见
    for (auto oldplayer : s1)
    {
        if (s2.end() == find(s2.begin(), s2.end(), oldplayer))
        {
            //消失在旧周围玩家的视野中
            ViewLost(dynamic_cast<GameRole*>(oldplayer));
        }
    }
    //再广播新位置给周围玩家
    //1.首先获取新位置    
    //3.遍历周围玩家，发送广播消息
    auto srd_player = world.GetSrdPlayers(this);
    for(auto single : srd_player)
    {
        //2.再组成新广播消息报文
        pb::BroadCast* pBmsg = new pb::BroadCast();
        pBmsg->set_username(sZname);
        pBmsg->set_pid(iPid);
        pBmsg->set_tp(4);
        auto ptrPos = pBmsg->mutable_p();
        ptrPos->set_x(_x);
        ptrPos->set_y(_y);
        ptrPos->set_z(_z);
        ptrPos->set_v(_v);
        auto pRole = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::GAME_MSG_BROADCAST, pBmsg)), *(pRole->m_Protocol));
    }
}


void GameRole::Fini()
{
    //玩家下线,向周围玩家发送本玩家下线的消息
    auto srd_player = world.GetSrdPlayers(this);
    for (auto single : srd_player)
    {
        GameMsg* pMsg = CreateIDNameLogoff();
        auto pRole = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Protocol));
    }
    //从游戏世界中删除本玩家
    world.DelPlayers(this);
    //判断框架内玩家是否为空,如果为空则将定时任务添加到定时列表中
    if (ZinxKernel::Zinx_GetAllRole().size()<1)
    {              
        TimerHandlemng::GetInfomng().addTimerproc(&get_exittimer);
    }  
  

    //使用hiredis进行姓名缓存
    //1.建立hiredis连接
    auto connect = redisConnect("127.0.0.1", 6379);
    //2.判断是否建立成功后，进行使用命令
    if (NULL != connect)
    {
        freeReplyObject(redisCommand(connect, "lrem game_name 1 %s", sZname.c_str()));
        redisFree(connect);
    }
}

GameRole::GameRole()
{
    //初始化用户名
    //sZname = "Tom";
    sZname = random_name.GetName();
    x = 100 + random_engine() % 50;
    z = 100 + random_engine() % 50;

}

GameRole::~GameRole()
{
    random_name.ReleaseName(sZname);
}

int GameRole::GetX()
{
    return (int)x;
}

int GameRole::GetY()
{
    return (int)z;
}
