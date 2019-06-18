#include"GameScene.h"
//#include "Player.h"
#include "Operator.h"
#include"Weapon.h"
#include "Settings.h"
#include"OPOperator.h"
#include"thread"
#include"AiPlayer.h"
#include"AiTest.h"
#include"Settings.h"
USING_NS_CC;
//static Player* player;

//#include "SimpleAudioEngine.h"

//#include <string>
//#include <iostream>


Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	scene->retain();
	auto game_layer = GameScene::create();

	game_layer->setName("GameScene");
	game_layer->setTag(10);
	scene->addChild(game_layer);

	return scene;

}

TMXTiledMap* GameScene::getCurrentMap() {
	auto currentScene = Director::getInstance()->getRunningScene();
	auto scene = currentScene->getChildByName("GameScene");
	return dynamic_cast<TMXTiledMap*>(scene->getChildByName("Map"));
}

GameScene* GameScene::getCurrentScene() {
	auto currentScene = Director::getInstance()->getRunningScene();
	return dynamic_cast<GameScene*>(currentScene->getChildByName("GameScene"));
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	MapItems = Array::create();
	Players = Array::create();
	Bullets = Array::create();
	MapItems->retain();
	Players->retain();
	Bullets->retain();

	auto operate = Operator::create();
	addChild(operate);
	
	auto x = Director::getInstance()->getWinSize().width;
	auto y = Director::getInstance()->getWinSize().height;

	/*auto exitItem = MenuItemLabel::create(
		Label::create("Exit", "fonts/Cordelia.ttf", 30), CC_CALLBACK_1(GameScene::exitCallback, this));
	exitItem->setPosition(-x * 0.45, y *0.45);
	auto menu = Menu::create();
	menu->addChild(exitItem);
	addChild(menu, 100);*/

	/*auto upItem = MenuItemLabel::create(
		Label::create("up", "fonts/Cordelia.ttf", 30), CC_CALLBACK_1(GameScene::exitCallback, this));
	auto downItem = MenuItemLabel::create(
		Label::create("down", "fonts/Cordelia.ttf", 30), CC_CALLBACK_1(GameScene::exitCallback, this));
	auto leftItem = MenuItemLabel::create(
		Label::create("left", "fonts/Cordelia.ttf", 30), CC_CALLBACK_1(GameScene::exitCallback, this));
	auto rightItem = MenuItemLabel::create(
		Label::create("right", "fonts/Cordelia.ttf", 30), CC_CALLBACK_1(GameScene::exitCallback, this));
	upItem->setPosition(0, y *0.15);
	downItem->setPosition(0, -y *0.15);
	leftItem -> setPosition(-x*0.15, 0);
	rightItem -> setPosition(x * 0.15, 0);
	auto menu = Menu::create();
	menu->addChild(upItem);
	menu->addChild(downItem);
	menu->addChild(rightItem);
	menu->addChild(leftItem);
	addChild(menu, 100);*/


	return true;
}

