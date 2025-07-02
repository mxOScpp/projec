#include "RandomName.h"
#include<random>
#include<unistd.h>
#include<iostream>
#include<algorithm>
#include<fstream>


static std::default_random_engine randomname_engine(time(NULL));

//ȡ����                                                                               
std::string RandomName::GetName()
{
    //�ȵõ�һ�������
    auto num = randomname_engine() % m_poll.size();
    //���ȡ��
    auto first = m_poll[num]->m_first;
    //���ȡ��
    auto last = m_poll[num]->m_last_list[randomname_engine() % m_poll[num]->m_last_list.size()];
    //�ж�����û��ȡ�꣬���ȡ�����ȥ��
    if (m_poll[num]->m_last_list.size() <= 0)
    {
        delete m_poll[num];
        m_poll.erase(m_poll.begin() + num);
    }

    return first+" "+last;
}
//�����֣����в��
void RandomName::ReleaseName(std::string _name)
{
    //��ȡ�ո�λ��
    auto spacepos = _name.find(" ",0);
    //���в��������
    auto firstname = _name.substr(0, spacepos);
    auto lastname = _name.substr(spacepos+1, _name.size() - spacepos - 1);
    //����жϵڶ������������
    bool found = false;
    //���û���վʹ��������������û����ֱ�Ӳ���
    for (auto first_name_last : m_poll)
    {
        if (firstname == first_name_last->m_first)
        {
            found = true;
            first_name_last->m_last_list.push_back(lastname);
            break;
        }
    }
    if (false == found)
    {
		auto first = new FirstName();
		//first->m_first = firstname;
		first->m_last_list.push_back(lastname);
		m_poll.push_back(first);
    }
    
}

void RandomName::LoadFile()
{
    //���ļ�
    std::ifstream first("random_first.txt");
    std::ifstream last("random_last.txt");

    //ʹ��getline������ȡ�ļ���ÿ��
    std::string firstname;
    std::string lastname;
    //������ʱvector�������洢��
    std::vector<std::string> tmp;
    //һ��һ�н��л�ȡ��
    while (getline(last, lastname))
    {
        tmp.push_back(lastname);
    }
    //�����е�����ӵ������У�����Ҫ�������󣬽�ÿ���ո�ֵһ����
    while (getline(first, firstname))
    {       
        auto first_name_last = new FirstName();
        first_name_last->m_first = firstname;   
        first_name_last->m_last_list = tmp;
        //����ȡ����ӵ������С�
        m_poll.push_back(first_name_last);
    }
}
