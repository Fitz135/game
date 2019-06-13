#pragma once
#include"cocos2d.h"
using namespace cocos2d;
class Weapon;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string , int);
	//CREATE_FUNC(Player);
	static Player* create(std::string , int);
	bool initWithPlayerType(int id);
	Player * getMychara(char* str);
	int CharaType;

	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;
	Sprite *Hand;
	Sprite *MyWeapon;
	Weapon* weapon;

	bool IsHaveWeapon;
	bool AttackAbleFlag;
	bool AttackEndFlag;

	Vector<SpriteFrame*> MoveFrames[4];

	Vector<SpriteFrame*> AnimationFrames(std::basic_string<char, std::char_traits<char>, std::allocator<char>> FrameName, int begin, int end);
	Animate * createAnimate(int i);

	void MoveBegin();

	void MoveEnd();

	void AttackAbleflag(float dt);

	void AttackEndflag(float dt);

	void AttackEnd(int pressnum);

	void AttackBegan(Point MousePosition);

	void AttackMode1(Point TouchPosition);

	struct Attr
	{
		uint8_t speed;//�ٶ�
		uint8_t attack;//������
		uint8_t hp;//����ֵ
		uint8_t weapon;//����
		uint8_t superPower;//�ӳ�Ч��
		uint8_t level;//�ȼ�
		uint8_t exp;//����
	};//��ɫ����

	void AttackMode2(Point TouchPosition);

	int getId();
	std::string getName();
private:
	std::string m_name;
	int m_id;
	Attr attr;
};
