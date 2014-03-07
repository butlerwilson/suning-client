/*
 * =========================================================================
 *
 *       Filename:  product.h
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

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextStream>
#include <QPushButton>
#include <QLabel>
#include <QFile>

#include <iostream>
#include <stdio.h>

#include "syscntl.h"
#include "isearch.h"

class product : public QHBoxLayout
{
public:
    product(int pronum);
    int readProInfo(int pronum);
private:
    QString dstr;
    QString pstr;
    QString cstr;
    QString sstr;
};

#endif // PRODUCT_H
