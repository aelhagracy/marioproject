#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QFont>

class Score : public QGraphicsTextItem
{
public:
    Score(QGraphicsItem* parent = nullptr);

    void increase(int amount = 1);
    void setScore(int value);
    int getScore() const;

private:
    int score;
};

#endif
