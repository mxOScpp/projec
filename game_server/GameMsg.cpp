#include "GameMsg.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* pMsg):enumMsgType(_type),pMsg(pMsg)
{
}
/*从字节流（二进制）转为protobuf结构*/
GameMsg::GameMsg(MSG_TYPE _type, std::string pString) :enumMsgType(_type)
{
	switch (_type)
	{
	case GameMsg::GAME_MSG_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::GAME_MSG_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case GameMsg::GAME_MSG_NEW_POSITION:
		pMsg = new pb::Position();
		break;
	case GameMsg::GAME_MSG_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case GameMsg::GAME_MSG_LOGIOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::GAME_MSG_SRD_PLAYER:
		pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	pMsg->ParseFromString(pString);
}
/*protobuf格式转换为字符流二进制格式*/
std::string GameMsg::serialize()
{
	std::string ret ;

	pMsg->SerializeToString(&ret);
	return ret;
}

GameMsg::~GameMsg()
{
	if (NULL != pMsg)
	{
		delete  pMsg;
	}
}
