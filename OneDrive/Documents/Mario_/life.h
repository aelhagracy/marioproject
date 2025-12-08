#ifndef LIFE_H
#define LIFE_H

#include <QGraphicsTextItem>
#include <QFont>

class Life : public QGraphicsTextItem
{
public:
    Life(QGraphicsItem* parent = nullptr, int startingLives = 3);

    void decrease();
    void increase();
    void reset();
    int getLives() const;

private:
    int lives;
    const int maxLives;
    void updateText();
};

#endif
