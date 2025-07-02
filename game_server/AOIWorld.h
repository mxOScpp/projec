#pragma once
#include<list>
#include<vector>
/*1��������ң����񣬾�����
  2���ھ��������������ӷ���
*/
//�������Ϊ��Ա����������Ϊ���麯��������������Ϊ�ӿ�
class Player
{
public:
	virtual int GetX() = 0;
	virtual int GetY() = 0;
};
//ʹ��list�������洢�����Ϣ��������ɾ
class Grid
{
public:
	//�洢�������
	std::list<Player*> m_player;
};
//ʹ��vector�������洢������Ҫ����Ϸ����һ����������������
class AOIWorld
{
	/*��ʼ���������е�һЩԪ��,x��y�Ŀ�ʼ�������Χ���������������񾫶�*/
	int x_begin = 0;
	int x_end = 0;
	int y_begin = 0;
	int y_end = 0;
	int x_count = 0;
	int y_count = 0;
	int x_width = 0;
	int y_width = 0;
public:
	/*��ʼ�����������е�һЩԪ�أ�ʹ�ù��������г�ʼ��*/
	AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);
	//�洢����
	std::vector<Grid> m_grid;
	/*������ȡ��Χ��ҵĺ���*/
	std::list<Player*> GetSrdPlayers(Player* _player);

	/*���������ҵ�����ĺ���*/
	bool AddPlayers(Player* _player);
	/*ɾ������������ҵĺ���*/
	void DelPlayers(Player* _player);
};