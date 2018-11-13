#include <model/gameobject.hpp>
#include <iostream>
#include <controller/controller.hpp>

using namespace std;

//this is the game object class
//in our game everything is a gameobject object so box, ball, line and platform inherit from this class
//so this class have general methods that creates the gameobject, destroys it, marks for deletion (used for bricks),
//applies deletions and sets objects IDs (we used objects IDs so we could see which object we constructed and destroyed)

int GameObject::totalObjects = 0;

GameObject::GameObject() {}

GameObject::~GameObject() {}

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

    lines.clear();

    lines = { 
        Line::getVertical(left, top, bottom, this->getId()), 
        Line::getVertical(right, top, bottom, this->getId()),
        Line::getHorizontal(top, left, right, this->getId()),
        Line::getHorizontal(bottom, left, right, this->getId())
    };
}


int GameObject::setId() {
    this->id = GameObject::totalObjects++;
    Controller::sendNewObject(*this);
    return this->id;
}
