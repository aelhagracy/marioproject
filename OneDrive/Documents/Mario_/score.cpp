#include "score.h"

Score::Score(QGraphicsItem* parent)
    : QGraphicsTextItem(parent), score(0)
{
    setPlainText("Score: 0");
    setDefaultTextColor(Qt::white);
    setFont(QFont("Arial", 24));
}

void Score::increase(int amount)
{
    score += amount;
    setPlainText(QString("Score: %1").arg(score));
}

void Score::setScore(int value)
{
    score = value;
    setPlainText(QString("Score: %1").arg(score));
}

int Score::getScore() const
{
    return score;
}

void Score::reset()   // <-- implementation
{
    score = 0;
    setPlainText("Score: 0");
}
