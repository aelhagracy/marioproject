#ifndef LIFE_H
#define LIFE_H

class Life
{
public:
    Life(int lives = 2);
    int getLife();
    int loseLife();
    bool isLife() const;

private:

    int lives;


};

#endif // LIFE_H
