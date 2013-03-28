#include "indiclientdlg.h"
#include "libindi-0.9.6/libs/indibase/basedevice.h"

#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QGroupBox>
#include <QFormLayout>

INDIClientDlg::INDIClientDlg(QObject* parent):QObject(parent)
{
    tabNames.clear();
    properties.clear();
    myLineEdits.clear();
    myMessages.clear();
}

INDIClientDlg::~INDIClientDlg()
{
    disconnectDevice(myDevice.toLocal8Bit().data());
    disconnectServer();
    tabNames.clear();
    myMessages.clear();

    for(int i=0; i<myLineEdits.size(); i++)
        delete myLineEdits.at(i);
    myLineEdits.clear();
}

void INDIClientDlg::newDevice(INDI::BaseDevice *dp)
{

}

void INDIClientDlg::newProperty(INDI::Property *property)
{
    QByteArray bArray = myDevice.toLocal8Bit();
    const char* strPtr = bArray.data();

    if( strcmp(property->getDeviceName(), strPtr) )
        return;

    properties.push_back(property);
    if( getTabIndex(property->getGroupName()) == -1 )
        tabNames.push_back(property->getGroupName());

    emit UpdateTabs();

}

void INDIClientDlg::Connect(QString server, QString device, int port)
{
    myServer = server;
    myDevice = device;
    myPort   = port;

    QByteArray bArray = server.toLocal8Bit();
    const char* strPtr = bArray.data();

    setServer(strPtr, myPort);
    connectServer();
}

QWidget* INDIClientDlg::getWidget(int n)
{
    QFormLayout* mainform = new QFormLayout;
    for(unsigned i=0; i<properties.size(); i++)
    {
        QByteArray bArray = tabNames[n].toLocal8Bit();
        const char* strPtr = bArray.data();
        if( strcmp(properties.at(i)->getGroupName(), strPtr) )
                continue;

        QGroupBox* group = new QGroupBox(properties.at(i)->getLabel());
        QFormLayout* layout = new QFormLayout;

        switch( properties.at(i)->getType() )
        {
        case INDI_NUMBER:
            for(int j=0; j<properties.at(i)->getNumber()->nnp; j++)
            {
                MyLineEdit* lineEdit = new MyLineEdit;
                lineEdit->nPropertyIndex = i;
                lineEdit->nNumIndex = j;

                char buf[256];
                sprintf(buf, "%lf", properties.at(i)->getNumber()->np[j].value);
                lineEdit->setText(buf);
                lineEdit->name = properties.at(i)->getName();
                //
                // set read only, write only or read/write
                // assume rw or write only unless read only specififed
                lineEdit->setReadOnly(false);
                if( properties.at(i)->getNumber()->p == IP_RO )
                    lineEdit->setReadOnly(true);

                myLineEdits.push_back(lineEdit);
                connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(NumEdited()));
                layout->addRow(properties.at(i)->getNumber()->np[j].label, lineEdit);
            }
            break;

        case INDI_SWITCH:
            for(int j=0; j<properties.at(i)->getSwitch()->nsp; j++)
            {
                INDISwitch *indiSwitch = new INDISwitch(properties.at(i)->getSwitch()->sp[j].label);
                indiSwitch->nPropertyIndex = i;
                indiSwitch->nSwitchIndex = j;
                indiSwitch->name = properties.at(i)->getName();
                connect(indiSwitch, SIGNAL(pressed()), this, SLOT(SwitchToggled()));
                switch( properties.at(i)->getSwitch()->sp[j].s )
                {
                    case 0:
                        indiSwitch->setChecked(false);
                        break;
                    case 1:
                        indiSwitch->setChecked(true);
                        break;
                    default:
                        indiSwitch->setText("?");
                }

             //   if (properties.at(i)->getDeviceName()==myDevice && !strcmp(properties.at(i)->getName(),"CONNECTION") )
             //       indiSwitch->setDisabled(true);

                layout->addRow(indiSwitch);
            }
            break;

        case INDI_TEXT:
            for(int j=0; j<properties.at(i)->getText()->ntp; j++)
            {
                MyLineEdit *lineEdit = new MyLineEdit;
                lineEdit->setText(properties.at(i)->getText()->tp[j].text);
                connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(TextEdited()));
                lineEdit->nPropertyIndex = i;
                lineEdit->nNumIndex = j;
                layout->addRow(properties.at(i)->getText()->tp[j].label, lineEdit);
            }
            break;

        case INDI_LIGHT:
        case INDI_BLOB:
        case INDI_UNKNOWN:
            default:
                MyLineEdit *lineEdit = new MyLineEdit;
                layout->addRow("Yo", lineEdit);
                break;
        }

        group->setLayout(layout);
        mainform->addRow(group);
    }

    QWidget* myWidget = new QWidget;
        myWidget->setLayout(mainform);

    return myWidget;
}

