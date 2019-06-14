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
	auto scene = Scene::create();

	auto layer = GameScene::create();
	scene->retain();
	layer->setName("GameScene");
	layer->setTag(10);

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

	
	for (int i = 0; i < players; i++) {
		std::string name (playerList[i]->_name);
		int id = playerList[i]->_id;
		//auto label = Label::create(name, "arial.ttf", 15);
		
		auto player = Player::create(name, id );
		player->setPosition(w*(i + 1), h*(i + 1));
		//label->setPosition(0, 40);
		//player->addChild(label);
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

void GameScene::updatePlayer(char* buffer) {
	if (buffer[0] == KeyPress|| buffer[0] == KeyRelease) {
		int id= static_cast<int>(buffer[2]) - 48;
		if(id!=local_Id)
		dynamic_cast<OPOperator*>(
			dynamic_cast<Player*>(
				GameScene::getCurrentMap()->getChildByTag(id))->getChildByName("op"))->KeyStart(buffer);
	}
}