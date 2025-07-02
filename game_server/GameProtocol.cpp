#include "GameProtocol.h"
#include"GameMsg.h"
#include"GameChannel.h"
using namespace std;

/*请求数据在此处转换为业务需求的数据，将字节流转为
  protobuf格式传递给上一层，同时需要解决TCP粘包，在
  zinx框架中已经设置了消息格式为TLV格式（固定了数据的
  一部分大小为8个字节），
1.首先在协议层声明一个变量来接收数据，
2.借助循环来接收多条数据，
3.通过位操作符来接收数据固定头部，
4.接收数据，并存储在链表中，
5.弹出已使用的数据；
*/
UserData* GameProtocol::raw2request(std::string _szInput)
{
    /*定义返回值*/
    MUltiMsgs* pRet = new MUltiMsgs();
    /*将数据拼接在协议对象中*/
    sLaststr.append(_szInput);

    while (1)
    {
        /*判断字节是否大于8个字节*/
        if (sLaststr.size() < 8)
        {
            break;
        }
        /*将数据头部使用位操作解析出来*/
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
        /*判断数据长度是否小于头部标识的大小*/
        if (iLength < (sLaststr.size() - 8))
        {
            break;
        }
        /*将消息内容封装到消息类中，发送到业务层*/
        GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, sLaststr.substr(8, iLength));
        /*将消息对象添加到消息链表中*/
        pRet->m_pMsg.push_back(pMsg);
        /*弹出已使用的数据*/
        sLaststr.erase(0, 8 + iLength);
    }



    return pRet;
}
/*业务层待发送的数据，返回的是转换后的字节流
上层传下来的是消息内容，不包括头部*/
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
/*交给消息处理对象，而消息处理对象在上层就属于是业务对象，就是向上传递，返回独有的业务对象*/
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
