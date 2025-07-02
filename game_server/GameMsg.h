#pragma once
#include<zinx.h>
#include<google/protobuf/message.h>
#include"msg.pb.h"
class GameMsg : public UserData 
{
	
public:
	google::protobuf::Message* pMsg = NULL;
	enum MSG_TYPE
	{
		GAME_MSG_LOGIN_ID_NAME = 1,
		GAME_MSG_CHAT_CONTENT = 2,
		GAME_MSG_NEW_POSITION = 3,
		GAME_MSG_BROADCAST = 200,
		GAME_MSG_LOGIOFF_ID_NAME = 201,
		GAME_MSG_SRD_PLAYER = 202,
	}enumMsgType{};
	/*从protobuf结构转为字节流，传入protobuf对象类型*/
	/*已知消息类型来创建对象*/
	GameMsg(MSG_TYPE _type, google::protobuf::Message* pMsg);
	/*从字节流（二进制）转为protobuf结构*/
	GameMsg(MSG_TYPE _type, std::string pString);
	/*从序列化结构转为字节流*/
	std::string serialize();
	//GameMsg() = default;
	virtual ~GameMsg();
};
class MUltiMsgs:public UserData
{
public:
	std::list<GameMsg*> m_pMsg;
};
