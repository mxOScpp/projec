#pragma once

#include<ZinxTCP.h>
#include"GameProtocol.h"
#include"GameRole.h"
class GameChannel :public ZinxTcpData
{
public:
    virtual ~GameChannel();
    /*����һ��Э�������ÿ���´�����ͨ������а󶨣������ܲ����������п�����Ϊ����һ�����ͨ�������Ϳ���ʵ���ض�ͨ��������������Ͳ�ͬ*/
    GameProtocol* m_Proto = NULL;
    //GameRole* m_Role = NULL;
    GameChannel(int _fd);
    // ͨ�� ZinxTcpData �̳�
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
/*ͨ����������������Э�������а��봴������*/
class GameConnFact :public IZinxTcpConnFact
{
    // ͨ�� IZinxTcpConnFact �̳�
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};