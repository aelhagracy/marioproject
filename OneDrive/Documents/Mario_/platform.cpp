#include "platform.h"
#include <QPixmap>
#include <QGraphicsItem>
platform::platform(int width, int height, const QString& imgPath)
{
    QPixmap img(imgPath);
    setPixmap(img.scaled(width,height));
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}
