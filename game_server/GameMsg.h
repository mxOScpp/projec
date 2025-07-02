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
	/*��protobuf�ṹתΪ�ֽ���������protobuf��������*/
	/*��֪��Ϣ��������������*/
	GameMsg(MSG_TYPE _type, google::protobuf::Message* pMsg);
	/*���ֽ����������ƣ�תΪprotobuf�ṹ*/
	GameMsg(MSG_TYPE _type, std::string pString);
	/*�����л��ṹתΪ�ֽ���*/
	std::string serialize();
	//GameMsg() = default;
	virtual ~GameMsg();
};
class MUltiMsgs:public UserData
{
public:
	std::list<GameMsg*> m_pMsg;
};
