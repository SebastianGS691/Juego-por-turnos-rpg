//
// Created by Victor Navarro on 13/02/24.
//
#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define RED "\033[31m"
#include "Enemy.h"
#include <iostream>

using namespace std;

//TODO: Check the circular dependency
int getRolledAttack(int attack) {
    int lowerLimit = attack * .80;
    return (rand() % (attack - lowerLimit)) + lowerLimit;
}

Enemy::Enemy(char name[], int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, false) {
};

void Enemy::flee() {
    this->fleed = true;
}

void Enemy::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Enemy::takeDamage(int damage) {
    setHealth(getHealth() - damage);
    if(getHealth() <= 0) {
        cout<<GREEN<<getName()<<" has died"<<RESET<< endl;
    }
    else {
        cout<<getName()<<" has taken " << damage << " damage" << endl;
    }
}
void Enemy::levelup() {
    int hppls = this->getHealth() * .15;
    int atkpls = this->getAttack() * .20;
    int defpls = this->getDefense() * .10;
    int spdpls = this->getSpeed() * .6;
    this->setLevel(getLevel() + 1);
    this->setHealth(getHealth() + hppls);
    this->setAttack(getAttack() + atkpls);
    this->setDefense(getDefense() + defpls);
    this->setSpeed(getSpeed() + spdpls);


    cout << RED << "\t The enemy: " << getName() << getLevel() << RESET << endl;
    cout << "\n\n";
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
//Accion de ataque del enemigo
Action Enemy::takeAction(vector<Player *> player) {
    Action myAction;
    myAction.speed = getSpeed();
    myAction.subscriber = this;
	Character* target = getTarget(player);
    if (getHealth() < maxHealth * 0.60 && rand() % 100 < 50) { //Cuando tiene menos del 60% de vida hay un 50% de que huya
		myAction.action = [this, player]() {
			flee();
		};
    }
    else {
		myAction.target = target;
		myAction.action = [this, target]() {
            doAttack(target);
		};
    }

    return myAction;
}

//samlo estuvo aqui >:)