#include "Game.h"
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>





Game::Game(QWidget *parent)
    : QWidget(parent),
    state(GameState::Waiting)
{
    level = new Level(1, nullptr, this);
    score(100),
    life(3),
    level(nullptr)

        {
        
    setFocusPolicy(Qt::StrongFocus);

    level = new Level(1, nullptr, this);
// Question 
    connect(level, &Level::levelCompleted,
            this, &Game::onLevelCompleted);
}

}

void Game::start()
{
    state = GameState::Playing;
    update();
}

void Game::restart()
{
    score.reset();          
    life = Life(3);         
    state = GameState::Waiting;
    update();
}


void Game::end()
{
    state = GameState::GameOver;
    update();
}

void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    switch (state) {
    case GameState::Waiting:
        p.drawText(rect(), Qt::AlignCenter, "Press Enter to start");
        p.drawText(10, 20, QString("Score: %1").arg(score.points()));
        p.drawText(10, 40, QString("Lives: %1").arg(life.getLife()));
        break;

    case GameState::Playing:
        p.drawText(rect(), Qt::AlignCenter, "Game is running");
        break;

    case GameState::Paused:
        p.drawText(rect(), Qt::AlignCenter, "Paused");
        break;

    case GameState::LevelComplete:
        p.drawText(rect(), Qt::AlignCenter, "Level Complete!");
        break;

    case GameState::GameOver:
        p.drawText(rect(), Qt::AlignCenter, "Game Over");
        break;



    }

}


Void Game::onLevelCompleted(int levelNumber)
{
Q_UNUSED(levelNumber);          
    state = GameState::LevelComplete;
    update();
    
}

//     Detected with the help of AI

void Game::keyPressEvent(QKeyEvent *event)
{
    if (state == GameState::Waiting &&
        (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        start();
    }
    else if (state == GameState::Playing &&
             event->key() == Qt::Key_Escape) {
             level->completeLevel();
            end();
    }

    QWidget::keyPressEvent(event);
}






