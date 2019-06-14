#include "Entity.h"

Entity::Entity(std::string name, int id) :_name (name),_id(id) {

}

Entity* Entity::create(std::string name, int id) {
	Entity* entity = new Entity(name, id);
	return entity;
}
int Entity::getId() const
{
    return _id;
};
std::string Entity::getName() {
	return _name;
}
