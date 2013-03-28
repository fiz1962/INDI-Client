#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include "libindi-0.9.6/libs/indibase/indiproperty.h"

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = 0);
    int nPropertyIndex;
    int nNumIndex;
    QString name;
    INDI::Property* thisProperty;

protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);

signals:
    
public slots:

};

#endif // MYLINEEDIT_H
