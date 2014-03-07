/*
 * =========================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  implement the fibonacci using recusion
 *
 *        Version:  1.0
 *        Created:  11/22/2013 05:30:27 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Youngcy (yangxin), youngcy.youngcy@gmail.com
 *   Organization:  Ocreator
 *
 * =========================================================================
 */

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *mainWindow = new MainWindow;
    mainWindow->setFixedHeight(700);
    mainWindow->setFixedWidth(450);
    mainWindow->show();

    return a.exec();
}
