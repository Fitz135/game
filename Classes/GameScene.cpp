#include"GameScene.h"
//#include "Player.h"
#include "Operator.h"
#include"Weapon.h"
#include "Settings.h"
#include"OPOperator.h"
#include"thread"
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
	//auto tileMap = scene->getChildByName("Map");
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
	BG = tileMap->getLayer("Ground");
	twall = tileMap->getLayer("Wall");
	Meta = tileMap->getLayer("Meta");
	tileMap->setName("Map");

	this->addChild(tileMap);
	auto w = Director::getInstance()->getWinSize().width / (players + 1);
	auto h = Director::getInstance()->getWinSize().height / (players + 1);

	posList.push_back(Vec2(0, 0));
	posList.push_back(Vec2(100, 300));
	posList.push_back(Vec2(300, 100));
	posList.push_back(Vec2(300, 300));

	for (int i = 0; i < players; i++) {
		std::string name(playerList[i]->_name);
		int id = playerList[i]->_id;

		auto player = Player::create(name, id);
		player->setPosition(posList[id]);
		if (id != local_Id) {
			player->setTag(player->getId());
			auto op = OPOperator::create();
			op->setName("op");
			player->addChild(op);
		}
		tileMap->addChild(player);
		Players->addObject(player);
	}

	auto sp = Sprite::create("KnightStand1.png");
	sp->setScale(0.5);
	sp->setName("test");
	tileMap->addChild(sp);
	sp->setPosition(100,100);
	sp->setZOrder(1);
	Players->addObject(sp);
	this->scheduleUpdate();

}
void GameScene::onExit() {
	Layer::onExit();
	endThread = 1;
}
void GameScene::update(float delta)
{
	
	this->MapMove();
	//this->IsWeaponIntoPlayer();
	this->PickMapItems();
	this->IsBulletIntoWall();
	//this->IsBulletIntoPlayer();
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
/*void GameScene::IsWeaponIntoPlayer()
{
	auto players = Array::create();
	players->retain();
	auto weapons = Array::create();
	weapons->retain();
	Object*iplayer;
	Object*iweapon;

	CCARRAY_FOREACH(Players, iplayer)
	{
		Sprite* player = (Sprite*)iplayer;
		auto playerZone = CCRectMake(player->getPositionX() - 9, player->getPositionY() - 24, 18, 42);
		CCARRAY_FOREACH(Weapons, iweapon)
		{
			Sprite* weapon = (Sprite*)iweapon;
			auto weaponZone = weapon->boundingBox();
			if (weaponZone.intersectsRect(playerZone))
			{
				if (((Sprite*)iweapon)->getName() != ((Player*)iplayer)->getName())
				{
					log("ys");
					players->addObject(iplayer);
					weapons->addObject(iweapon);
				}
			}
		}
	}
}*/
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
					players->addObject(iplayer);
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
				players->addObject(iplayer);
				((Player*)iplayer)->WeaponType = 1;
				((Player*)iplayer)->scheduleUpdate();
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



////////AI need//////////
bool GameScene::isInMap(const cocos2d::Vec2& pos) {
	auto mapSize = tileMap->getMapSize();
	auto tileSize = tileMap->getTileSize();
	return 0 <= pos.x && pos.x < mapSize.width * tileSize.width
		&& 0 <= pos.y && pos.y < mapSize.height * tileSize.height;
}

Vec2 GameScene::positionToTileCoord(const cocos2d::Vec2 & pos)
{
	cocos2d::Size mapSize = tileMap->getMapSize();
	cocos2d::Size tileSize = tileMap->getTileSize();

	int x = pos.x / tileSize.width;
	int y = (mapSize.height * tileSize.height - pos.y) / tileSize.height;
	// edge case
	x = MIN(MAX(0, x), mapSize.width - 1);
	y = MIN(MAX(0, y), mapSize.height - 1);
	//CCLOG("coord x: %f y: %f", x, y);
	return Vec2(x, y);
}
Vec2 GameScene::tileCoordToPosition(const cocos2d::Vec2 & coord)
{
	auto mapSize = tileMap->getMapSize();
	auto tileSize = tileMap->getTileSize();

	int x = coord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - coord.y) * tileSize.height - tileSize.height / 2;
	//CCLOG("pos x: %f y: %f", x, y);
	return Vec2(x, y);
}