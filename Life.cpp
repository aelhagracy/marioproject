#include "Life.h"
#include <iostream>
using namespace std;


Life::Life(int startingLives)
    : lives(startingLives){}


int Life::getLife() {
    ++lives;
    return lives;
}

int Life::loseLife() {
    if (lives > 0) {
        --lives;

    }

    if (lives == 0) {
        cout << "You died! Game Over." << endl;

    }
    return lives;

}

bool Life::isLife() const
{
    return lives > 0;
}


