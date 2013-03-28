#ifndef INDITAB_H
#define INDITAB_H

#include <QWidget>
#include <QFormLayout>
#include "mylineedit.h"
#include "indiswitch.h"

#include <QSignalMapper>
#include <QString>
#include <vector>
#include "libindi-0.9.6/libs/indibase/indiproperty.h"

using namespace std;

class INDITab : public QObject
{
    Q_OBJECT

public:
    INDITab(QObject* parent=0);
    ~INDITab();

    void AddProperty(INDI::Property* newProperty);
    void UpdateValues();
    int  FindProperty(INDI::Property* searchProperty);
    void UpdateProperty(INumberVectorProperty* property);
    void UpdateProperty(ISwitchVectorProperty* property);
    void RemoveProperty(QString name);
    QWidget* getWidget();

    QString tabName;
    vector<INDI::Property> myProperties;
    vector<MyLineEdit*> myLineEdits;
    vector<INDISwitch*> mySwitches;

protected slots:
    void NumEdited();
    void SwitchToggled(bool bState);

signals:
    void sendSwitch(ISwitchVectorProperty* property);
    void sendNumber(INumberVectorProperty* property);

};

#endif // INDITAB_H
