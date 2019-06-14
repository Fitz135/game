#include"cocos2d.h"
#include"Entity.h"

class Prop：public Entity
{
public:
    enum class Type
    {
        EMPTY = 0,
        hp= ,
        exp=
    };
    
    static Prop * create(const std::string& id, Type type);
    
    bool init(Type type);
    
    int getType();
    
private:
    Type type;
};
