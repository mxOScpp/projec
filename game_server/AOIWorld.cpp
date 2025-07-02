#include "AOIWorld.h"
using namespace std;

//初始化元素
AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count):
    x_begin(_x_begin),x_end(_x_end),y_begin(_y_begin),y_end(_y_end),x_count(_x_count),y_count(_y_count)
{
    //在初始化时就完成对网格宽度（精度）的计算
    // x轴网格宽度=(x轴结束坐标-x轴起始坐标)/x轴网格数量；y轴的计算方式相同 
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;
    //在构造中要初始化容器，把网格的内容初始化，一般知道网格数量就会知道矩形内需要多少网格
    for (int i = 0; i < x_count * y_count; ++i)
    {
        Grid tmp;
        m_grid.push_back(tmp);
    }
}
//获取周围玩家信息
std::list<Player*> AOIWorld::GetSrdPlayers(Player* _player)
{
    //存放周围玩家的list容器
    std::list<Player*> ret;
    //计算网格编号
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
    //通过网格编号来确定周围玩家的范围
    int x_srd = grid_id % x_count;
    int y_srd = grid_id / y_count;
    //判断，开始确定周围玩家的位置
    //左上角玩家
    if (x_srd > 0 && y_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - 1 - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //正上方玩家
    if (y_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //右上角玩家
    if ((x_srd < (x_count - 1)) && (y_srd > 0))
    {
        std::list<Player*>& list_player = m_grid[grid_id + 1 - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //左方玩家
    if (x_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - 1].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //本玩家
    std::list<Player*>& list_player = m_grid[grid_id ].m_player;
    ret.insert(ret.end(), list_player.begin(), list_player.end());
    //右方玩家
    if (x_srd < (x_count - 1))
    {
        std::list<Player*>& list_player = m_grid[grid_id +1].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //左下角玩家
    if ((x_srd > 0) && (y_srd < (y_count - 1)))
    {
        std::list<Player*>& list_player = m_grid[grid_id -1+ x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //正下方玩家
    if (y_srd < (y_count - 1))
    {
        std::list<Player*>& list_player = m_grid[grid_id + x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //右下角玩家
    if ((x_srd < (x_count - 1)) && (y_srd < (y_count - 1)))
    {
        std::list<Player*>& list_player = m_grid[grid_id +1+x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    return ret;
}

//添加玩家需要知道玩家在那个网格
bool AOIWorld::AddPlayers(Player* _player)
{
    //计算网格编号
    //网格编号=(x-x轴起始坐标)/x轴网格宽度 + (y-y轴起始坐标)/y轴宽度*x轴网格数量
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;

    //将玩家添加到对应的网格中
    m_grid[grid_id].m_player.push_back(_player);
    return true;
}

void AOIWorld::DelPlayers(Player* _player)
{
    //同样删除也需要网格所需的编号
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
    //使用remove操作来删除
    m_grid[grid_id].m_player.remove(_player);
}
