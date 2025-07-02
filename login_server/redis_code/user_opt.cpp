#include <sys/types.h>
#include <sys/wait.h>
#include<string>

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include"user_opt.h"
#include<hiredis/hiredis.h>
using namespace std;
void Adduser(const char * _name,const char * _passwd)
{
	char buff[1024] = {0};
	sprintf(buff,"./add_user.sh %s %s",_name,_passwd);
	system(buff);
}
bool Checkuser(const char * _name,const char* _passwd_dgst)
{
	bool bRet = false;
	if(fork()>0)
	{
		int iSval = 0;
		wait(&iSval);
		if(0==iSval)
		{
			bRet = true;
		}
	}
	else
	{
		execlp("./check_user.sh","./check_user.sh",_name,_passwd_dgst,NULL);
	}


	return bRet;
}
void DelUser(const char *_name)
{
	char buff[1024]={0};
	sprintf(buff,"./del_user.sh %s",_name);
	system(buff);
}
void AddUserMd5(const char *_name, const char *_password)
{
	char buff[1024]={0};
	sprintf(buff,"echo %s:%s >> userfile",_name,_password);
	system(buff);
}
static int g_cur_select = 0;
string  create_container(int cur_room_no)
{

	string port;
	//1. define ip
	string server_array[2]={"192.168.144.183","192.168.144.185"};
	string cur_server = server_array[g_cur_select];
	++g_cur_select;
	g_cur_select %= sizeof(server_array)/sizeof(server_array[0]);
	//2.redis send
	auto pc = redisConnect("127.0.0.1",6379);
	if(NULL!=pc)
	{
		freeReplyObject(redisCommand(pc,"publish create_room %s:%d",cur_server.c_str(),cur_room_no));

	//3.reply port
	
		freeReplyObject(redisCommand(pc,"subscribe server_port"));
		redisReply *reply = NULL;
		if(REDIS_OK == redisGetReply(pc,(void **)&reply))
		{
			port.append(reply->element[2]->str);
			freeReplyObject(reply);
		}
		redisFree(pc);
	}	
	//4.connect ip port




	return cur_server+" "+port;
}

string check_room(const char * _room_no)
{
	//ip:xxxxx port:xx
	string bRet;
	
	redisContext*pc = redisConnect("127.0.0.1",6379);
	if(NULL!=pc)
	{
		redisReply* reply=(redisReply*)redisCommand(pc,"get room_no%s",_room_no);
		if(reply->str !=NULL){
		bRet.append(reply->str);
		}
		freeReplyObject(reply);
		redisFree(pc);
	}

	return bRet;
}
#if 0
bool check_room(const char * _room_no)
{
	bool bRet = false;
	if(fork()>0)
	{
		int iSval = 0;
		wait(&iSval);
		if(0==iSval)
		{
			bRet = true;
		}
	}
	else
	{
		execlp("./check_room.sh","./check_room.sh",_room_no,NULL);
	}


	return bRet;
}
#endif
