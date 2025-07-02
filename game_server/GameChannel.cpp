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
	/*��Ϣ�������һ��������Э����*/
	return m_Proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
	/*Э���������ͨ����Ĵ������*/
	/*���ǻ����*/
	auto pChannel = new GameChannel(_fd);
	auto pProtocol = new GameProtocol();
	auto pRole = new GameRole();
	/*Э���������ͨ���������󶨣�Ŀ����Ϊ��ÿ�����ӵĿͻ��˶�������Ӧ��Э�����������*/
	pChannel->m_Proto = pProtocol;
	pProtocol->m_Channel = pChannel;
	/*Э���������ҵ���������󶨣�Ŀ����Ϊ����Э����������ת�����ض���Ҫ��ҵ�������*/
	pProtocol->m_Role = pRole;
	pRole->m_Protocol = pProtocol;
	/*Э�������ӵ�����ں�*/
	ZinxKernel::Zinx_Add_Proto(*pProtocol);
	ZinxKernel::Zinx_Add_Role(*pRole);
	return pChannel;
}
