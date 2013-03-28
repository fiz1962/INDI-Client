#include "indiswitch.h"

INDISwitch::INDISwitch(QString textQObject, QWidget *parent) :
    QRadioButton(textQObject, parent)
{
    nPropertyIndex = nSwitchIndex = -1;
}
