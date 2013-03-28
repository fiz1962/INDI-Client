#include "inditab.h"
#include <QGroupBox>
#include <QRadioButton>
#include "libindi-0.9.6/libs/indibase/baseclient.h"
#include "libindi-0.9.6/libs/indibase/basedevice.h"

INDITab::INDITab(QObject* parent): QObject(parent)
{
    qDebug("Creating  new tab");
    tabName = "";
    myProperties.clear();
    myLineEdits.clear();
}

INDITab::~INDITab()
{
    myProperties.clear();
    myLineEdits.clear();
}

void INDITab::AddProperty(INDI::Property* newProperty)
{
    myProperties.push_back(*newProperty);
}

QWidget* INDITab::getWidget()
{
    qDebug("getWidget start");
    myLineEdits.clear();
    QFormLayout* mainform = new QFormLayout;
   /* for(unsigned i=0; i<myProperties.size(); i++)
    {
        INDI::Property tryit = myProperties.at(i);

        QGroupBox* group = new QGroupBox(myProperties.at(i).getLabel());
        QFormLayout* layout = new QFormLayout;

        switch( myProperties.at(i).getType() )
        {
        case 0:
            for(int j=0; j<myProperties.at(i).getNumber()->nnp; j++)
            {
                qDebug("Get widget number %d", j);
                MyLineEdit* lineEdit = new MyLineEdit;
                lineEdit->nPropertyIndex = i;
                lineEdit->nNumIndex = j;

                myLineEdits.push_back(lineEdit);
                char buf[256];
                sprintf(buf, "%lf", myProperties.at(i).getNumber()->np[j].value);
                lineEdit->setText(buf);
                lineEdit->thisProperty = &myProperties.at(i);
                lineEdit->name = myProperties.at(i).getName();
                //qDebug("CONNECTING %s",myProperties.at(i).getNumber()->np[j].label);
                connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(NumEdited()));
                layout->addRow(myProperties.at(i).getNumber()->np[j].label, lineEdit);
            }
            break;

        case 1:
            for(int j=0; j<myProperties.at(i).getSwitch()->nsp; j++)
            {
                qDebug("Get widget switch %d", j);
                INDISwitch *indiSwitch = new INDISwitch(myProperties.at(i).getSwitch()->sp[j].label);
                indiSwitch->nPropertyIndex = i;
                indiSwitch->nSwitchIndex = j;
                indiSwitch->thisProperty = &myProperties.at(i);
                indiSwitch->name = myProperties.at(i).getName();
                mySwitches.push_back(indiSwitch);

                connect(indiSwitch, SIGNAL(toggled(bool)), this, SLOT(SwitchToggled(bool)));
                switch( myProperties.at(i).getSwitch()->sp[j].s )
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
                layout->addRow(indiSwitch);
            }
            break;

        case 2:
            for(int j=0; j<myProperties.at(i).getText()->ntp; j++)
            {
                MyLineEdit *lineEdit = new MyLineEdit;
                lineEdit->setText(myProperties.at(i).getText()->tp[j].text);
                layout->addRow(myProperties.at(i).getText()->tp[j].label, lineEdit);
            }
            break;

            default:
                MyLineEdit *lineEdit = new MyLineEdit;
                layout->addRow("Yo", lineEdit);
                break;
        }

        group->setLayout(layout);
        mainform->addRow(group);
    }*/
    QWidget* myWidget = new QWidget;
        myWidget->setLayout(mainform);

   // UpdateValues();
        qDebug("getWidget done");
    return myWidget;
}

void INDITab::UpdateValues()
{
    int nLineEditPos = 0;
    for(unsigned i=0; i<myProperties.size(); i++)
    {
        switch( myProperties.at(i).getType() )
        {
            case 0:
                for(int j=0; j<myProperties.at(i).getNumber()->nnp; j++)
                {
                    MyLineEdit* num = myLineEdits.at(nLineEditPos++);
                    if( !num->hasFocus() )
                    {
                        char buf[256];
                        sprintf(buf, "%lf", myProperties.at(i).getNumber()->np[j].value);
                        num->setText(buf);
                        num->update();
                    }
                }
                break;

            default:
                break;
        }
    }
}

void INDITab::NumEdited()
{
    int i=0;
    QObject* caller = QObject::sender();
    if( !((MyLineEdit*)caller)->isModified() )
            return;

    QString val = ((MyLineEdit*)caller)->displayText();
    i++;
}

void INDITab::SwitchToggled(bool bState)
{
    INDISwitch* caller = (INDISwitch*)QObject::sender();

    ISwitchVectorProperty *indiSwitch = (ISwitchVectorProperty*)(myProperties.at(caller->nPropertyIndex).getSwitch());

    qDebug("\n---Setting new state %d for switch %d, %d",bState, caller->nPropertyIndex, caller->nSwitchIndex);
    qDebug("# switches %d", indiSwitch->nsp);
    for(int j=0; j<indiSwitch->nsp; j++)
    {
        qDebug("Switch state %d - %d", j, indiSwitch->sp[j].s);
    }

    indiSwitch->sp[caller->nSwitchIndex].s = (ISState)bState;

    if( bState )
        emit sendSwitch(indiSwitch);
}

int INDITab::FindProperty(INDI::Property* searchProperty)
{
    for(unsigned i=0; i<myProperties.size(); i++)
        if( myProperties.at(i).getName() == searchProperty->getName() )
            return i;

    return -1;
}

void INDITab::UpdateProperty(INumberVectorProperty* property)
{
    // one INumberVectorProperty may have several numbers
    for(int i=0; i<myLineEdits.size(); i++)
        if( myLineEdits.at(i)->thisProperty->getNumber() == property )
        {
            if( !myLineEdits.at(i)->hasFocus() )
            {
                char buf[256];
                sprintf(buf, "%lf", property->np[myLineEdits.at(i)->nNumIndex].value);
                myLineEdits.at(i)->setText(buf);
                myLineEdits.at(i)->update();
            }
        }
}

void INDITab::UpdateProperty(ISwitchVectorProperty* property)
{
    // one ISwitchVectorProperty may have several numbers
   // for(int i=0; i<mySwitches.size(); i++)
   //     if( mySwitches.at(i)->thisProperty->getSwitch() == property )
   //         emit sendSwitch(property);
}

void INDITab::RemoveProperty(QString name)
{
    for(int i=myLineEdits.size()-1; i>=0; i--)
    {
        if( myLineEdits.at(i)->name == name )
        {
            myLineEdits.erase(myLineEdits.begin() + i);
        }
    }

    for(int i=mySwitches.size()-1; i>=0; i--)
    {
        if( mySwitches.at(i)->name == name )
        {
            mySwitches.erase(mySwitches.begin() + i);
        }
    }
}
