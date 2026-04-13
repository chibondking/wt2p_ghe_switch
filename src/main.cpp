#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1e1e1e;
            color: #c0c0c0;
        }
        QPushButton {
            background-color: #3a3a3a;
            border: 1px solid #555;
            border-radius: 3px;
            padding: 4px 8px;
            color: #c0c0c0;
        }
        QPushButton:hover { background-color: #454545; }
        QPushButton:pressed { background-color: #2a2a2a; }
    )");

    MainWindow window;
    window.show();

    return app.exec();
}
