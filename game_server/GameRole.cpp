#include "GameRole.h"
#include<iostream>
#include<unistd.h>
#include<random>
#include"ZinxTimer.h"
#include"RandomName.h"
#include<fstream>
#include<hiredis/hiredis.h>
//ʹ��AOI�໭һ���������磬������Ҷ�����������У�������Ҫȫ�־�̬�������洢
static AOIWorld world(0, 1000, 0, 1000, 20, 20);
static std::default_random_engine random_engine(time(NULL));
RandomName random_name;

//����һ����ʱ������󣬹涨��ʱʱ��
class ExitTimer :public TimerOutProc
{
    // ͨ�� TimerOutProc �̳�
    void Proc() override
    {
        ZinxKernel::Zinx_Exit();
    }
    int GetTimerSec() override
    {
        return 20;
    }
};
//����һ��ȫ�ֶ�����ȫ�ֶ���Ч
static ExitTimer get_exittimer;

//����ID������
GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid *pid =new  pb::SyncPid();
    pid->set_username(sZname);
    pid->set_pid(iPid);
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_LOGIN_ID_NAME, pid);
    return pmsg;
}
//��ǰ��ҷ�����Χ��ҵ�λ�ü���Ϣ
GameMsg* GameRole::CreateSrdPlayerPosition()
{
    pb::SyncPlayers* sysplayer = new  pb::SyncPlayers();
    std::list<Player*> srdplayer = world.GetSrdPlayers(this);

    for (auto single : srdplayer)
    {      
        auto pRole = dynamic_cast<GameRole*>(single);
        //addps�ȿ����ڴ����ӵ���������к��ڽ��и�ֵ������������ָ��
        auto player = sysplayer->add_ps();
        player->set_pid(pRole->iPid);
        player->set_username(pRole->sZname);
        //mutable_p����ӵ���������к��ڽ��и�ֵ������������ָ��
        auto position = player->mutable_p();
        position->set_x(pRole->x);
        position->set_y(pRole->y);
        position->set_z(pRole->z);
        position->set_v(pRole->v);
    }
    
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_SRD_PLAYER, sysplayer);
    return pmsg;
}
//����Χ��ҷ����Լ�λ�ü���Ϣ
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
//������߷���һЩ��Ϣ
GameMsg* GameRole::CreateIDNameLogoff()
{
    pb::SyncPid* pid = new  pb::SyncPid();
    pid->set_username(sZname);
    pid->set_pid(iPid);
    GameMsg* pmsg = new GameMsg(GameMsg::GAME_MSG_LOGIOFF_ID_NAME, pid);
    return pmsg;
}
//���ñ���ҽ�Ҫ���͹㲥��������Ϣ
GameMsg* GameRole::CreateSendTalkMsg(std::string contenMsg)
{
    
    //����������ڲ��Լ�д��ע�͵��Ķ��д�
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
//�������µ���Χ�����Ұ��
void GameRole::ViewAppear(GameRole* _pRole)
{

    //���Լ�������Χ�����Ϣ200��
    auto pmsg = _pRole->CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*pmsg, *m_Protocol);
    //����Χ��ҷ����Լ���Ϣ200��
    pmsg = CreateSelfPosition();
    ZinxKernel::Zinx_SendOut(*pmsg, *(_pRole->m_Protocol));
}
void GameRole::ViewLost(GameRole* _pRole)
{
    //���Լ�������Χ������ߵ�201��Ϣ
    GameMsg* pMsg = _pRole->CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *(m_Protocol));
    //����Χ��ҷ����Լ����ߵ�201��Ϣ
    pMsg = CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *(_pRole->m_Protocol));
}
//
bool GameRole::Init()
{
    //�ڳ�ʼ����ʱ�򽫶�ʱ����Ӷ�ʱ�б���ȡ��
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
    {
        TimerHandlemng::GetInfomng().deleteTimerproc(&get_exittimer);
    }
    bool pRet = false;
    //��pid��ֵΪÿ�����ӵ�socket��
    iPid = m_Protocol->m_Channel->GetFd();
    //���������ͼ��
    pRet = world.AddPlayers(this);
    //������������ӳɹ�����ʼ����һЩ����
    if (pRet == true)
    {
        //����ID������
        GameMsg* pMsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pMsg, *m_Protocol); 
        //���Լ�������Χ���λ�ü���Ϣ
        pMsg = CreateSrdPlayerPosition();
        ZinxKernel::Zinx_SendOut(*pMsg, *m_Protocol);
        //����Χ��ҷ��ͱ���ҵ�λ�ü���Ϣ
        auto srd_player = world.GetSrdPlayers(this);
        for (auto single : srd_player)
        {
            pMsg = CreateSelfPosition();
            auto pRole = dynamic_cast<GameRole*>(single);
            ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Protocol));
        }
    }
    //������ʹ�����������
    /*std::ofstream save_name("/tmp/name_record", std::ios::app);
    save_name << sZname << std::endl;*/
    //ʹ��hiredis������������
    //1.����hiredis����
    auto connect = redisConnect("127.0.0.1", 6379);
    //2.�ж��Ƿ����ɹ��󣬽���ʹ������
    if (NULL != connect)
    {
        freeReplyObject(redisCommand(connect, "lpush game_name %s", sZname.c_str()));
        redisFree(connect);
    }

    return pRet;
}

