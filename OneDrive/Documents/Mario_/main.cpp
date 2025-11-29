#include <QApplication>
#include "gamescene.h"
#include "gameview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameScene* scene = new GameScene();
    GameView* view  = new GameView(scene);

    view->show();
    return a.exec();
}
