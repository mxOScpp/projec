#pragma once
#include<vector>
#include<string>

//������Ӵ洢�Ķ���
class FirstName
{
public:
	std::string m_first;
	std::vector<std::string> m_last_list;
};
//���������
class RandomName
{
public:
	std::vector< FirstName*>m_poll;
	std::string GetName();
	void  ReleaseName(std::string _name);
	void LoadFile();
};

