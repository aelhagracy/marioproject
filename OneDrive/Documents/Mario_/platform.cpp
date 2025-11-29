#include "platform.h"
#include <QPixmap>

platform::platform(int width, int height, const QString& imgPath)
{
    QPixmap img(imgPath);
    setPixmap(img.scaled(width,height));

}
