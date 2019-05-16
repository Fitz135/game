#include "LoginScene.h"
#include "Settings.h"
#include "ModeSelectScene.h"
//#include "Scene/UI/CheckBox.h"
//#include "Model/User.h"
//#include "Util/GameAudio.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = LoginScene::create();

    scene->addChild(layer);

    return scene;
}

bool LoginScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto playLabel = Label::createWithTTF("PLAY", Settings::Font::Type::base, Settings::Font::Size::normal);
    playLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(playLabel,1);
    
    Sprite *bg = Sprite::create("LoginScene/bg.png");
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
                         origin.y + visibleSize.height / 2));
    this->addChild(bg,-1);

    
    auto closeItem = MenuItemImage::create(
                                           "LoginScene/PlayNormal.png",
                                           "LoginScene/PlaySelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    return true;
}
