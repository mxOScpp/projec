#pragma once
#include <zinx.h>
#include<sys/timerfd.h>
#include<list>
#include<vector>
class TimerChannel : public Ichannel
{
private:
    int m_timerfd = -1;
public:
    
    // ͨ�� Ichannel �̳�
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

/*����һ���ӿ��࣬������̳к�ʵ���书�ܣ���ʵ�ֶ�̬ÿ�����ද����ͬ*/
class TimerOutProc {
public:
    //��ʱ����
    virtual void Proc() = 0;
    //��ʱʱ��
    virtual int GetTimerSec() = 0;
    /*֮ǰ�Ǵ�������ʱ�䣬����������Ȧ��*/
    int iCount = -1;
};

/*����һ����ʱ�����࣬��������ʱ����*/
class TimerHandlemng :public AZinxHandler
{
   // std::list<TimerOutProc *> m_task_list;//���񱣴��б�
    std::vector<std::list<TimerOutProc*>> m_timer_list;
    int Cur_index = 0;
    TimerHandlemng();
    static TimerHandlemng Hmng;//��������ֻ��һ��
public:

    // ͨ�� AZinxHandler �̳�
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
    virtual void addTimerproc(TimerOutProc* _ptask);
    virtual void deleteTimerproc(TimerOutProc* _ptask);
    static TimerHandlemng& GetInfomng()
    {
        return Hmng;        
    }
};
