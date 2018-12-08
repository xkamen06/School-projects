/** \file
 * @file    main.cpp
 * @author  Miroslav Karpíšek
 * @date    19 april 2016
 * @brief   Spusteni okna kalkulacky
 */


#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    w.setFixedSize(300, 475);
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center()); //vycentrovano
    w.show();

    return a.exec();
}
