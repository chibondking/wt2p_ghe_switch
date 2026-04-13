#include "MainWindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("GHEControl v0.1.0");
    setMinimumSize(200, 350);

    auto *label = new QLabel("GHEControl \u2014 connecting...", this);
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);
}

MainWindow::~MainWindow() = default;