/*���յ�����Э����󴫽�����protobuf��Ϣ�ṹ*/
UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MUltiMsgs, input, _poUserData);

    /*����list�����и�����Ϣ�����Լ�����*/  
    //�յ�������Ϣ
    for (auto single : input.m_pMsg)
    {
        std::cout << "type is: " << single->enumMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;
        //��ȡ��λ��
        auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);
        switch (single->enumMsgType)
        {
            //�ж��Ƿ���������Ϣ<<<< �Լ�д����һ���ж�,���з��͵���Ϣ��Ҫ�Լ�ָ��������>>>>
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
    //��������ҷ���������Ϣ
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
    //�ȿ�������Ұ�л�
    //1.��ȡ�ƶ�ǰ����Χ���s1��
    auto s1 = world.GetSrdPlayers(this);
    //��������ȡ��
    world.DelPlayers(this);
    //�ƶ����
    x = _x;
    y = _y;
    z = _z;
    v = _v;
    //��ӵ��µ�������
    world.AddPlayers(this);
    //2.��ȡ�ƶ������Χ���s2
    auto s2 = world.GetSrdPlayers(this);

    //���ھӻ����ܿ���
    for (auto newplayer : s2)
    {
        //�ھ��ھ���û��Ԫ��ʱ���������ھ���
        if (s1.end() == find(s1.begin(), s1.end(), newplayer))
        {
            //����������Χ�����Ұ��
            ViewAppear(dynamic_cast<GameRole*>(newplayer));
        }
    }
    //���ھӻ��࿴����
    for (auto oldplayer : s1)
    {
        if (s2.end() == find(s2.begin(), s2.end(), oldplayer))
        {
            //��ʧ�ھ���Χ��ҵ���Ұ��
            ViewLost(dynamic_cast<GameRole*>(oldplayer));
        }
    }
    //�ٹ㲥��λ�ø���Χ���
    //1.���Ȼ�ȡ��λ��    
    //3.������Χ��ң����͹㲥��Ϣ
    auto srd_player = world.GetSrdPlayers(this);
    for(auto single : srd_player)
    {
        //2.������¹㲥��Ϣ����
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
    //�������,����Χ��ҷ��ͱ�������ߵ���Ϣ
    auto srd_player = world.GetSrdPlayers(this);
    for (auto single : srd_player)
    {
        GameMsg* pMsg = CreateIDNameLogoff();
        auto pRole = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*pMsg, *(pRole->m_Protocol));
    }
    //����Ϸ������ɾ�������
    world.DelPlayers(this);
    //�жϿ��������Ƿ�Ϊ��,���Ϊ���򽫶�ʱ������ӵ���ʱ�б���
    if (ZinxKernel::Zinx_GetAllRole().size()<1)
    {              
        TimerHandlemng::GetInfomng().addTimerproc(&get_exittimer);
    }  
  

    //ʹ��hiredis������������
    //1.����hiredis����
    auto connect = redisConnect("127.0.0.1", 6379);
    //2.�ж��Ƿ����ɹ��󣬽���ʹ������
    if (NULL != connect)
    {
        freeReplyObject(redisCommand(connect, "lrem game_name 1 %s", sZname.c_str()));
        redisFree(connect);
    }
}

GameRole::GameRole()
{
    //��ʼ���û���
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
