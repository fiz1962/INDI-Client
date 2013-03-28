#ifndef Client_H
#define Client_H

#include "libindi-0.9.6/libs/indibase/baseclient.h"
#include <QString>
#include <QObject>
#include "mylineedit.h"

class INDIClientDlg : public QObject, public INDI::BaseClient
{
    Q_OBJECT

public:
    INDIClientDlg(QObject* parent=0);
    ~INDIClientDlg();

    QString myServer;
    QString myDevice;
    int     myPort;

    vector<QString> tabNames;
    vector<INDI::Property*> properties;
    vector<MyLineEdit*> myLineEdits;
    vector<QString> myMessages;

    void Connect(QString server, QString device, int port);
    QWidget* getWidget(int n);
    QString  getTabName(unsigned n);
    int      getTabIndex(QString tab);
    void     UpdateValues();

    virtual void newDevice(INDI::BaseDevice *dp);
    virtual void newProperty(INDI::Property *property);
    virtual void removeProperty(INDI::Property *property);
    virtual void newBLOB(IBLOB *bp) {(void) bp;}
    virtual void newSwitch(ISwitchVectorProperty *svp);
    virtual void newNumber(INumberVectorProperty *nvp);
    virtual void newMessage(INDI::BaseDevice *dp, int messageID);
    virtual void newText(ITextVectorProperty *tvp) {(void) tvp;}
    virtual void newLight(ILightVectorProperty *lvp) {(void) lvp;}
    virtual void serverConnected() {}
    virtual void serverDisconnected(int exit_code) {(void) exit_code;}

public slots:
    void NumEdited();
    void SwitchToggled();
    void TextEdited();

signals:
    void UpdateTabs();
    void UpdateMessages(QString msg);
};

#endif // Client_H
