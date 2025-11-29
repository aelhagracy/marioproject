#include "inputhandler.h"
#include "player.h"
#include <QKeyEvent>

InputHandler::InputHandler(Player* p) : player(p) {}

void InputHandler::keyPressed(int key)
{
    if (key == Qt::Key_A) player->moveLeft();
    if (key == Qt::Key_D) player->moveRight();
    if (key == Qt::Key_W) player->jump();
}

void InputHandler::keyReleased(int key)
{
    if (key == Qt::Key_A || key == Qt::Key_D) player->stop();
}
