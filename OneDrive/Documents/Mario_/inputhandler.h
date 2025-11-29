#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>

class Player;

class InputHandler : public QObject
{
    Q_OBJECT
public:
    InputHandler(Player* p);
    void keyPressed(int key);
    void keyReleased(int key);

private:
    Player* player;
};

#endif
