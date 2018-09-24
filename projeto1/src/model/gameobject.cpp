#include <model/gameobject.hpp>

int GameObject::totalObjects = 0;

int GameObject::getTotalObjects() {
    return GameObject::totalObjects;
}

int GameObject::getId() {
    return this->id;
}
// basic boundaries for a box-shaped object
void GameObject::updateCollisionLogic() {
    left = x - width / 2;
    right = x + width / 2;
    top = y + height / 2;
    bottom = y - height / 2;

    while(!lines.empty()) delete lines.front(), lines.pop_front();

    lines = { 
        Line::getVertical(left, top, bottom), 
        Line::getVertical(right, top, bottom),
        Line::getHorizontal(top, left, right),
        Line::getHorizontal(bottom, left, right)
    };
}

void GameObject::update() {}    

int GameObject::setId() {
    this->id = GameObject::totalObjects++;
}