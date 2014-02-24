#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "formsmodel.h"
#include <QModelIndex>


namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:
    void on_cmdclose_clicked();

    void on_cmdsettings_clicked();

    void formClicked(QModelIndex index);

private:
    Ui::mainWindow *ui;
    void loadSettings();
    QString dataDir;
    QString formsDir;
    formsModel *m_formModel;
};

#endif // MAINWINDOW_H
