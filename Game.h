#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "Gamestate.h"

#include "Score.h"
#include "Life.h"
#include "Level.h"


class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);

    void start();
    void end();

protected:
    void paintEvent(QPaintEvent *event) override;
// Add this
    void keyPressEvent(QKeyEvent *event) override;


private:
    GameState state;
    Score score;
    Life  life;
    Level *level;
};

#endif // GAME_H
