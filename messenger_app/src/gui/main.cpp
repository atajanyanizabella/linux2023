#include "gui/loginwindow.hpp"
#include "gui/signupwindow.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap appIcon(":logo.png");
    if (appIcon.isNull()) {
        qWarning("Failed to load application icon");
    } else {
        QIcon windowIcon(appIcon);
        app.setWindowIcon(windowIcon);
    }
    login_window loginWindow;
    loginWindow.move(QGuiApplication::primaryScreen()->availableGeometry().center()
                     - loginWindow.rect().center());
    loginWindow.show();
    return app.exec();
}

