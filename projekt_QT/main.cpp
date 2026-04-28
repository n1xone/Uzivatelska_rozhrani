//make
//./StudentManager 
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application info
    QApplication::setApplicationName("StudentManager");
    QApplication::setApplicationVersion("1.0");

    // Apply dark fusion style
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,          QColor(18, 18, 30));
    darkPalette.setColor(QPalette::WindowText,      QColor(220, 220, 235));
    darkPalette.setColor(QPalette::Base,            QColor(25, 25, 40));
    darkPalette.setColor(QPalette::AlternateBase,   QColor(32, 32, 52));
    darkPalette.setColor(QPalette::ToolTipBase,     QColor(18, 18, 30));
    darkPalette.setColor(QPalette::ToolTipText,     QColor(220, 220, 235));
    darkPalette.setColor(QPalette::Text,            QColor(220, 220, 235));
    darkPalette.setColor(QPalette::Button,          QColor(35, 35, 58));
    darkPalette.setColor(QPalette::ButtonText,      QColor(220, 220, 235));
    darkPalette.setColor(QPalette::BrightText,      Qt::red);
    darkPalette.setColor(QPalette::Link,            QColor(100, 140, 255));
    darkPalette.setColor(QPalette::Highlight,       QColor(80, 100, 220));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(100, 100, 120));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text,       QColor(100, 100, 120));

    QApplication::setPalette(darkPalette);

    QFont appFont("Segoe UI", 10);
    QApplication::setFont(appFont);

    MainWindow w;
    w.show();

    return a.exec();
}
