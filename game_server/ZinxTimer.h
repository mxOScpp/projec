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
    
    // 通过 Ichannel 继承
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

/*构建一个接口类，其子类继承后实现其功能，可实现多态每个子类动作不同*/
class TimerOutProc {
public:
    //超时处理
    virtual void Proc() = 0;
    //超时时间
    virtual int GetTimerSec() = 0;
    /*之前是代表消耗时间，现在是所需圈数*/
    int iCount = -1;
};

/*构建一个超时管理类，用来处理超时任务，*/
class TimerHandlemng :public AZinxHandler
{
   // std::list<TimerOutProc *> m_task_list;//任务保存列表
    std::vector<std::list<TimerOutProc*>> m_timer_list;
    int Cur_index = 0;
    TimerHandlemng();
    static TimerHandlemng Hmng;//单例对象只有一份
public:

    // 通过 AZinxHandler 继承
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
    virtual void addTimerproc(TimerOutProc* _ptask);
    virtual void deleteTimerproc(TimerOutProc* _ptask);
    static TimerHandlemng& GetInfomng()
    {
        return Hmng;        
    }
};
