/*
 * =========================================================================
 *
 *       Filename:  isearch.h
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
#include "ui_mainwindow.h"
#include "product.h"
#include "isearch.h"

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  MainWindow(QWidget *parent) : QMainWindow(parent),
 *				   ui(new Ui::MainWindow)
 *  Description:  construct function of MainWindow
 * =========================================================================
*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    procNum = 10;
    widget = new QWidget;
    layout = new QVBoxLayout;
    hhLayout = new QHBoxLayout;
    bvLayout = new QVBoxLayout;
    thLayout = new QHBoxLayout;
    blabel = new QLabel;
    hlabel = new QLabel;
    tlabel = new QLabel;
    lineEdit = new QLineEdit;
    sbutton = new QPushButton;
    splitter = new QSplitter(Qt::Vertical);
    scrollArea = new QScrollArea;

    setWindowTitle("Search Your Goods");
    setWindowIcon(QIcon("suning.png"));
    addheadToWidget();
    addBodyToWidget();
    addTailToWidget();

    mergeLayout();
    widget->setLayout(layout);
    setCentralWidget(widget);
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  ~MainWindow()
 *  Description:  release the system resource in case of memory leaks
 * =========================================================================
*/
MainWindow::~MainWindow()
{
    std::cout << "clean envrionment" << std::endl;
    delete ui;
    delete widget;
    delete layout;
    delete hhLayout;
    delete bvLayout;
    delete thLayout;
    delete blabel;
    delete hlabel;
    delete tlabel;
    delete lineEdit;
    delete sbutton;
    delete splitter;

    //clean the environment
//    struct dirent *dir;
//    struct stat statbuf;
//    DIR *sd;

//    unlink(PROFILE);
//    sd = opendir(LOG);
//    if (sd == NULL)
//        exit(0);
//    chdir(LOG);
//    while ((dir = readdir(sd))) {
//        lstat(dir->d_name, &statbuf);
//        if (S_ISREG(statbuf.st_mode))
//            unlink(dir->d_name);
//        std::cout << dir->d_name << std::endl;
//    }
//    chdir("../");
//    closedir(sd);
//    rmdir(LOG);
}

void MainWindow::enablesbutton(const QString &text)
{
    sbutton->setEnabled(!text.isEmpty());
    sbutton->setDefault(true);
    searchText = text;
}

void MainWindow::cleanSystem()
{
    int res = 0;

    res = unlink(PROFILE);
    if (res)
        unlink(PROFILE);

}

void MainWindow::changeWidget()
{
    blabel->setText("Searching your products......");
    bvLayout->addWidget(blabel);
    connect(this, SIGNAL(searchFinished()), this, SLOT(showResult()));
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  searchResult()
 *  Description:  when receive the signal begin to search
 * =========================================================================
*/
void MainWindow::searchResult()
{
#ifdef DEBUG
    std::cout << "Begin search" << std::endl;
#endif
    unlink(PROFILE);
    search = new isearch(searchText.toUtf8().constData());
    search->connectToHost();
    if (search->isConnSucc()) {
#ifdef DEBUG
        std::cout << "Connect succeed..." << std::endl;
#endif
        search->sendRequestToHost();
        search->receiveMsgFromHost();
#ifdef DEBUG
        std::cout << "connected closed..." <<std::endl;
#endif
    } else {
        //connect failed!
        blabel->setText("Please check your network connected!");
        return;
    }
    if (search->searchFinished()) {
        procNum = search->getProductNum();
#ifdef DEBUG
    std::cout << "search finished" << std::endl;
#endif
        emit searchFinished();
    }
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  showResult()
 *  Description:  show the result in mainWindow
 * =========================================================================
*/
void MainWindow::showResult()
{
#ifdef DEBUG
    std::cout << "Show result" << std::endl;
#endif
    int i = 0;
    char result[50];

    snprintf(result, 50, "We found <i>%d</i> products for you", procNum);
    blabel->setText(result);
    splitter = new QSplitter(Qt::Vertical);

    hhLayout->setAlignment(Qt::AlignTop);
    thLayout->setAlignment(Qt::AlignBottom);
    scrollArea->viewport()->setAutoFillBackground(true);
    while (i < procNum) {
        QHBoxLayout *pro= new product(i+1);
        QLabel *line = new QLabel("<hr>");
        QLabel *text = new QLabel;

        text->setFixedHeight(170);
        text->setFixedWidth(400);
        text->setLayout(pro);

        splitter->addWidget(text);
        splitter->addWidget(line);
        i++;
    }
    if (i == 0)
        blabel->setText("<h4>Sorry, we found nothing for you!</h4><br>Please change another keywords");
    else {
        splitter->setStretchFactor(1, 1);
        scrollArea->setWidget(splitter);
        bvLayout->addWidget(scrollArea);
    }

}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  addheadToWidget()
 *  Description:  add search head to widget
 * =========================================================================
*/
void MainWindow::addheadToWidget()
{
    sbutton->setText("Search");
    sbutton->setEnabled(false);

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this,
            SLOT(enablesbutton(const QString &)));
    connect(sbutton, SIGNAL(clicked()), this, SLOT(changeWidget()));
    connect(sbutton, SIGNAL(clicked()), this, SLOT(searchResult()));
//    connect(this, SIGNAL(searchFinished()), this, SLOT(showResult()));

    hlabel->setBuddy(lineEdit);
    hhLayout->addWidget(hlabel);
    hhLayout->addWidget(lineEdit);
    hhLayout->addWidget(sbutton);
    hhLayout->setAlignment(Qt::AlignCenter);
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  addBodyToWidget()
 *  Description:  add body to widget
 * =========================================================================
*/
void MainWindow::addBodyToWidget()
{
    blabel->setText("begin to Search your Goods");
    bvLayout->addWidget(blabel);
    bvLayout->setAlignment(Qt::AlignCenter);
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  addTailToWidget()
 *  Description:  add tail layout to widget
 * =========================================================================
*/
void MainWindow::addTailToWidget()
{
    tlabel->setText("<h6>Copyright&copy; Ocreator, 2013</h6>");
    tlabel->setAlignment(Qt::AlignCenter);
    thLayout->addWidget(tlabel);
}

/*
 * ===  FUNCTION  ==========================================================
 *         Name:  mergeLayout()
 *  Description:  add all xlayout to a layout
 * =========================================================================
*/
void MainWindow::mergeLayout()
{

    layout->addLayout(hhLayout);
    layout->addLayout(bvLayout);
    layout->addLayout(thLayout);

}
