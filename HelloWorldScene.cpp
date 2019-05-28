/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
int ud = 1;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void shaking(Ref* ref) {
	MenuItemFont* item = (MenuItemFont*)ref;
	item->setColor(Color3B::RED);
	auto player = Director::getInstance()->getRunningScene()->getChildByTag(1);
	
	player->setPosition(player->getPosition().x, player->getPosition().y + ud * 20);
	ud *= -1;
}
// on "init" you need to initialize your instance
void AddNero(Ref* ref) {
	Label* item = (Label*)ref;
	item->setColor(Color3B::RED);
	auto player = Sprite::create("Target.png");
	player->setPosition(rand() % 400, rand() % 200);
	Director::getInstance()->getRunningScene()->addChild(player);
}
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	auto label_add_child = Label::create("Add a nero", "arial.ttf", 24);
	label_add_child->setColor(Color3B(0,0,0));
	auto label_move_child = Label::create("Shaking it", "arial.ttf", 24);
	label_move_child->setColor(Color3B(0, 0, 0));
	auto item_add_child = MenuItemLabel::create(label_add_child, AddNero);
	auto item_move_child = MenuItemFont::create("Shaking it", shaking);
	item_move_child->setColor(Color3B(0, 0, 0));
	auto menu = Menu::create();
	menu->addChild(item_add_child);
	menu->addChild(item_move_child);
	menu->alignItemsVerticallyWithPadding(40);
	//item_add_child->setPosition(item_add_child->getContentSize().width / 2, item_add_child->getContentSize().height + 20);
	//item_move_child->setPosition(item_add_child->getContentSize().width / 2, item_add_child->getContentSize().height - 20);

	this->addChild(menu,1);
	

    /////////////////////////////
    // 3. add your codes below...


    // add "HelloWorld" splash screen"
	auto player = Sprite::create("Target.png");
    if (player== nullptr)
    {
        problemLoading("Player.png'");
    }

    else
    {
        // position the sprite on the center of the screen
        player->setPosition(Vec2(player->getContentSize().height/2 + origin.x, visibleSize.height/2 + origin.y));
		player->setTag(1);
        // add the sprite as a child to this layer
        this->addChild(player,2);
    }

	//this->schedule(schedule_selector(shaking));
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
