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
#include"ResultScene.h"
USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	scene->retain();
	auto game_layer = GameScene::create();

	game_layer->setName("GameScene");
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
	

	return true;
}

void GameScene::onEnter() {
	Layer::onEnter();
	
	tileMap = TMXTiledMap::create("Map/Map01.tmx");
	Meta = tileMap->getLayer("Meta");
	tileMap->setName("Map");
	addChild(tileMap);

	Bulletset = Sprite::create();
	tileMap->addChild(Bulletset,100);
	Itemset = Sprite::create();
	tileMap->addChild(Itemset, 100);

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
				player->setTag(player->getId()+10);
				auto op = OPOperator::create();
				op->setName("op");
				player->addChild(op);
			}
			tileMap->addChild(player->HPBar,100);
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
		tileMap->addChild(player->HPBar,100);
		tileMap->addChild(player);
		Players->addObject(player);

		for (int i = 4; i <= 6; i++)
		{
			auto ai = Player::create("ai" + std::to_string(i), i);
			ai->IsAI = 1;
			ai->setPosition(posList[i - 3]);
			ai->weapon = nullptr;
			ai->MoveBegin();
			Players->addObject(ai);
			tileMap->addChild(ai->HPBar,100);
			tileMap->addChild(ai,100);

			auto aiop = AiPlayer::create();
			aiop->setName("aiop");
			ai->addChild(aiop);
		}
	}
	this->scheduleUpdate();
	if(gameMode)this->schedule(schedule_selector(GameScene::SpawnItems), 3.0f);
}
void GameScene::onExit() {
	Layer::onExit();
	endThread = 1;
}
void GameScene::update(float delta)
{
	if (!gameMode&&isNewProp)SpawnItems(propPos);
	MoveMap();
	PickMapItems();
	Collision();
	MovePlayer();
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
	int type = rand()%14;
	if (type > 6)type = 6;
	auto items = Sprite::create(settings::weapon_paths[type]);
	items->setTag(type);
	items->setPosition(x, y);
	auto jump = JumpBy::create(0.5f,Vec2(0,10),10,1);
	auto seq = Sequence::create(jump, jump->reverse(), nullptr);
	items->runAction(RepeatForever::create(seq));
	MapItems->addObject(items);
	this->Itemset->addChild(items);
}
void GameScene::SpawnItems(Vec3 pos)
{
	isNewProp = false;
	float x = pos.x,y=pos.y;
	int type = static_cast<int>(pos.z);
		auto git = Meta->getTileGIDAt(Vec2(static_cast<int>(floor(x / 32)), static_cast<int>(39 - floor(y / 32))));
		auto value = tileMap->getPropertiesForGID(git);
		auto valueMap = value.asValueMap();
		if (!valueMap.at("Collidable").asBool())
		{
			auto items = Sprite::create(settings::weapon_paths[type]);
			items->setTag(type);
			items->setPosition(x, y);
			auto jump = JumpBy::create(0.5f, Vec2(0, 10), 10, 1);
			auto seq = Sequence::create(jump, jump->reverse(), nullptr);
			items->runAction(RepeatForever::create(seq));
			MapItems->addObject(items);
			this->Itemset->addChild(items);
		}
}
void GameScene::MoveMap()
{
	auto player = tileMap->getChildByName("Player");
	auto playerPosition = player->getPosition();
	auto visiblesize = Director::getInstance()->getOpenGLView()->getVisibleSize();

	if (playerPosition.x > visiblesize.width / 2 && playerPosition.x < (1280 - visiblesize.width / 2))
		tileMap->setPositionX(visiblesize.width / 2 - playerPosition.x);
	if (playerPosition.y > visiblesize.height / 2 && playerPosition.y < (1280 - visiblesize.height / 2))
		tileMap->setPositionY(visiblesize.height / 2 - playerPosition.y);
}

