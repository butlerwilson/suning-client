/*
 * =========================================================================
 *
 *       Filename:  product.cpp
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

#include "product.h"

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  product(int pronum)
 *  Description:  construct the products layout
 * =========================================================================
*/
product::product(int pronum)
{
    QHBoxLayout *phlayout;
    QVBoxLayout *pvlayout;
    QLabel *picture;
    QLabel *description;
    QLabel *price;
    QLabel *comment;
    QLabel *status;
    QPushButton *buy;

    phlayout = new QHBoxLayout;
    pvlayout = new QVBoxLayout;
    picture = new QLabel;
    description = new QLabel();
    price = new QLabel();
    comment = new QLabel();
    status = new QLabel();
    buy = new QPushButton("buy me");


//    dstr.append("Description: ");
//    pstr.append("Price: ");
    cstr.append("Comments: ");

    char propic[20];
    char propri[20];

    readProInfo(pronum - 1);
    description->setText(dstr);
    comment->setText(cstr);
    status->setText(sstr);
    sprintf(propic, "./log/propic%d.jpg", pronum);
    sprintf(propri, "./log/propri%d.png", pronum);
    QPixmap pic(propic);
    if (pic.isNull())
        picture->setText("No picture now");
    else
        picture->setPixmap(pic);
    QPixmap pri(propri);
    if (pri.isNull())
        price->setText("Price Unknown");
    else
        price->setPixmap(pri);

    pvlayout->addWidget(description);
    pvlayout->addWidget(price);
    pvlayout->addWidget(comment);
    pvlayout->addWidget(status);
    pvlayout->addWidget(buy);

    phlayout->addWidget(picture);
    phlayout->addLayout(pvlayout);
    this->addLayout(phlayout);

}

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  readProInfo(int pronum)
 *  Description:  read the product informations from file
 * =========================================================================
*/
int product::readProInfo(int pronum)
{
    int i = 0;
    QFile file(PROFILE);

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split('\t');
            QString str1 = fields.takeFirst();
            QString str2 = fields.takeFirst();
            QString str3 = fields.takeFirst();
            if (pronum == i) {
                if (str1.isEmpty())
                    dstr.append("No information now");
                else
                    dstr.append(str1);
                if (str2.isEmpty())
                    sstr.append("No information now");
                else
                    sstr.append(str2);
                if (str3.isEmpty())
                    cstr.append("No information now");
                else
                    cstr.append(str3);
            }
            i++;
        }
    }
}
