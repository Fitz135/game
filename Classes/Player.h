#pragma once
#include"cocos2d.h"
using namespace cocos2d;
class Player :public cocos2d::Sprite
{
public:
	Player(std::string name, int id);
	//CREATE_FUNC(Player);
	static Player* create(std::string name, int id);
	bool initWithPlayerType();
	Player * getMychara(char* str);

	Sprite *Body;
	Sprite *Head;
	Sprite *Legs;

	bool AttackAbleFlag;
	bool AttackEndFlag;

	Vector<SpriteFrame*> MoveFrames[3];
	Vector<SpriteFrame*> AttackFrames;

	Vector<SpriteFrame*> AnimationFrames(const char * FrameName, int begin, int end);
	Animate * createMoveAni(int i);
	Animate * createAttackAni();

	void MoveBegin();

	void MoveEnd();

	void AttackAbleflag(float dt);

	void AttackEndflag(float dt);

	void AttackEnd(int pressnum);

	void Attack_Shoot(Point MousePosition);

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
private:
	std::string m_name;
	int m_id;
	Attr attr;
};
