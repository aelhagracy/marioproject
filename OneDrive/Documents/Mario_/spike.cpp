#include "spike.h"
#include <QPixmap>

Spike::Spike()
{

    setPixmap(QPixmap(":/images/spike.png").scaled(125, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

