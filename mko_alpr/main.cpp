#include <QApplication>
#include <QFontDatabase>
#include <QDesktopWidget>

#include "logger.h"
#include "config.h"
#include "database.h"
#include "app.h"

#include <stdexcept>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    QFontDatabase::addApplicationFont(":/fuentes/angelina.TTF");
    QFontDatabase::addApplicationFont(":/fuentes/Bethhrg_.ttf");
    QFontDatabase::addApplicationFont(":/fuentes/HelveticaNeue.ttf");

//    QFontDatabase baseFuentes;
//    qDebug() << baseFuentes.families();

    // Iniciamos logger
    Logger::getInstance()->setOut("../logs");

    // Iniciamos config
    Config::getInstance();

    // Iniciamos y conectamos la DB
    Database::getInstance()->connect();

    // App
    App *app = new App( NULL );

    QRect rec = QApplication::desktop()->screenGeometry();
    int anchoScreen = rec.width();
    int altoScreen = rec.height();

    app->resize( anchoScreen / 2.5f, 3 * altoScreen / 4 );
    app->move( app->width() / 3, altoScreen / 2 - app->height() / 2 );
    app->show();

    return qApplication.exec();
}
