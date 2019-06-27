#include "Prop.h"
#include "Settings.h"

Prop * Prop::create(const std::string& id,Type type)
{
    auto prop = new(std::nothrow) Prop();
    if(prop&&prop->init(type))
    {
        prop->_id = id;
        prop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        prop->autorelease();
        return prop;
    }
    return nullptr;
}

bool Prop::init(Type type)
{
    if(type==Type::EMPTY)
    {
        return false;
    }
    this->type=type;
    std::string path="需要填写";
    path +=Settings::Prop::name[getType()-(需要填写)];
    if (!initWithFile(path))
    {
        return false;
    }
    return true;
}

int Prop::getType()
{
    return type;
}
