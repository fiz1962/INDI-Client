#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGroupBox>
#include "libindi-0.9.6/libs/indibase/indiproperty.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(1);

    myClient = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myClient;


}

void MainWindow::on_pbConnect_clicked()
{
    if( !myClient )
        delete myClient;

    myClient = new INDIClientDlg();
    connect(myClient, SIGNAL(UpdateTabs()), this, SLOT(UpdateTabs()));
    connect(myClient, SIGNAL(UpdateMessages(QString)), this, SLOT(UpdateMessages(QString)));

    QString strPort = ui->portLineEdit->text();
    QString strAddress = ui->iPAddressLineEdit->text();
    QString strDevice = ui->nameLineEdit->text();

    // set address, device and port read only so we can disconnect properly
    ui->portLineEdit->setReadOnly (true);
    ui->iPAddressLineEdit->setReadOnly(true);
    ui->nameLineEdit->setReadOnly(true);

    myClient->Connect(strAddress, strDevice, strPort.toInt());
}

void MainWindow::UpdateMessages(QString msg)
{
    ui->msgsText->append(msg);
}

void MainWindow::UpdateTabs()
{
    while( ui->tabWidget->count()>1 )
        ui->tabWidget->removeTab(ui->tabWidget->count()-1);

    myClient->myLineEdits.clear();
    qDebug("Property count %d", myClient->properties.size());

    for(unsigned i=0; i<myClient->tabNames.size(); i++)
        ui->tabWidget->addTab(myClient->getWidget(i), myClient->getTabName(i));
}


void MainWindow::on_pbDisconnect_clicked()
{
    QByteArray bArray = ui->nameLineEdit->text().toLocal8Bit();
    const char* strDevice = bArray.data();

    myClient->disconnectDevice(strDevice);
    myClient->disconnectServer();

    while( ui->tabWidget->count()>1 )
        ui->tabWidget->removeTab(ui->tabWidget->count()-1);

    if( !myClient )
        delete myClient;

    myClient = NULL;

    // set address, device and port read/write only so we can disconnect properly
    ui->portLineEdit->setReadOnly (false);
    ui->iPAddressLineEdit->setReadOnly(false);
    ui->nameLineEdit->setReadOnly(false);
}
