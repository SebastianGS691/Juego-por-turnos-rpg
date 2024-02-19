#include <iostream>
#include "Enemy/Enemy.h"
#include "Player/Player.h"

using namespace std;

void Combate(Player *player, Enemy *enemy){
  while(player->getHealth() > 0 && enemy->getHealth() > 0) {
    player->doAttack(enemy);
    if (enemy->getHealth() <= 0) break;
    enemy->doAttack(player);
    cout<<"---------------------------------------"<< endl << endl;
  }
}

int main() {

    Player *player = new Player("Victor", 100, 10, 5, 5);
    Enemy *enemy = new Enemy("Enemigo", 100, 6, 4, 3);

    cout<<"\t-- Inicio de combate --"<< endl << endl;
    Combate(player, enemy);
    getchar();
  
    cout<< player->toString()<< endl << endl;
    cout<< enemy->toString()<< endl;
  
    return 0;
}
