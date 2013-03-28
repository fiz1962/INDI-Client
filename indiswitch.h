#ifndef INDISWITCH_H
#define INDISWITCH_H

#include <QRadioButton>
#include <QString>
#include "libindi-0.9.6/libs/indibase/indiproperty.h"

class INDISwitch : public QRadioButton
{
    Q_OBJECT
public:
    INDISwitch(QString textQObject = "", QWidget *parent = 0);
    int nPropertyIndex;
    int nSwitchIndex;
    QString name;
    INDI::Property* thisProperty;

signals:
    
public slots:
    
};

#endif // INDISWITCH_H
