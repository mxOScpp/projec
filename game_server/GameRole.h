#pragma once
#include<zinx.h>
#include"AOIWorld.h"
#include"GameMsg.h"
#include"GameChannel.h"
#include"msg.pb.h"
#include "GameProtocol.h"

class GameProtocol;
class GameRole : public Irole,public Player
{

    //继承了AOI类，所需要的参数
    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;
    int iPid = 0;
    std::string sZname;
    //创建登录时ID,Name的消息的函数并发出
    GameMsg* CreateIDNameLogin();
    //创建周围玩家的位置消息的函数
    GameMsg* CreateSrdPlayerPosition();
    //创建向周围玩家发送自己的位置的函数
    GameMsg* CreateSelfPosition();
    //创建玩家下线发送自己ID,Name的消息的函数并发出
    GameMsg* CreateIDNameLogoff();
    //获取聊天消息向所有玩家发送聊天信息
    //创建向所有玩家发送聊天信息的函数
    GameMsg* CreateSendTalkMsg(std::string  contenMsg);
    void ViewAppear(GameRole* _pRole);
    void ViewLost(GameRole* _pRole);
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void ProcTalkMsg(std::string sendcontent);
    void ProcMoveMsg(float _x, float _y, float _z,float _v);
    void Fini() override;

    GameRole();
    virtual ~GameRole();
    GameProtocol* m_Protocol = NULL;

    // 通过 Player 继承
    int GetX() override;
    int GetY() override;
   


};

