#include <QApplication>
#include "Game.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    Game game;
    game.resize(800, 600);
    game.show();

    return app.exec();
}
