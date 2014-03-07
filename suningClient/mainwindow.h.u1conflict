/*
 * =========================================================================
 *
 *       Filename:  mainwindow.h
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextBrowser>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QLineEdit>
#include <QPixmap>
#include <QLayout>
#include <QSlider>
#include <QLabel>

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <dirent.h>
#include "isearch.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addheadToWidget();
    void addBodyToWidget();
    void addTailToWidget();

    void mergeLayout();

signals:
    void searchFinished();

private slots:
    void enablesbutton(const QString &);
    void changeWidget();
    void searchResult();
    void showResult();
    void cleanSystem();

private:
    Ui::MainWindow *ui;

    isearch *search;
    QWidget *widget;

    int procNum;
    QString searchText;

    QPushButton *sbutton;
    QVBoxLayout *layout;

    QHBoxLayout *hhLayout;
    QVBoxLayout *bvLayout;
    QHBoxLayout *thLayout;

    QSplitter *splitter;
    QScrollArea *scrollArea;
    QLineEdit *lineEdit;
    QLabel *hlabel;
    QLabel *blabel;
    QLabel *tlabel;
};

#endif // MAINWINDOW_H
