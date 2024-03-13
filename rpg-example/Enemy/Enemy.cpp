//
// Created by Victor Navarro on 13/02/24.
//
#include "Enemy.h"
#include <iostream>


using namespace std;

//TODO: Check the circular dependency
int maxHealth = 0;

int getRolledAttack(int attack) {
    int lowerLimit = attack * .80;
    return (rand() % (attack - lowerLimit)) + lowerLimit;
}

Enemy::Enemy(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
    maxHealth = health;
}

bool Enemy::flee() {
    cout << this->getName() << " The enemy has escaped " << endl;
    return true;
}

int Enemy::getMaxHealth (){
    return maxHealth;
}

void Enemy::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Enemy::takeDamage(int damage) {
    bool wow = false;
    if (damage >= 5){
        wow = true;
    }
    setHealth(getHealth() - damage);
    if(getHealth() <= 0) {
        if (wow = false) {
            cout << getName() << " has died" << endl;
        }
        if (wow = true) {
            cout << getName() <<" has died" << endl;
        }
    }
    else {
        if (wow = false) {
            cout << getName() << " has taken " << damage << " damage" << endl;
        }
        if (wow = true) {
            cout << getName() << " has taken " << damage << " damage" << endl;
        }
    }
}

Character* Enemy::getTarget(vector<Player *> teamMembers) {
    // Obtiene el miembro del equipo con menos vida
    int targetIndex = 0;
    int lowestHealth = INT_MAX;
    for(int i=0; i < teamMembers.size(); i++) {
        if(teamMembers[i]->getHealth() < lowestHealth) {
            lowestHealth = teamMembers[i]->getHealth();
            targetIndex = i;
        }
    }

    return teamMembers[targetIndex];
}

ActionResult Enemy::takeAction(vector<Player*> teamMembers) {
    Character* target = nullptr;
    bool fleed = false;
    if ((this->getMaxHealth() * 0.15 >= this->getHealth()) && rand() % 100 < 90) {
        fleed = flee();
    } else {
        target = getTarget(teamMembers);
        doAttack(target);
    }
    return ActionResult(target, fleed);
}