void GameScene::onEnter() {
	Layer::onEnter();
	
	tileMap = TMXTiledMap::create("Map/Map01.tmx");
	BG = tileMap->getLayer("Ground");
	twall = tileMap->getLayer("Wall");
	Meta = tileMap->getLayer("Meta");
	tileMap->setName("Map");
	this->addChild(tileMap);

	Bulletset = Sprite::create();
	tileMap->addChild(Bulletset,100);
	Itemset = Sprite::create();
	tileMap->addChild(Itemset, 100);

	auto w = Director::getInstance()->getWinSize().width / (players + 1);
	auto h = Director::getInstance()->getWinSize().height / (players + 1);

	posList.push_back(Vec2(100, 100));
	posList.push_back(Vec2(100, 300));
	posList.push_back(Vec2(300, 100));
	posList.push_back(Vec2(300, 300));
	
	if (!gameMode) {
		for (int i = 0; i < players; i++) {
			std::string name(playerList[i]->_name);
			int id = playerList[i]->_id;

			auto player = Player::create(name, id);
			player->setZOrder(100);
			player->setPosition(posList[id]);
			if (id != local_Id) {
				player->setTag(player->getId()+6);
				auto op = OPOperator::create();
				op->setName("op");
				player->addChild(op);
			}
			tileMap->addChild(player->HPBar);
			tileMap->addChild(player);
			Players->addObject(player);
		}
	}
	else
	{
		int ap = 1;
		local_Id = ap;
		auto player = Player::create(local_username, ap);
		player->setPosition(posList[0]);
		player->setZOrder(100);
		tileMap->addChild(player->HPBar);
		tileMap->addChild(player);
		Players->addObject(player);

		/*for (int i = 4; i <= 4; i++)
		{
			auto ai = Player::create("ai"+std::to_string(i), i);
			ai->IsAI = 1;
			ai->setPosition(posList[i-3]);
			ai->weapon=nullptr;
			ai->MoveBegin();
			tileMap->addChild(ai->HPBar);
			tileMap->addChild(ai);

			auto aiop = AiPlayer::create();
			aiop->setName("aiop");
			ai->setZOrder(100);
			ai->addChild(aiop);
			Players->addObject(ai);

			
			
		}*/
		auto ai = AiTest::create("ai", 2);
		auto op = OPOperator::create();
		op->setName("op");
		ai->addChild(op);
		ai->setTag(8);
		ai->setPosition(posList[1]);
		ai->setZOrder(100);
		ai->setName("aitest");
		tileMap->addChild(ai);
		Players->addObject(ai);
	}


	this->scheduleUpdate();
	this->schedule(schedule_selector(GameScene::SpawnItems), 5.0f);
}
void GameScene::onExit() {
	Layer::onExit();
	endThread = 1;
}
void GameScene::update(float delta)
{
	this->IsDead();
	this->MapMove();
	this->IsWeaponIntoPlayer();
	this->PickMapItems();
	this->IsBulletIntoWall();
	this->IsBulletIntoPlayer();
	this->MovePlayer();

}
void GameScene::IsDead()
{
	auto players = CCArray::create();
	players->retain();
	Object* iplayer;
	CCARRAY_FOREACH(Players, iplayer)
	{
		Player *player = (Player*)iplayer;
		player->HPBar->setHp(player->HP);
		player->HPBar->setPosition(player->getPosition() + Vec2(0,35));
		if (player->HP <= 0)
		{
			player->Dead(player);
			players->addObject(player);
		}

	}
	CCARRAY_FOREACH(players, iplayer)
	{
		Players->removeObject(iplayer);
	}
}
void GameScene::SpawnItems(float dt)
{
	srand(int(time(0)) + rand());
	float x, y;
	while (true)
	{
		x = (float)(rand() % 1280);
		y = (float)(rand() % 1280);
		auto git = Meta->getTileGIDAt(Vec2((int)floor(x/ 32), (int)(39 - floor(y/ 32))));
		auto value = tileMap->getPropertiesForGID(git);
		auto valueMap = value.asValueMap();
		if (!valueMap.at("Collidable").asBool())break;
	}
	//int type = rand()%5;
	int type = 5;
	auto items = Sprite::create(settings::weapon_paths[type]);
	items->setTag(type);
	items->setPosition(x, y);
	auto jump = JumpBy::create(0.5f,Vec2(0,10),10,1);
	auto seq = Sequence::create(jump, jump->reverse(), nullptr);
	items->runAction(RepeatForever::create(seq));
	auto map=this->getChildByName("Map");
	MapItems->addObject(items);
	this->Itemset->addChild(items);
}
void GameScene::MapMove()
{
	auto map = this->getChildByName("Map");
	auto player = map->getChildByName("Player");
	auto playerPosition = player->getPosition();
	auto visiblesize = Director::getInstance()->getOpenGLView()->getVisibleSize();

	if (playerPosition.x > visiblesize.width / 2 && playerPosition.x < (1280 - visiblesize.width / 2))
		tileMap->setPositionX(visiblesize.width / 2 - playerPosition.x);
	if (playerPosition.y > visiblesize.height / 2 && playerPosition.y < (1280 - visiblesize.height / 2))
		tileMap->setPositionY(visiblesize.height / 2 - playerPosition.y);
}
void GameScene::IsWeaponIntoPlayer()
{
	auto players = Array::create();
	players->retain();
	Object*iplayer;
	Object*iplayer2;

	CCARRAY_FOREACH(Players, iplayer)
	{
		Player* player = (Player*)iplayer;
		if (player->weapon&&(player->weapon->MyWeapon->getOpacity())&& (player->WeaponType == 1 || player->WeaponType== 4))
		{

			auto tmpZone = (player->weapon->MyWeapon)->boundingBox();
			auto weaponZone = CCRectMake(tmpZone.origin.x + player->getPositionX(), tmpZone.origin.y + player->getPositionY(), tmpZone.size.width, tmpZone.size.height);;
			CCARRAY_FOREACH(Players, iplayer2)
			{
				Player* player2=(Player*)iplayer2;
				if (player != player2)
				{
					auto playerZone = CCRectMake(player2->getPositionX() - 9, player2->getPositionY() - 24, 18, 42);
					if (weaponZone.intersectsRect(playerZone))
					{
						player2->HP -= 2;
					}
				}
			}
		}
	}
}
void GameScene::IsBulletIntoPlayer()
{
	auto players = Array::create();
	players->retain();
	auto bullets = Array::create();
	bullets->retain();
	Object*iplayer;
	Object*ibullet;


	CCARRAY_FOREACH(Players, iplayer)
	{
		Sprite* player = (Sprite*)iplayer;
		auto playerZone = CCRectMake(player->getPositionX() - 9, player->getPositionY() - 24, 18, 42);
		CCARRAY_FOREACH(Bullets, ibullet)
		{
			Sprite* bullet = (Sprite*)ibullet;
			auto bulletZone = bullet->boundingBox();
			if (bulletZone.intersectsRect(playerZone))
			{
				if (((Sprite*)ibullet)->getName() != ((Player*)iplayer)->getName())
				{
					((Player*)iplayer)->HP -= 2;
					bullets->addObject(ibullet);
				}
			}
		}
	}

	CCARRAY_FOREACH(bullets, ibullet)
	{
		Sprite* bullet = (Sprite*)ibullet;
		if (bullet->getTag()<3)
		{
			Bullets->removeObject(ibullet);
			bullet->stopActionByTag(bullet->getTag());
			auto weapon = Weapon::create();
			(weapon->*(weapon->BulletEnd[bullet->getTag()]))(bullet);
		}
	}
}
bool GameScene::isAccessable(Point Position, int Direction)
{
	if (Direction == -1)return false;
	if (Position.x - 9 <=0 || Position.y - 24 <=0 || Position.x + 9 >= 1280 || Position.y + 18 >= 1280)
		return true;
	int dir[8][2] = { {9,0},{0,-24},{-9,0},{0,18},{9,-24},{-9,-24},{-9,18},{9,18}};
	auto git = Meta->getTileGIDAt(Vec2((int)floor((Position.x + dir[Direction][0]) / 32), (int)(39 - floor((Position.y + dir[Direction][1]) / 32))));
	auto value = tileMap->getPropertiesForGID(git);
	auto valueMap = value.asValueMap();
	if (valueMap.at("Collidable").asBool())return true;
	for (int i=0; i < 8; i++)
	{
		auto git = Meta->getTileGIDAt(Vec2((int)floor((Position.x + dir[i][0]) / 32), (int)(39 - floor((Position.y + dir[i][1]) / 32))));
		auto value = tileMap->getPropertiesForGID(git);
		auto valueMap = value.asValueMap();
		if (valueMap.at("Collidable").asBool())return true;
	}
	return false;

}
void GameScene::PickMapItems()
{
	auto players = Array::create();
	players->retain();
	auto mapitems = Array::create();
	mapitems->retain();
	Object*iplayer;
	Object*imapitem;

	CCARRAY_FOREACH(Players, iplayer)
	{
		Sprite* player = (Sprite*)iplayer;
		auto playerZone = CCRectMake(player->getPositionX() - 9, player->getPositionY() - 24, 18, 42);
		CCARRAY_FOREACH(MapItems, imapitem)
		{
			Sprite* mapitem = (Sprite*)imapitem;
			auto mapitemZone = CCRectMake(mapitem->getPositionX(), mapitem->getPositionY(), mapitem->getContentSize().width, mapitem->getContentSize().height);

			if (mapitemZone.intersectsRect(playerZone))
			{
				auto player = (Player*)iplayer;
				auto weapontype = ((Sprite*)imapitem)->getTag();
				player->WeaponType = weapontype;
				player->scheduleUpdate();
				if(player->getName()=="ai4"|| player->getName() == "ai5"|| player->getName() == "ai6")
					(player->getChildByName("aiop"))->scheduleOnce(schedule_selector(AiPlayer::AiAttack),player->AttackSpeed);

				mapitems->addObject(imapitem);
			}
		}
	}

	CCARRAY_FOREACH(mapitems, imapitem)
	{
		MapItems->removeObject(imapitem);
		Sprite* mapitem = (Sprite*)imapitem;
		mapitem->removeFromParentAndCleanup(TRUE);
	}
}
void GameScene::IsBulletIntoWall()
{
	auto bullets = Array::create();
	Object*ibullet;

	CCARRAY_FOREACH(Bullets, ibullet)
	{
		Sprite* bullet = (Sprite*)ibullet;
		auto bulletPosition = bullet->getPosition();
		int flag=0;
		if (bulletPosition.x <= 0 || bulletPosition.y <= 0 || bulletPosition.x >= 1280 || bulletPosition.y >= 1280)
			flag = 1;
		else
		{
			auto git = Meta->getTileGIDAt(Vec2((int)floor((bulletPosition.x) / 32), (int)(39 - floor((bulletPosition.y) / 32))));
			auto value = tileMap->getPropertiesForGID(git);
			auto valueMap = value.asValueMap();
			flag = valueMap.at("Collidable").asBool();
		}
		if(flag)
		{
			bullets->addObject(ibullet);
		}
	}
	CCARRAY_FOREACH(bullets, ibullet)
	{
		Sprite* bullet = (Sprite*)ibullet;
		if (bullet->getTag() == 0 || bullet->getTag() == 2)
		{
			Bullets->removeObject(ibullet);
			bullet->stopActionByTag(bullet->getTag());
			auto weapon = Weapon::create();
			(weapon->*(weapon->BulletEnd[bullet->getTag()]))(bullet);
		}
	}
}


