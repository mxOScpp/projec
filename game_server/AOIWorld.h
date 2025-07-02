#pragma once
#include<list>
#include<vector>
/*1、创建玩家，网格，矩形类
  2、在矩形世界类中增加方法
*/
//玩家类内为成员函数，并且为纯虚函数，这样方便作为接口
class Player
{
public:
	virtual int GetX() = 0;
	virtual int GetY() = 0;
};
//使用list容器来存储玩家信息，方便增删
class Grid
{
public:
	//存储各个玩家
	std::list<Player*> m_player;
};
//使用vector容器来存储网格，主要是游戏世界一般是以数组来存在
class AOIWorld
{
	/*初始矩形世界中的一些元素,x与y的开始与结束范围，网格数量，网格精度*/
	int x_begin = 0;
	int x_end = 0;
	int y_begin = 0;
	int y_end = 0;
	int x_count = 0;
	int y_count = 0;
	int x_width = 0;
	int y_width = 0;
public:
	/*初始化矩形世界中的一些元素，使用构造来进行初始化*/
	AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);
	//存储网格
	std::vector<Grid> m_grid;
	/*创建获取周围玩家的函数*/
	std::list<Player*> GetSrdPlayers(Player* _player);

	/*创建添加玩家到网格的函数*/
	bool AddPlayers(Player* _player);
	/*删除在网格中玩家的函数*/
	void DelPlayers(Player* _player);
};