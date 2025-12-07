#include "life.h"

Life::Life(QGraphicsItem* parent, int startingLives)
    : QGraphicsTextItem(parent), lives(startingLives), maxLives(startingLives)
{
    setFont(QFont("Arial", 16));
    setDefaultTextColor(Qt::red);
    updateText();
}

void Life::decrease()
{
    if (lives > 0) {
        --lives;
        updateText();
    }
}

void Life::reset()
{
    lives = maxLives;
    updateText();
}

int Life::getLives() const
{
    return lives;
}

void Life::updateText()
{
    setPlainText(QString("Lives: %1").arg(lives));
}
