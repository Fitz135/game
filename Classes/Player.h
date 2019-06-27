#pragma once
#include"cocos2d.h"
#include"Entity.h"
#include"HP.h"

using namespace cocos2d;
class Weapon;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string , int);

	static Player* create(std::string , int);
	static Player* create(Entity*);
	Hp* HPBar;
	Weapon* weapon;

	int MoveSpeed;
	int WeaponType;
	int HP=100;
	float AttackSpeed;
	bool IsHaveWeapon=0;
	bool AttackAbleFlag=1;
	bool IsAI = 0;

	void update(float dt);
	void MoveBegin();
	void MoveEnd();
	void AttackBegan(Point MousePosition);
	void AttackEnd(int pressnum);
	void AttackMode1(Point TouchPosition);
	void AttackMode2(Point TouchPosition);
	void AttackMode3(Point TouchPosition);
	void Dead(Node * who);

	typedef struct 
	{
		uint8_t speed;//�ٶ�
		uint8_t attack;//������
		uint8_t hp;//����ֵ
		uint8_t weapon;//����
		uint8_t superPower;//�ӳ�Ч��
		uint8_t level;//�ȼ�
		uint8_t exp;//����
	}Attr;//��ɫ����

	int getId();
private:
	Entity info;
	std::string m_name;
	int m_id;
	Attr attr;

	bool initWithPlayerType(int id);
	Animate * createAnimate(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end, float delay);
	void ChangeWeapon(int WeaponType);
	void AttackAbleflag(float dt);
	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;
	Sprite *Hand;
	int CharaType;
};
