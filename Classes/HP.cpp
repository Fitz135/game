#include"HP.h"
USING_NS_CC;
bool Hp::init() {
	if (!Layer::create()) {
		return false;
	}
	loadingbar = Sprite::create("UI/loadingbar.png");
	HpBar = ui::LoadingBar::create("UI/Hp.png",100.0f);
	this->addChild(loadingbar);
	this->addChild(HpBar);
	return true;
}
void Hp::setHp(float hp) {

		HpBar->setPercent( hp);

}