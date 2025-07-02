#include "GameChannel.h"

GameChannel::~GameChannel()
{
	if (NULL != m_Proto)
	{
		ZinxKernel::Zinx_Del_Proto(*m_Proto);
		delete m_Proto;
		
	}
}

GameChannel::GameChannel(int _fd) :ZinxTcpData(_fd)
{

}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	/*消息处理的下一个环节是协议类*/
	return m_Proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
	/*协议类对象与通道类的创建与绑定*/
	/*这是互相绑定*/
	auto pChannel = new GameChannel(_fd);
	auto pProtocol = new GameProtocol();
	auto pRole = new GameRole();
	/*协议类对象与通道类对象互相绑定，目的是为了每个连接的客户端都有所对应的协议对象来处理*/
	pChannel->m_Proto = pProtocol;
	pProtocol->m_Channel = pChannel;
	/*协议类对象与业务类对象互相绑定，目的是为了让协议层的数据能转发到特定需要的业务类对象*/
	pProtocol->m_Role = pRole;
	pRole->m_Protocol = pProtocol;
	/*协议对象添加到框架内核*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	ZinxKernel::Zinx_Add_Role(*pRole);
	return pChannel;
}
