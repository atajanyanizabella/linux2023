#include "../../include/gui/login_window.hpp"
#include "../../include/gui/signup_window.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    login_window loginWindow;
    loginWindow.move(QGuiApplication::primaryScreen()->availableGeometry().center()
                     - loginWindow.rect().center());
    loginWindow.show();
    return app.exec();
}
