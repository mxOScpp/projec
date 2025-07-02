#include "ZinxTimer.h"
#include<iostream>

TimerHandlemng TimerHandlemng::Hmng;
//初始化定时器在构造通道时，设置定时文件描述符
bool TimerChannel::Init()
{

    bool tRet = false;
    //创建定时文件描述符
    int mFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (0 <= mFd)
    {
        //设置定时周期
        struct itimerspec s = {
            {1,0},{1,0}
        };
        int mSet = timerfd_settime(mFd, 0, &s, NULL);
        if (0 == mSet)
        {
            tRet = true;
            m_timerfd = mFd;
        }
    }
    return tRet;
}
//内容读到定时通道中
bool TimerChannel::ReadFd(std::string& _input)
{
    bool cRet = false;
    char buf[8] = { 0 };

    int sizeRead = read(m_timerfd, buf, sizeof(buf));
    if (sizeof(buf) == sizeRead)
    {
        cRet = true;
        //_input.push_back(*buf);使用push与assign赋值都可以成功打印
        _input.assign(buf, sizeof(buf));
    }
    return cRet;
}

bool TimerChannel::WriteFd(std::string& _output)
{
    return false;
}
//关闭定时文件描述符
void TimerChannel::Fini()
{
    close(m_timerfd);
    m_timerfd = -1;
}
//返回定时文件描述符
int TimerChannel::GetFd()
{
    return m_timerfd;
}
//返回定时通道
std::string TimerChannel::GetChannelInfo()
{
    return "timerout";
}
//交给下一个处理类，协议类对象，之前是内部写的类，现在是超时任务管理类
AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return &(TimerHandlemng::GetInfomng());
}
TimerHandlemng::TimerHandlemng()
{
    for (int i = 0; i < 10; i++)
    {
        std::list<TimerOutProc*> tmp;
        m_timer_list.push_back(tmp);
    }
}
/*遍历所有定时任务，计数减一，若为0则执行该任务的处理函数*/
IZinxMsg* TimerHandlemng::InternelHandle(IZinxMsg& _oInput)
{
    /*超时次数,借用read返回的数据来计算*/
    unsigned long iTimerCount = 0;

    GET_REF2DATA(BytesMsg, input, _oInput);
    input.szData.copy((char*) & iTimerCount, sizeof(iTimerCount), 0);
    while ((iTimerCount--) > 0)
    {
        /*第一、肯定要移动当前刻度*/
        ++Cur_index;
        Cur_index %= 10;
        std::list<TimerOutProc*> m_cache;
        /*第二、就是遍历齿中任务列表，列表中圈数为0或圈数减一*/
        for (auto itr = m_timer_list[Cur_index].begin(); itr != m_timer_list[Cur_index].end();)
        {
            if ((*itr)->iCount <= 0)
            {
                //先添加到缓冲列表内之后再统一运行列表内任务
                m_cache.push_back(*itr);
                auto tmp = *itr;
                //从当前齿中删除，添加到新的齿中
                itr = m_timer_list[Cur_index].erase(itr);
                addTimerproc(tmp);
            }
            else
            {
                --(*itr)->iCount;
                ++itr;
            }
        }
        /*统一执行处理超时函数*/
        for (auto task : m_cache)
        {
            task->Proc();
        }
    }    
    return nullptr;
}

AZinxHandler* TimerHandlemng::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}
/*添加任务到任务列表list*/
void TimerHandlemng::addTimerproc(TimerOutProc* _ptask)
{
    /*计算在那个齿上*/
    int index = (_ptask->GetTimerSec() + Cur_index) % 10;
    /*添加任务到对应的list中*/
    m_timer_list[index].push_back(_ptask);
    /*计算所需圈数*/
    _ptask->iCount = _ptask->GetTimerSec() / 10;
}
/*从任务列表list删除任务*/
void TimerHandlemng::deleteTimerproc(TimerOutProc* _ptask)
{
    /*删除就需要遍历链表中的元素*/
    for (auto &chi : m_timer_list)
    {
        for (auto task : chi)
        {
            if (task == _ptask)
            {
                chi.remove(_ptask);
                return;
            }
        }
    }
}
