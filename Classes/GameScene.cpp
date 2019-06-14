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
		std::string name = playerList[i]->getName();
		int id = playerList[i]->getId();
		//auto label = Label::create(name, "arial.ttf", 15);
		auto player = Player::create(name, id);
		player->setPosition(w*(i + 1), h*(i + 1));
		//label->setPosition(0, 40);
		//player->addChild(label);
		player->setTag(player->getId());
		this->addChild(player);
	}
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

