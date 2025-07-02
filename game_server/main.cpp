#include"GameChannel.h"
#include"GameMsg.h"
#include<list>
#include"AOIWorld.h"
#include"ZinxTimer.h"
#include"RandomName.h"
#include<unistd.h>
//声明
extern RandomName random_name;
void daemon()
{
    //守护进程
    //1.创建子进程，父进程退出
    int fpid = fork();
    if (0 > fpid)
    {
        exit(-1);
    }
    if (0 < fpid)
    {
        exit(0);
    }
    //2.创建会话id
    setsid();
    //3.重定向3个文件描述符到/dev/null
    int nullfd = open("/dev/null",O_RDWR);
    if (0 < nullfd)
    {
        dup2(nullfd, 0);
        dup2(nullfd, 1);
        dup2(nullfd, 2);
        close(nullfd);
    }
    //进程监控
    //1.进入循环后创建子进程
    while (true)
    {
        int pid = fork();
        if (0 > pid)
        {
            exit(-1);
        }
        //2.父进程等待
        if (0 < pid)
        {
            int status;
            wait(&status);
            if (0 == status)
            {
                exit(0);
            }           
        }
        //3.子进程跳出
        else
        {
            break;
        }
    }
}
//可以通过记录玩家姓名来查看
//通过tmp文件来获取
//可以在初始化阶段进行存，在结束阶段进行删
int main()
{
  

    daemon();
    random_name.LoadFile();
    ZinxKernel::ZinxKernelInit();
     
    ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(9999, new GameConnFact())));
    ZinxKernel::Zinx_Add_Channel(*(new TimerChannel()));
    ZinxKernel::Zinx_Run();
    ZinxKernel::ZinxKernelFini();
    return 0;
}