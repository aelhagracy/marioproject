#include "spike.h"
#include <QPixmap>

Spike::Spike()
{
    // Set size of spike (adjust as you like)
    setPixmap(QPixmap(":/images/spike.png").scaled(125, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

