// main.cpp

#include <QSplashScreen>

#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create splash screen
    QPixmap splashPixmap(":/images/splash.jpg");
    //qDebug()<<splashPixmap.size();
    QSplashScreen splash(splashPixmap);
    splash.setFont(QFont("Arial",14,QFont::Bold));
    splash.showMessage("Loading Snake Game...",Qt::AlignBottom | Qt::AlignHCenter,Qt::red);
    splash.show();
    QApplication::processEvents();

    GameWindow w;

    // Delay show game window
    QTimer::singleShot(3000, &splash, [&]() {
        splash.close();
        w.show();
    });

    return app.exec();
}
