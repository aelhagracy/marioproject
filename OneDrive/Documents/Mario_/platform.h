#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsPixmapItem>

class platform: public QGraphicsPixmapItem
{
public:
    platform(int width, int height, const QString& imgPath);
};

#endif // PLATFORM_H
