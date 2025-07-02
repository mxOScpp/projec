#include<string>
#include<stdlib.h>
#include"user_opt.h"
#include<fcgi_stdio.h>

using namespace std;

int main()
{
	while(FCGI_Accept() >= 0)
	{
		char buff[1024]={0};
		fread(buff,1,sizeof(buff),stdin);
		std::string orig_string(buff);
		int and_pos = orig_string.find("&");
		string username = orig_string.substr(9,and_pos-9);
	        string password = orig_string.substr(and_pos+10,orig_string.size()-and_pos-10);
		printf("Content-Type=text\r\n\r\n");
		if(true == Checkuser(username.c_str(),NULL))
		{
			//说明用户存在
			printf("User Exist\r\n");
		}	
		else
		{
			//用户不存在，需添加
			Adduser(username.c_str(),password.c_str());
			printf("Sucess\r\n");
		}
	}
}
