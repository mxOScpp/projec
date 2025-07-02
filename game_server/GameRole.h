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

    //�̳���AOI�࣬����Ҫ�Ĳ���
    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;
    int iPid = 0;
    std::string sZname;
    //������¼ʱID,Name����Ϣ�ĺ���������
    GameMsg* CreateIDNameLogin();
    //������Χ��ҵ�λ����Ϣ�ĺ���
    GameMsg* CreateSrdPlayerPosition();
    //��������Χ��ҷ����Լ���λ�õĺ���
    GameMsg* CreateSelfPosition();
    //����������߷����Լ�ID,Name����Ϣ�ĺ���������
    GameMsg* CreateIDNameLogoff();
    //��ȡ������Ϣ��������ҷ���������Ϣ
    //������������ҷ���������Ϣ�ĺ���
    GameMsg* CreateSendTalkMsg(std::string  contenMsg);
    void ViewAppear(GameRole* _pRole);
    void ViewLost(GameRole* _pRole);
public:
    // ͨ�� Irole �̳�
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void ProcTalkMsg(std::string sendcontent);
    void ProcMoveMsg(float _x, float _y, float _z,float _v);
    void Fini() override;

    GameRole();
    virtual ~GameRole();
    GameProtocol* m_Protocol = NULL;

    // ͨ�� Player �̳�
    int GetX() override;
    int GetY() override;
   


};