void GameScene::exitCallback(Ref*ref) {
	//Director::getInstance()->popScene();
	MenuItemLabel* item = (MenuItemLabel*)ref;
	auto ai = static_cast<AiTest*>(tileMap->getChildByName("aitest"));
	char buffer[MSGSIZE];
	if (item->getString() == "up") {
		if (!ai->isUp) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 2);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
			ai->isUp = true;
		}
		else {
			ai->isUp = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 2);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
		}
	}
	else if (item->getString() == "down") {
		if (!ai->isDown) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 3);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
			ai->isDown = true;
		}
		else {
			ai->isDown = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 3);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
		}
	}
	else if (item->getString() == "right") {
		if (!ai->isRight) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 1);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
			ai->isRight = true;
		}
		else {
			ai->isRight = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 1);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
		}
	}
	else if (item->getString() == "left") {
		if (!ai->isLeft) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 0);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
			ai->isLeft = true;
		}
		else {
			ai->isLeft = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 0);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);//updatePlayer(buffer);
		}
	}
	
}
void GameScene::MovePlayer() {
	
	if (!cmdList.empty()) {
		
		std::list<std::string>::iterator it = cmdList.begin();
		auto buffer = (*it).c_str();
		
		if (buffer[0] == KeyPress || buffer[0] == KeyRelease) {
			int id = static_cast<int>(buffer[2]) - 48;
			if (id != local_Id)
				dynamic_cast<OPOperator*>(
					dynamic_cast<Player*>(
						GameScene::getCurrentMap()->getChildByTag(id + 6))->getChildByName("op"))->KeyStart(buffer);
		}
		else if (buffer[0] == MousePress || buffer[0] == MouseRelease) {
			int id = static_cast<int>(buffer[2]) - 48;
			if (id != local_Id)
				dynamic_cast<OPOperator*>(
					dynamic_cast<Player*>(
						GameScene::getCurrentMap()->getChildByTag(id + 6))->getChildByName("op"))->MouseStart(buffer);
		}
		cmdList.pop_front();
	}
}