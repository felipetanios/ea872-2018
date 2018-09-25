#include <model/gameobject.hpp>
#include <iostream>
using namespace std;

int GameObject::totalObjects = 0;

map<int, GameObject*> GameObject::gameObjects = {};
list<int> GameObject::toBeDeleted = {};

GameObject::GameObject() {
    this->setId();
}

GameObject::~GameObject() {
    cout << "destroying game object " << (int)this->id << endl;
    // while(!lines.empty()) delete lines.front(), lines.pop_front();
    // delete renderer;
    // delete GameObject::gameObjects[this->id];
}

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
        Line::getVertical(left, top, bottom, this->getId()), 
        Line::getVertical(right, top, bottom, this->getId()),
        Line::getHorizontal(top, left, right, this->getId()),
        Line::getHorizontal(bottom, left, right, this->getId())
    };
}

void GameObject::update() {}   

void GameObject::collide() {}    

void GameObject::markForDeletion() {
    this->deleted = true;
    GameObject::toBeDeleted.push_back(this->getId());
}

void GameObject::applyDeletions() {
    list<int>::iterator it1;
    for (it1 = GameObject::toBeDeleted.begin(); it1 != GameObject::toBeDeleted.end(); ++it1) {
        int index = *it1;
        map<int, GameObject*>::iterator it2;
        it2 = GameObject::gameObjects.find(index);
        GameObject::gameObjects.erase(it2);
    }
    GameObject::toBeDeleted.clear();
}

int GameObject::setId() {
    this->id = GameObject::totalObjects++;
    GameObject::gameObjects[this->id] = this;
}
