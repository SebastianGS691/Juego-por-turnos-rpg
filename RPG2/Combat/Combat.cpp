//
// Created by Victor Navarro on 19/02/24.
//
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#include "Combat.h"
#include <iostream>
#include <algorithm>

using namespace std;

//comparador de speed
bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

// ----- INICIA CONSTRUCTORES ---- //
Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant: participants) {
        if(participant->getIsPlayer()) {
            teamMembers.push_back((Player*)participant);
        }
        else {
            enemies.push_back((Enemy*)participant);
        }
    }
}

Combat::Combat(vector<Player*> _teamMembers, vector<Enemy*> _enemies) {
    teamMembers = std::move(_teamMembers);
    enemies = std::move(_enemies);
}

Combat::Combat() {
    participants = vector<Character*>();
}
// ----- TERMINA CONSTRUCTORES ---- //


//funcion no usada
void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()) {
        teamMembers.push_back((Player*)participant);
    }
    else {
        enemies.push_back((Enemy*)participant);
    }
}

//Ordena los participantes por su velocidad
void Combat::prepareCombat() {
    sort(participants.begin(), participants.end(), compareSpeed);
}

// -- INICIA EL COMBATE -- // -- doCombat
void Combat::doCombat() {
    prepareCombat();

    //Este while es 1 iteracion por ronda
    while(enemies.size() != 0 && teamMembers.size() != 0) {
        //Itera a traves de todo (Character)participants guardando las acciones en actions
        registerActions();
        //Executar las acciones
        executeActions();
    }

    //No se imprime el nombre del ganador
    if(enemies.size() == 0) {
        cout<<GREEN<<"You have won the combat"<<RESET<<endl;
    }
    else {
        cout<<RED<<"The enemies have won the combat - Game Over"<<RESET<<endl;
    }
}

//Carga todas las acciones en (actions)
void Combat::registerActions() {
    //itera a traves de participants
    vector<Character*>::iterator participant = participants.begin();
    //Una iteracion por turno de cada participante (player y enemigo)
    while(participant != participants.end()) {
        //Character* target = nullptr; no hace nada (posible codigo residual??)

        Action currentAction; // variable: accion actual
        if((*participant)->getIsPlayer()) { //es jugador?
			// accion actual = (Jugador)->takeAction
        currentAction = ((Player*)*participant)->takeAction(enemies);
        }
        else { //es enemigo?
            // accion actual = (Enemigo)->takeAction
            currentAction = ((Enemy*)*participant)->takeAction(teamMembers);
        }
        actions.push(currentAction); //enviar accion actual a (priority_queue)actions
        participant++;
    }
}

void Combat::executeActions() {
    //Aqui se ejecutan las acciones
    while(!actions.empty()) {
        Action currentAction = actions.top();
        currentAction.action();
        checkForFlee(currentAction.subscriber);
        if (currentAction.target != nullptr) {
            //si huyo
			checkParticipantStatus(currentAction.subscriber);
			checkParticipantStatus(currentAction.target);
			actions.pop();
        }
        else {
            //si no huyo
			while (!actions.empty()) {
				actions.pop();
			}
        }
    }
}

void Combat::checkParticipantStatus(Character* participant) {
    if(participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            teamMembers.erase(remove(teamMembers.begin(), teamMembers.end(), participant), teamMembers.end());
        }
        else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::checkForFlee(Character *character) {
    bool fleed = character->hasFleed();
    if(character->getHealth() > 0 && fleed) {
        //si es jugador?
        if(character->getIsPlayer()) {
            cout<<"You have fled the combat"<<endl;
            //Borra el jugador de teamMembers
            teamMembers.erase(remove(teamMembers.begin(), teamMembers.end(), character), teamMembers.end());
        }
        //si es enemigo?
        else {
            cout<<character->getName()<<" has fled the combat"<<endl;
            //Borra el enemigo de enemies
            enemies.erase(remove(enemies.begin(), enemies.end(), character), enemies.end());
        }
        //
        participants.erase(remove(participants.begin(), participants.end(), character), participants.end());
    }
}

string Combat::participantsToString() {
    string result = "";
    for (int i = 0; i < participants.size(); i++) {
        result += participants[i]->toString() + "\n";
    }
    return result;
}