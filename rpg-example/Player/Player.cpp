//
// Created by Victor Navarro on 13/02/24.
//
#include "Player.h"
#include <iostream>
#include <algorithm>
#include "../Utils.h"

using namespace std;
using namespace combat_utils;

bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

Player::Player(string name, int health, int attack, int defense, int speed) : Character(name, health, attack, defense, speed, true) {
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
    bool shit = false;
    if (damage >= 5) {
        shit = true;
    }
    if(health <= 0) {
        cout<<"You have died, You are a really Crap"<<endl;
    }
    else {
        if (shit = false) {
            cout << "You have taken " << damage << " damage" << endl;
        }
        if (shit = true) {
            cout << "You have taken" << damage << " damage" << endl;
        }
    }
}

bool Player::flee(vector<Enemy*> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy* fastestEnemy = enemies[0];
    bool fleed = false;
    if(this->getSpeed() > fastestEnemy->getSpeed()) {
        fleed =  true;
    }
    else {
        srand(time(NULL));
        int chance = rand() % 100;
        cout<< "chance: " << chance << endl;
        fleed = chance > 99;
    }

    return fleed;
}

void Player::emote() {
    cout<<"Jokes on you" << endl;
}

Character* Player::getTarget(vector<Enemy*> enemies) {
    int targetIndex = 0;
    while (true) {
        cout << "Choose a target" << endl;
        for (int i = 0; i < enemies.size(); i++) {
            cout << i << ". " << enemies[i]->getName() << endl;
        }
        cin >> targetIndex;
        if (targetIndex >= enemies.size() || targetIndex < 0) {
            cout << " Invalid Option" << endl;
        }
        else break;
    }   
    return enemies[targetIndex];
}

ActionResult Player::takeAction(vector<Enemy*> enemies) {
    int option = 0;
    bool oks = false;
    Character* target = nullptr;
    bool fleed = false;
    do {
        cout << "Choose an action" << endl;
        cout << "1. Attack" << endl;
        cout << "2. Flee" << endl;
        cin >> option;
        switch (option) {
        case 1:
            target = getTarget(enemies);
            doAttack(target);
            oks = true;
            break;
        case 2:
            fleed = flee(enemies);
            if (fleed) {
                cout << "You have fled" << endl;
            }
            else {
                cout << "You couldn't flee" << endl;
            }
            oks = true;
            break;
        default:
            cout << "Invalid option" << endl;
            break;
        }
    } while (oks == false);
    return ActionResult(target, fleed);
}