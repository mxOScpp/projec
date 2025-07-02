#include "GameProtocol.h"
#include"GameMsg.h"
#include"GameChannel.h"
using namespace std;

/*���������ڴ˴�ת��Ϊҵ����������ݣ����ֽ���תΪ
  protobuf��ʽ���ݸ���һ�㣬ͬʱ��Ҫ���TCPճ������
  zinx������Ѿ���������Ϣ��ʽΪTLV��ʽ���̶������ݵ�
  һ���ִ�СΪ8���ֽڣ���
1.������Э�������һ���������������ݣ�
2.����ѭ�������ն������ݣ�
3.ͨ��λ���������������ݹ̶�ͷ����
4.�������ݣ����洢�������У�
5.������ʹ�õ����ݣ�
*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
    /*���巵��ֵ*/
    MUltiMsgs* pRet = new MUltiMsgs();
    /*������ƴ����Э�������*/
    sLaststr.append(_szInput);

    while (1)
    {
        /*�ж��ֽ��Ƿ����8���ֽ�*/
        if (sLaststr.size() < 8)
        {
            break;
        }
        /*������ͷ��ʹ��λ������������*/
        int iLength = 0;
        iLength |= sLaststr[0] << 0;
        iLength |= sLaststr[1] << 8;
        iLength |= sLaststr[2] << 16;
        iLength |= sLaststr[3] << 24;

        int id = 0;
        id |= sLaststr[4] << 0;
        id |= sLaststr[5] << 8;
        id |= sLaststr[6] << 16;
        id |= sLaststr[7] << 24;
        /*�ж����ݳ����Ƿ�С��ͷ����ʶ�Ĵ�С*/
        if (iLength < (sLaststr.size() - 8))
        {
            break;
        }
        /*����Ϣ���ݷ�װ����Ϣ���У����͵�ҵ���*/
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, sLaststr.substr(8, iLength));
        /*����Ϣ������ӵ���Ϣ������*/
        pRet->m_pMsg.push_back(pMsg);
        /*������ʹ�õ�����*/
        sLaststr.erase(0, 8 + iLength);
    }



    return pRet;
}
/*ҵ�������͵����ݣ����ص���ת������ֽ���
�ϲ㴫����������Ϣ���ݣ�������ͷ��*/
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    int id = 0;
    int ilength = 0;
    std::string MsgContent;
    GET_REF2DATA(GameMsg, pMsg, _oUserData);
    id = pMsg.enumMsgType;
    MsgContent = pMsg.serialize();
    ilength = MsgContent.size();
    std::string *pRet = new string();

    pRet->push_back((ilength >> 0) & 0xff);
    pRet->push_back((ilength >> 8) & 0xff);
    pRet->push_back((ilength >> 16) & 0xff);
    pRet->push_back((ilength >> 24) & 0xff);
    pRet->push_back((id >> 0) & 0xff);
    pRet->push_back((id >> 8) & 0xff);
    pRet->push_back((id >> 16) & 0xff);
    pRet->push_back((id >> 24) & 0xff);
    pRet->append(MsgContent);
    return pRet;
}
/*������Ϣ������󣬶���Ϣ����������ϲ��������ҵ����󣬾������ϴ��ݣ����ض��е�ҵ�����*/
Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return m_Role;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return m_Channel;
}

GameProtocol::~GameProtocol()
{
    if (NULL != m_Role)
    {
        ZinxKernel::Zinx_Del_Role(*m_Role);
        delete m_Role;

    }
}
