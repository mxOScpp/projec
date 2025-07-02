#pragma once
#include<zinx.h>
#include"msg.pb.h"
class GameChannel;
class GameRole;
class GameProtocol : public Iprotocol
{
    std::string sLaststr;
    // Í¨¹ý Iprotocol ¼Ì³Ð
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
public:
    GameChannel* m_Channel = NULL;
    GameRole* m_Role = NULL;
    virtual ~GameProtocol();
};

