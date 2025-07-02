namespace TestCodeUnless
{
	void test()
	{
        /*std::list<int> s;
    std::list<int> ss;
    ss.push_back(1);
    ss.push_back(2);
    ss.push_back(3);
    ss.push_back(4);
    ss.push_back(5);

    list<int>::const_iterator it = ss.begin();
    initializer_list<int> sss = ss;
    s.insert(it,);*/
    /*
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(1);
    pMsg->set_username("hhh_test");



    GameMsg gm(GameMsg::GAME_MSG_LOGIN_ID_NAME, pMsg);
    auto pOut = gm.serialize();

    for (auto byte : pOut)
    {
        printf("%02x ", byte);
    }
    puts("");

    char buf[] = { 0x08,0x01,0x12,0x08,0x68,0x68,0x68,0x5f,0x74,0x65,0x73,0x74 };
    std::string input(buf, sizeof(buf));
   auto msg = GameMsg(GameMsg::GAME_MSG_LOGIN_ID_NAME, input);
    std::cout << dynamic_cast<pb::SyncPid*>(msg.pMsg)->pid() << std::endl;
    std::cout << dynamic_cast<pb::SyncPid*>(msg.pMsg)->username() << std::endl;
*/
       /* pb::BroadCast* bc = new pb::BroadCast();
        bc->set_pid(2);
        bc->set_username("BroadCast");
        bc->set_tp(3);
        bc->set_content("hello world!!");

        GameMsg gbc(GameMsg::GAME_MSG_BROADCAST, bc);
        auto pBc = gbc.serialize();
        for (auto byte : pBc)
        {
            printf("%02x ", byte);
        }
        puts("");

        char buf[] = { 0x08,0x02 ,0x10 ,0x03 ,0x1a ,0x0d ,0x68 ,0x65 ,0x6c ,0x6c ,0x6f ,0x20 ,0x77 ,0x6f ,0x72 ,0x6c ,0x64 ,0x21 ,0x21 ,0x32 ,0x09 ,0x42 ,0x72 ,0x6f ,0x61 ,0x64 ,0x43 ,0x61 ,0x73 ,0x74 };
        std::string ss(buf, sizeof(buf));
        auto newgm = GameMsg(GameMsg::GAME_MSG_BROADCAST, ss);
        std::cout << dynamic_cast<pb::BroadCast*>(newgm.pMsg)->tp() << std::endl;
        std::cout << dynamic_cast<pb::BroadCast*>(newgm.pMsg)->pid() << std::endl;
        std::cout << dynamic_cast<pb::BroadCast*>(newgm.pMsg)->username() << std::endl;
        std::cout << dynamic_cast<pb::BroadCast*>(newgm.pMsg)->content() << std::endl;*/

        //for (auto ss : pRet->m_pMsg)
//{
//    cout << ss->pMsg->Utf8DebugString() << endl;
//    //cout << ss->pMsg->Utf8DebugString() << endl;
//}

//pb::Talk* pmsg = new pb::Talk();
//pmsg->set_content("bye");

//GameMsg* pgamemsg = new GameMsg(GameMsg::GAME_MSG_CHAT_CONTENT, pmsg);
//ZinxKernel::Zinx_SendOut(*pgamemsg, *this);


        /*class myplayer :public Player
{
public:
    int x = 0;
    int y = 0;
    std::string name;
    myplayer(int _x, int _y, std::string _name) :x(_x), y(_y), name(_name){}
    // 通过 Player 继承
    int GetX() override
    {
        return x;
    }
    int GetY() override
    {
        return y;
    }

};*/
        /* AOIWorld w(20, 200, 50, 230, 6, 6);
    myplayer p1(60, 107,"1");
    myplayer p2(91, 118,"2");
    myplayer p3(147, 133,"3");
    w.AddPlayers(&p1);
    w.AddPlayers(&p2);
    w.AddPlayers(&p3);
    auto srdplayer = w.GetSrdPlayers(&p1);
    for (auto single : srdplayer)
    {
        std::cout << dynamic_cast<myplayer*>(single)->name << endl;
    }
    //ZinxKernel::ZinxKernelFini();080210031a0d68656c6c6f20776f726c642121320942726f616443617374
   // 0801120474657374
    //07000000020000000A0568656C6C6F
*/
//GameRole：298
        {
            //删名字需要在fini内部进行删除
    //读出所有玩家姓名，将不是退出的本玩家的姓名重新写入
    /*std::list<std::string> name_list;
    std::string tmp;
    std::ifstream name_file("/tmp/name_record");
    while (getline(name_file, tmp))
    {
        name_list.push_back(tmp);
    }
    std::ofstream re_name("/tmp/name_record");
    for (auto name : name_list)
    {
        if (name != sZname)
        {
            re_name << name << std::endl;
        }
    }*/
        }

	}
}