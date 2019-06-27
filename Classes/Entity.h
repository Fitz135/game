#ifndef _ENTITY_H_
#define _ENTITY_H_



#include <string>

class Entity 
{
public:
    Entity(std::string , int ) ;
    ~Entity() = default;
    
	static Entity* create(std::string, int);
    int getId() const;
	std::string getName();

	std::string _name;
    int _id;
};
#endif // !_ENTITY_H_