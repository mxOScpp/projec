#pragma once
#include<vector>
#include<string>

//建造池子存储的对象
class FirstName
{
public:
	std::string m_first;
	std::vector<std::string> m_last_list;
};
//随机名字类
class RandomName
{
public:
	std::vector< FirstName*>m_poll;
	std::string GetName();
	void  ReleaseName(std::string _name);
	void LoadFile();
};