QString INDIClientDlg::getTabName(unsigned n)
{
    if( n>= tabNames.size() )
        return "Bad Tab Name";

    return tabNames.at(n);
}

int INDIClientDlg::getTabIndex(QString tab)
{
    for(unsigned i=0; i<tabNames.size(); i++)
        if( tabNames.at(i)==tab )
            return i;

    return -1;
}

void INDIClientDlg::UpdateValues()
{
    for(unsigned i=0; i<myLineEdits.size(); i++)
    {
        MyLineEdit* num = myLineEdits.at(i);
        int nPIndex = num->nPropertyIndex;
        int nNIndex = num->nNumIndex;
        if( !num->hasFocus() )
        {
                char buf[256];
                sprintf(buf, "%lf", properties.at(nPIndex)->getNumber()->np[nNIndex].value);
                num->setText(buf);
                num->update();
        }
    }
}

void INDIClientDlg::SwitchToggled()
{
    INDISwitch* caller = (INDISwitch*)QObject::sender();
    ISwitchVectorProperty *indiSwitch = (ISwitchVectorProperty*)(properties.at(caller->nPropertyIndex)->getSwitch());

   // for(int j=0; j<indiSwitch->nsp; j++)
   //     indiSwitch->sp[j].s = (ISState)0;

    //indiSwitch->sp[caller->nSwitchIndex].s = (ISState)1;//bState;
    //sendNewSwitch(indiSwitch);
    sendNewSwitch (indiSwitch->device, indiSwitch->name, indiSwitch->sp[caller->nSwitchIndex].name);

}

void INDIClientDlg::NumEdited()
{
    MyLineEdit* caller = (MyLineEdit*)QObject::sender();

    INumberVectorProperty *indiNumber = (INumberVectorProperty*)(properties.at(caller->nPropertyIndex)->getNumber());
    indiNumber->np[caller->nNumIndex].value = caller->text().toDouble();

    //for(int n=0; n<indiNumber->nnp; n++)
    //    qDebug("\n---(%p) Setting new value %lf for number %s", caller, indiNumber->np[n].value, indiNumber->np[n].name);
    sendNewNumber(indiNumber);\
//    sendNewNumber (indiNumber->device, indiNumber->name, indiNumber->np[caller->nNumIndex].name, caller->text().toDouble());
    qDebug("New number %s - %s - %s - %lf", indiNumber->device, indiNumber->name, indiNumber->np[caller->nNumIndex].name, caller->text().toDouble());
}

void INDIClientDlg::TextEdited()
{
    MyLineEdit* caller = (MyLineEdit*)QObject::sender();

    QByteArray bArray = caller->text().toLocal8Bit();
    char* strPtr = bArray.data();

    ITextVectorProperty *indiText = (ITextVectorProperty*)(properties.at(caller->nPropertyIndex)->getText());
    sendNewText (indiText->device, indiText->name, indiText->tp[caller->nNumIndex].name, strPtr);
}

void INDIClientDlg::newMessage(INDI::BaseDevice *dp, int messageID)
{
    const char* msg = dp->messageQueue(messageID);
    QString newMsg = msg;

    emit UpdateMessages(newMsg);
}

void INDIClientDlg::newSwitch(ISwitchVectorProperty *svp)
{

}

void INDIClientDlg::newNumber(INumberVectorProperty *nvp)
{
    UpdateValues();
}

void INDIClientDlg::removeProperty(INDI::Property *property)
{
    for(int i=properties.size()-1; i>=0; i--)
        if( property == properties.at(i) )
        {
            properties.erase(properties.begin() + i);
        }

    emit UpdateTabs();
}
