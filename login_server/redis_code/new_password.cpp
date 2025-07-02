#include<string>
#include<fcgi_stdio.h>
#include"CJsonObject/CJsonObject.hpp"
#include"user_opt.h"
using namespace std;
using namespace neb;
int main()
{
	while(FCGI_Accept()>=0)
	{
		//通过环境变量获取请求长度，并通过标准输入获取数据
		//将数据存入到字符串中
		string username;
		string password;
		int req_len = atoi(getenv("CONTENT_LENGTH"));
		char *buff = (char *)calloc(1UL,req_len);
		fread(buff,1,req_len,stdin);
		string json_string(buff);
		//将字符串放到json对象中转换为可直接获取数据的对象
		CJsonObject json(json_string);
		
		json.Get("username",username);
		json.Get("password",password);

		//准备回应
		printf("Content-Type:application/json\r\n");
		CJsonObject rep_json;
		DelUser(username.c_str());
		AddUserMd5(username.c_str(),password.c_str());
		rep_json.Add("login_result","OK");

		string rep_string = rep_json.ToFormattedString();
		printf("Content-Length:%d\r\n\r\n",rep_string.size());
		printf("%s\r\n",rep_string.c_str());

		free(buff);
			
	}
}
