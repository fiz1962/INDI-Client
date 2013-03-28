#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    nPropertyIndex = nNumIndex = -1;
}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);

}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
  QLineEdit::focusOutEvent(e);

}
