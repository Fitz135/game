#include"GameScene.h"
#include "Player.h"
#include "Operator.h"
#include "Settings.h"
#include"OPOperator.h"
#include"thread"
USING_NS_CC;
static Player* player;
static bool endThread;
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

static void getMsg(ODSocket* client) {
	char buffer[MSGSIZE];
	while (!endThread) {
		if (client == NULL) {
			log("null client");
			break;
		}
		client->Recv(buffer, MSGSIZE);
		switch (buffer[0]) {
		case NewPlayer:
			case KeyPress:
			case KeyRelease:dynamic_cast<OPOperator*>(player->getChildByName("op"))->KeyStart(buffer); break;
		}
	}
}
bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	

	auto w = Director::getInstance()->getWinSize().width / (players + 1);
	auto h = Director::getInstance()->getWinSize().height / (players + 1);


	/*for (int i = 0; i < players; i++) {
		auto label = Label::create(playerList[i]->getName(), "arial.ttf", 15);
		playerList[i]->setPosition(w*(i + 1), h*(i + 1));
		label->setPosition(0, 40);
		playerList[i]->addChild(label);
		this->addChild(playerList[i]);
	}*/

	player = Player::create(local_username, local_Id);
	auto op = OPOperator::create();
	player->setName("local");
	op->setName("op");
	player->addChild(op);
	this->addChild(player);
	
	/*auto operate = Operator::create();
	addChild(operate);*/
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