bool GameScene::isAccessable(Point Position, int Direction)
{
	if (Direction == -1)return false;
	if (Position.x - 9 <=0 || Position.y - 24 <=0 || Position.x + 9 >= 1280 || Position.y + 18 >= 1280)
		return true;
	int dir[8][2] = { {9,0},{0,-24},{-9,0},{0,18},{9,-24},{-9,-24},{-9,18},{9,18}};
	auto git = Meta->getTileGIDAt(Vec2(static_cast<int>(floor((Position.x + dir[Direction][0]) / 32)), static_cast<int>(39 - floor((Position.y + dir[Direction][1]) / 32))));
	auto value = tileMap->getPropertiesForGID(git);
	auto valueMap = value.asValueMap();
	if (valueMap.at("Collidable").asBool())return true;
	for (int i=0; i < 8; i++)
	{
		auto git = Meta->getTileGIDAt(Vec2(static_cast<int>(floor((Position.x + dir[i][0]) / 32)), static_cast<int>(39 - floor((Position.y + dir[i][1]) / 32))));
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
		auto player =dynamic_cast<Player*>(iplayer);
		auto playerZone = CCRectMake(player->getPositionX() - 9, player->getPositionY() - 24, 18, 42);
		CCARRAY_FOREACH(MapItems, imapitem)
		{
			Sprite* mapitem = dynamic_cast<Sprite*>(imapitem);
			auto mapitemZone = CCRectMake(mapitem->getPositionX(), mapitem->getPositionY(), mapitem->getContentSize().width, mapitem->getContentSize().height);

			if (mapitemZone.intersectsRect(playerZone))
			{
				auto type = dynamic_cast<Sprite*>(imapitem)->getTag();
				if (type != 6)
				{
					player->WeaponType = type;
					player->scheduleUpdate();
					if (player->IsAI)
						(player->getChildByName("aiop"))->scheduleOnce(schedule_selector(AiPlayer::AiAttack), player->AttackSpeed);
				}
				else 
				{
				;
					if (0< player->HP <= 80)player->HP += 20;
					else if (player->HP > 80)player->HP = 100;
				}
				mapitems->addObject(imapitem);
			}
		}
	}

	CCARRAY_FOREACH(mapitems, imapitem)
	{
		MapItems->removeObject(imapitem);
		auto mapitem = dynamic_cast<Sprite*>(imapitem);
		mapitem->removeFromParentAndCleanup(TRUE);
	}

}

