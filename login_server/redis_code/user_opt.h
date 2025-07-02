#include<string>
#include<stdio.h>
#include<stdbool.h>
void Adduser(const char * _name,const char * _passwd);
bool Checkuser(const char * _name,const char* _passwd_dgst);
void DelUser(const char *_name);
void AddUserMd5(const char *_name, const char *_password);
std::string create_container(int cur_room_no);
std::string check_room(const char * _room_no);
