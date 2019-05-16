//
//  ModeSelectScene.cpp
//  MyCppGame
//
//  Created by  周子龙 on 2019/5/16.
//

#include "ModeSelectScene.h"
#include "SimpleAudioEngine.h"
#include "Settings.h"

USING_NS_CC;
/*
Scene* ModeSelectScene::createScene()
{
    auto scene = Scene::create();
 
    auto layer = ModeSelectScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool ModeSelectScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    createBackground();
    createButton();
    
    //initUserBox();
    
    return true;
}

void ModeSelectScene::createBackground()
{
    auto bg = Sprite::create("ModeSelectScene/bg.png");
    bg->setScale(1.6f);
    bg->setLocalZOrder(-1);
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
                         origin.y + visibleSize.height / 2));
    this->addChild(bg);
}
*/
