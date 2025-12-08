#include "bonusblock.h"
#include <QPixmap>

bonusblock::bonusblock()
{
    setPixmap(QPixmap(":/images/bonus.png").scaled(30, 30));
}
