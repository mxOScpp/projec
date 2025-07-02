#include "ZinxTimer.h"
#include<iostream>

TimerHandlemng TimerHandlemng::Hmng;
//��ʼ����ʱ���ڹ���ͨ��ʱ�����ö�ʱ�ļ�������
bool TimerChannel::Init()
{

    bool tRet = false;
    //������ʱ�ļ�������
    int mFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (0 <= mFd)
    {
        //���ö�ʱ����
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
//���ݶ�����ʱͨ����
bool TimerChannel::ReadFd(std::string& _input)
{
    bool cRet = false;
    char buf[8] = { 0 };

    int sizeRead = read(m_timerfd, buf, sizeof(buf));
    if (sizeof(buf) == sizeRead)
    {
        cRet = true;
        //_input.push_back(*buf);ʹ��push��assign��ֵ�����Գɹ���ӡ
        _input.assign(buf, sizeof(buf));
    }
    return cRet;
}

bool TimerChannel::WriteFd(std::string& _output)
{
    return false;
}
//�رն�ʱ�ļ�������
void TimerChannel::Fini()
{
    close(m_timerfd);
    m_timerfd = -1;
}
//���ض�ʱ�ļ�������
int TimerChannel::GetFd()
{
    return m_timerfd;
}
//���ض�ʱͨ��
std::string TimerChannel::GetChannelInfo()
{
    return "timerout";
}
//������һ�������࣬Э�������֮ǰ���ڲ�д���࣬�����ǳ�ʱ���������
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
/*�������ж�ʱ���񣬼�����һ����Ϊ0��ִ�и�����Ĵ�����*/
IZinxMsg* TimerHandlemng::InternelHandle(IZinxMsg& _oInput)
{
    /*��ʱ����,����read���ص�����������*/
    unsigned long iTimerCount = 0;

    GET_REF2DATA(BytesMsg, input, _oInput);
    input.szData.copy((char*) & iTimerCount, sizeof(iTimerCount), 0);
    while ((iTimerCount--) > 0)
    {
        /*��һ���϶�Ҫ�ƶ���ǰ�̶�*/
        ++Cur_index;
        Cur_index %= 10;
        std::list<TimerOutProc*> m_cache;
        /*�ڶ������Ǳ������������б��б���Ȧ��Ϊ0��Ȧ����һ*/
        for (auto itr = m_timer_list[Cur_index].begin(); itr != m_timer_list[Cur_index].end();)
        {
            if ((*itr)->iCount <= 0)
            {
                //����ӵ������б���֮����ͳһ�����б�������
                m_cache.push_back(*itr);
                auto tmp = *itr;
                //�ӵ�ǰ����ɾ������ӵ��µĳ���
                itr = m_timer_list[Cur_index].erase(itr);
                addTimerproc(tmp);
            }
            else
            {
                --(*itr)->iCount;
                ++itr;
            }
        }
        /*ͳһִ�д���ʱ����*/
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
/*������������б�list*/
void TimerHandlemng::addTimerproc(TimerOutProc* _ptask)
{
    /*�������Ǹ�����*/
    int index = (_ptask->GetTimerSec() + Cur_index) % 10;
    /*������񵽶�Ӧ��list��*/
    m_timer_list[index].push_back(_ptask);
    /*��������Ȧ��*/
    _ptask->iCount = _ptask->GetTimerSec() / 10;
}
/*�������б�listɾ������*/
void TimerHandlemng::deleteTimerproc(TimerOutProc* _ptask)
{
    /*ɾ������Ҫ���������е�Ԫ��*/
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
