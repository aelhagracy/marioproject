#ifndef LIFE_H
#define LIFE_H

#include <QGraphicsTextItem>
#include <QFont>

class Life : public QGraphicsTextItem
{
public:
    Life(QGraphicsItem* parent = nullptr, int startingLives = 3);

    void decrease();   // lose 1 life
    void reset();      // reset to max lives
    int getLives() const;

private:
    int lives;
    const int maxLives;
    void updateText(); // update on-screen display
};

#endif // LIFE_H