void GameScene::Collision()
{
	auto players = Array::create();
	players->retain();
	auto bullets = Array::create();
	bullets->retain();
	Object*iplayer;
	Object*iplayer2;
	Object*ibullet;


	CCARRAY_FOREACH(Players, iplayer)
	{
		auto player = dynamic_cast<Player*>(iplayer);
		auto playerZone = CCRectMake(player->getPositionX() - 9, player->getPositionY() - 24, 18, 42);
		CCARRAY_FOREACH(Bullets, ibullet)
		{
			auto bullet = dynamic_cast<Sprite*>(ibullet);
			auto bulletZone = bullet->boundingBox();
			if (bulletZone.intersectsRect(playerZone))
			{
				if (dynamic_cast<Sprite*>(ibullet)->getTag() != dynamic_cast<Player*>(iplayer)->getTag())
				{
					dynamic_cast<Player*>(iplayer)->HP -= 10;
					bullets->addObject(bullet);
				}
			}
			else if(bullet->getName()== "Arrow" || bullet->getName() == "Bubble")
			{
				int flag = 0;
				auto bulletPosition = bullet->getPosition();
				if (bulletPosition.x <= 0 || bulletPosition.y <= 0 || bulletPosition.x >= 1280 || bulletPosition.y >= 1280)
					flag = 1;
				else
				{
					auto git = Meta->getTileGIDAt(Vec2(static_cast<int>(floor((bulletPosition.x) / 32)), static_cast<int>(39 - floor((bulletPosition.y) / 32))));
					auto value = tileMap->getPropertiesForGID(git);
					auto valueMap = value.asValueMap();
					flag = valueMap.at("Collidable").asBool();
				}
				if (flag)
				{
					bullets->addObject(bullet);
				}
			}
		}
		if (player->weapon && (player->weapon->MyWeapon->getOpacity()) && (player->WeaponType == 1 || player->WeaponType == 4|| player->WeaponType == 5))
		{

			auto tmpZone = (player->weapon->MyWeapon)->boundingBox();
			auto weaponZone = CCRectMake(tmpZone.origin.x + player->getPositionX(), tmpZone.origin.y + player->getPositionY(), tmpZone.size.width, tmpZone.size.height);;
			CCARRAY_FOREACH(Players, iplayer2)
			{
				auto player2 =dynamic_cast<Player*>(iplayer2);
				if (player != player2)
				{
					auto playerZone = CCRectMake(player2->getPositionX() - 9, player2->getPositionY() - 24, 18, 42);
					if (weaponZone.intersectsRect(playerZone))
					{
						player2->HP -=0.01;
					}
				}
			}
		}
	}

	CCARRAY_FOREACH(bullets, ibullet)
	{
		auto bullet = dynamic_cast<Sprite*>(ibullet);
		auto bulletName=bullet->getName();
		int i = 0;
		if ((++i&&bulletName=="Arrow")|| (++i&&bulletName == "Starlight")||(++i&&bulletName=="Bubble"))
		{
			Bullets->removeObject(bullet);
			bullet->stopActionByTag(100);
			auto weapon = Weapon::create();
			(weapon->*(weapon->BulletEnd[i-1]))(bullet);
		}
		else Bullets->removeObject(bullet);
	}
	CCARRAY_FOREACH(Players, iplayer)
	{
		auto player = dynamic_cast<Player*>(iplayer);
		player->HPBar->setHp(player->HP);
		player->HPBar->setPosition(player->getPosition() + Vec2(0, 35));
		if (player->HP <= 0)
		{
			player->Dead(player);
			players->addObject(player);
		}

	}
	CCARRAY_FOREACH(players, iplayer)
	{
		
		if (!gameMode) {
			auto player = dynamic_cast<Player*>(iplayer);
			char buffer[MSGSIZE];
			int id = player->getId();
			sprintf(buffer, "d$%d", id);
			client->Send(buffer, MSGSIZE);
		}
		Players->removeObject(iplayer);
	}
	if (Players->count() == 1) {
		if (dynamic_cast<Player*>(Players->getLastObject())->getName() != "Player") {
			isWin = false;
		}
		dynamic_cast<Player*>(Players->getLastObject())->HPBar->removeFromParentAndCleanup(true);
		auto cl = LayerColor::create(Color4B::BLACK);
		cl->setOpacity(150);
		cl->setName("solid");
		addChild(cl);

		auto layer = ResultScene::create();
		layer->setTag(1);
		addChild(layer);
		unscheduleUpdate();
	}
}
void GameScene::exitCallback(Ref*ref) {

	MenuItemLabel* item = (MenuItemLabel*)ref;
	auto ai = static_cast<AiTest*>(tileMap->getChildByName("aitest"));
	char buffer[MSGSIZE];
	if (item->getString() == "up") {
		if (!ai->isUp) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 2);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
			ai->isUp = true;
		}
		else {
			ai->isUp = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 2);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
		}
	}
	else if (item->getString() == "down") {
		if (!ai->isDown) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 3);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
			ai->isDown = true;
		}
		else {
			ai->isDown = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 3);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
		}
	}
	else if (item->getString() == "right") {
		if (!ai->isRight) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 1);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
			ai->isRight = true;
		}
		else {
			ai->isRight = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 1);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
		}
	}
	else if (item->getString() == "left") {
		if (!ai->isLeft) {
			sprintf(buffer, "%c$%d$%d", KeyPress, 2, 0);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
			ai->isLeft = true;
		}
		else {
			ai->isLeft = false;
			sprintf(buffer, "%c$%d$%d", KeyRelease, 2, 0);
			static_cast<OPOperator*>(ai->getChildByName("op"))->KeyStart(buffer);
		}
	}
	
}
void GameScene::MovePlayer() {
	
	if (!cmdList.empty()) {
		
		std::list<std::string>::iterator it = cmdList.begin();
		auto buffer = (*it).c_str();
		
		if (buffer[0] == KeyPress || buffer[0] == KeyRelease) {
			int id = static_cast<int>(buffer[2]) - 48;
			if (id != local_Id) {/*
				auto vc = GameScene::getCurrentMap()->getChildren();
				auto player = static_cast<Player*>(GameScene::getCurrentMap()->getChildByTag(id+10));
				auto cv=player->getChildren();
				auto op = static_cast<OPOperator*>(player->getChildByName("op"));
				op->KeyStart(buffer);*/
				dynamic_cast<OPOperator*>(
					dynamic_cast<Player*>(
						GameScene::getCurrentMap()->getChildByTag(id + 10))->getChildByName("op"))->KeyStart(buffer);
			}	
		}
		else if (buffer[0] == MousePress || buffer[0] == MouseRelease) {
			int id = static_cast<int>(buffer[2]) - 48;
			if (id != local_Id)
				dynamic_cast<OPOperator*>(
					dynamic_cast<Player*>(
						GameScene::getCurrentMap()->getChildByTag(id+10))->getChildByName("op"))->TouchStart(buffer);
		}
		cmdList.pop_front();
	}
}