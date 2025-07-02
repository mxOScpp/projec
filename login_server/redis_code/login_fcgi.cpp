#include<iostream>
#include<stdlib.h>
#include"CJsonObject/CJsonObject.hpp"
#include<string>
#include"user_opt.h"
#include<fcgi_stdio.h>
#include<hiredis/hiredis.h>
using namespace std;
using namespace neb;
static int cur_room_no=0;
int main()
{
	while(FCGI_Accept()>=0)
	{

		string username;
		string password;
		//取数据
		int req_len = atoi(getenv("CONTENT_LENGTH"));
		char * buff = (char *)calloc(1UL,req_len);
		fread(buff,1,req_len,stdin);
		string json_string(buff);
		CJsonObject ojson(json_string);

		ojson.Get("username",username);
		ojson.Get("password",password);
		printf("Content-Type:application/json\r\n");
		neb::CJsonObject rep_json;
		if(true== Checkuser(username.c_str(),password.c_str()))
		{
			//查找成功，可以登录
			rep_json.Add("login_result","OK");
			string opt;
			ojson.Get("opt",opt);
			if("create_room" == opt)
			{
				cur_room_no++;
				//recv ip and port : ip port
				string room_info  = create_container(cur_room_no);

				redisContext* pc =redisConnect("127.0.0.1",6379);
				if(NULL!=pc)
				{

					freeReplyObject(redisCommand(pc,"set room_no%d %s",cur_room_no,room_info.c_str()));	
					redisFree(pc);
				}

				rep_json.Add("room_no",cur_room_no);
				rep_json.Add("server_IP",room_info.substr(0,room_info.find(' ')));
				rep_json.Add("server_PORT",room_info.substr(room_info.find(' ')+1,room_info.size()-room_info.find(' ')-1));
			}
			else if("follow_room"==opt)
			{
				string room_no;
				ojson.Get("room_no",room_no);
				string room_info = check_room(room_no.c_str());

				if(room_info.size()<=0)
				{

					rep_json.Add("follow_result","failed");
				}
				else
				{

					rep_json.Add("follow_result","OK");
					rep_json.Add("server_IP",room_info.substr(0,room_info.find(' ')));
					rep_json.Add("server_PORT",room_info.substr(room_info.find(' ')+1,room_info.size()-room_info.find(' ')-1));
				}

			}
		}
		else
		{
			//查找失败，登录失败
			rep_json.Add("login_result","failed");
		}
		string rep_string = rep_json.ToFormattedString();
		printf("Content-Length:%d\r\n\r\n",rep_string.size());
		printf("%s\r\n",rep_string.c_str());

		free(buff);

	}
}
