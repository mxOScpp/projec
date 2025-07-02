#include "AOIWorld.h"
using namespace std;

//��ʼ��Ԫ��
AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count):
    x_begin(_x_begin),x_end(_x_end),y_begin(_y_begin),y_end(_y_end),x_count(_x_count),y_count(_y_count)
{
    //�ڳ�ʼ��ʱ����ɶ������ȣ����ȣ��ļ���
    // x��������=(x���������-x����ʼ����)/x������������y��ļ��㷽ʽ��ͬ 
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;
    //�ڹ�����Ҫ��ʼ������������������ݳ�ʼ����һ��֪�����������ͻ�֪����������Ҫ��������
    for (int i = 0; i < x_count * y_count; ++i)
    {
        Grid tmp;
        m_grid.push_back(tmp);
    }
}
//��ȡ��Χ�����Ϣ
std::list<Player*> AOIWorld::GetSrdPlayers(Player* _player)
{
    //�����Χ��ҵ�list����
    std::list<Player*> ret;
    //����������
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
    //ͨ����������ȷ����Χ��ҵķ�Χ
    int x_srd = grid_id % x_count;
    int y_srd = grid_id / y_count;
    //�жϣ���ʼȷ����Χ��ҵ�λ��
    //���Ͻ����
    if (x_srd > 0 && y_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - 1 - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //���Ϸ����
    if (y_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //���Ͻ����
    if ((x_srd < (x_count - 1)) && (y_srd > 0))
    {
        std::list<Player*>& list_player = m_grid[grid_id + 1 - x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //�����
    if (x_srd > 0)
    {
        std::list<Player*>& list_player = m_grid[grid_id - 1].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //�����
    std::list<Player*>& list_player = m_grid[grid_id ].m_player;
    ret.insert(ret.end(), list_player.begin(), list_player.end());
    //�ҷ����
    if (x_srd < (x_count - 1))
    {
        std::list<Player*>& list_player = m_grid[grid_id +1].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //���½����
    if ((x_srd > 0) && (y_srd < (y_count - 1)))
    {
        std::list<Player*>& list_player = m_grid[grid_id -1+ x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //���·����
    if (y_srd < (y_count - 1))
    {
        std::list<Player*>& list_player = m_grid[grid_id + x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    //���½����
    if ((x_srd < (x_count - 1)) && (y_srd < (y_count - 1)))
    {
        std::list<Player*>& list_player = m_grid[grid_id +1+x_count].m_player;
        ret.insert(ret.end(), list_player.begin(), list_player.end());
    }
    return ret;
}

//��������Ҫ֪��������Ǹ�����
bool AOIWorld::AddPlayers(Player* _player)
{
    //����������
    //������=(x-x����ʼ����)/x�������� + (y-y����ʼ����)/y����*x����������
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;

    //�������ӵ���Ӧ��������
    m_grid[grid_id].m_player.push_back(_player);
    return true;
}

void AOIWorld::DelPlayers(Player* _player)
{
    //ͬ��ɾ��Ҳ��Ҫ��������ı��
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
    //ʹ��remove������ɾ��
    m_grid[grid_id].m_player.remove(_player);
}
