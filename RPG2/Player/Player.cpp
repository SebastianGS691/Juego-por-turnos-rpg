//
// Created by Victor Navarro on 13/02/24.
//
#include "Player.h"
#include <iostream>
#include "../Utils.h"
#include <algorithm>
#define RED "\033[31m"
#define RESET "\033[0m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"

using namespace std;
using namespace combat_utils;

bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

Player::Player(char name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, true) {
    experience = 0;
    level = 1;
}

void Player::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Player::takeDamage(int damage) {
    setHealth(health - damage);
    cout << YELLOW << "You have taken " << damage << " damage" << RESET << endl;
    cout << GREEN << getName() << " has health points: " << getHealth() << RESET << endl;
    if (health <= 0) {
        cout<< RED << "You have died" << RESET << endl;
    }
}

//Decide si el jugador puede huir
void Player::flee(vector<Enemy *> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy *fastestEnemy = enemies[0];
    bool fleed = false;
    if (this->getSpeed() > fastestEnemy->getSpeed()) {
        fleed = true;
    } else {
        srand(time(NULL));
        int chance = rand() % 100;
        cout << "chance: " << chance << endl;
        fleed = chance > 50;
    }

    this->fleed = fleed;
}


//Funcion para que player suba nivel
void Player::levelUp() {
    level++;
    setHealth(getHealth() + 30);
    setAttack(getAttack() + 9);
    setDefense(getDefense() + 11);
    setSpeed(getSpeed() + 8);

    cout << GREEN << "\tLevel up: " << level << " " << RESET << endl;
    cout << "\n\n";
}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        int trash = experience - 100;
        levelUp();
        experience = trash;
    }
}

Character *Player::getTarget(vector<Enemy *> enemies) {
    cout << BLUE << "\t [Choose a target]" << RESET << endl;
    int targetIndex = 0;
    for (int i = 0; i < enemies.size(); i++) {
        cout << i+1 << ". " << enemies[i]->getName() << endl;
    }
    cin >> targetIndex;
    //TODO: Add input validation
    return enemies[targetIndex-1];
}

Action Player::takeAction(vector<Enemy *> enemies) {
    int option = 0;
    cout << BLUE << "\t [Choose an action]" << RESET << endl;
    cout << "1. Attack" << endl;
    cout << "2. Flee" << endl;
    cin >> option;
    Character *target = nullptr;

    //Esta variable guarda
    //1. Que voy a hacer?
    //2. Con que velocidad/prioridad?
    Action myAction;
    //2.
    myAction.speed = this->getSpeed();
    myAction.subscriber = this;

    switch (option) {
        case 1:
            target = getTarget(enemies);
            myAction.target = target;
            //1.
            myAction.action = [this, target]() {
                doAttack(target);
            };
            break;
        case 2:
            myAction.action = [this, enemies]() {
                flee(enemies);
            };
            break;
        default:
            cout << RED << "Invalid option" << RESET << endl;
            break;
    }

    return myAction;
}