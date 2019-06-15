#include"GameScene.h"
#include "Player.h"
#include "Operator.h"
#include "Settings.h"
#include"OPOperator.h"
#include"thread"
#include"Settings.h"
USING_NS_CC;
//static Player* player;

Scene* GameScene::createScene() {
	

	auto layer = GameScene::create();
	//scene->retain();
	layer->setName("GameScene");
	layer->setTag(10);
	auto scene = Scene::create();
	scene->addChild(layer);

	return scene;
}

GameScene* GameScene::getCurrentMap() {
	auto currentScene = Director::getInstance()->getRunningScene();
	return dynamic_cast<GameScene*>(currentScene->getChildByName("GameScene"));
}


bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	
	auto w = Director::getInstance()->getWinSize().width / (players + 1);
	auto h = Director::getInstance()->getWinSize().height / (players + 1);

	posList.push_back(Vec2(100, 100));
	posList.push_back(Vec2(100, 300));
	posList.push_back(Vec2(300, 100));
	posList.push_back(Vec2(300, 300));
	
	for (int i = 0; i < players; i++) {
		std::string name (playerList[i]->_name);
		int id = playerList[i]->_id;
		
		auto player = Player::create(name, id );
		player->setPosition(posList[id]);
	
		if (id != local_Id) {
			player->setTag(player->getId());
			auto op = OPOperator::create();
			op->setName("op");
			player->addChild(op);
		}
		
		this->addChild(player);
	}



	/*auto player = Player::create(playerList[0]->_name , playerList[0]->_id+2);
	this->addChild(player);*/

	auto operate = Operator::create();
	addChild(operate);

	/*player = Player::create(local_username, local_Id);
	auto op = OPOperator::create();
	player->setName("local");
	op->setName("op");
	player->addChild(op);
	this->addChild(player);*/
	
	return true;
}

void GameScene::onEnter() {
	Layer::onEnter();
	client->Send("4", 1);//////
	endThread = 0;
	std::thread t(getMsg, client);
	t.detach();
}
void GameScene::onExit() {
	Layer::onExit();
	endThread = 1;
}

