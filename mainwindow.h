#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "indiclientdlg.h"
#include "inditab.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    INDIClientDlg* myClient;

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

public:
    Ui::MainWindow *ui;

public slots:
    void UpdateTabs();
    void UpdateMessages(QString msg);
};

#endif // MAINWINDOW_H
