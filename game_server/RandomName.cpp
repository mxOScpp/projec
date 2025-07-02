#include "RandomName.h"
#include<random>
#include<unistd.h>
#include<iostream>
#include<algorithm>
#include<fstream>


static std::default_random_engine randomname_engine(time(NULL));

//取名字                                                                               
std::string RandomName::GetName()
{
    //先得到一个随机数
    auto num = randomname_engine() % m_poll.size();
    //随机取姓
    auto first = m_poll[num]->m_first;
    //随机取名
    auto last = m_poll[num]->m_last_list[randomname_engine() % m_poll[num]->m_last_list.size()];
    //判断名有没有取完，如果取完把姓去掉
    if (m_poll[num]->m_last_list.size() <= 0)
    {
        delete m_poll[num];
        m_poll.erase(m_poll.begin() + num);
    }

    return first+" "+last;
}
//还名字，进行拆分
void RandomName::ReleaseName(std::string _name)
{
    //获取空格位置
    auto spacepos = _name.find(" ",0);
    //进行拆分姓与名
    auto firstname = _name.substr(0, spacepos);
    auto lastname = _name.substr(spacepos+1, _name.size() - spacepos - 1);
    //添加判断第二种情况的条件
    bool found = false;
    //如果没有姓就创建对象，如果有姓没有名直接插入
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
    //读文件
    std::ifstream first("random_first.txt");
    std::ifstream last("random_last.txt");

    //使用getline函数获取文件内每行
    std::string firstname;
    std::string lastname;
    //创建临时vector变量来存储名
    std::vector<std::string> tmp;
    //一行一行进行获取名
    while (getline(last, lastname))
    {
        tmp.push_back(lastname);
    }
    //读所有的姓添加到池子中，并且要创建对象，将每个姓赋值一串名
    while (getline(first, firstname))
    {       
        auto first_name_last = new FirstName();
        first_name_last->m_first = firstname;   
        first_name_last->m_last_list = tmp;
        //，获取后添加到池子中。
        m_poll.push_back(first_name_last);
    }
}
