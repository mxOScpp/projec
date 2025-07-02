#pragma once

#include<ZinxTCP.h>
#include"GameProtocol.h"
#include"GameRole.h"
class GameChannel :public ZinxTcpData
{
public:
    virtual ~GameChannel();
    /*创建一个协议对象，让每个新创建的通道类进行绑定，（可能不是这样，有可能是为了下一层的连通）这样就可以实现特定通道传输的数据类型不同*/
    GameProtocol* m_Proto = NULL;
    //GameRole* m_Role = NULL;
    GameChannel(int _fd);
    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};
/*通道工厂类来进行与协议对象进行绑定与创建对象*/
class GameConnFact :public IZinxTcpConnFact
{
    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};