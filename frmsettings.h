#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QDialog>

namespace Ui {
class frmSettings;
}

class frmSettings : public QDialog
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();

private slots:
    void on_cmdcancel_clicked();

    void on_cmdforms_clicked();

    void on_cmddata_clicked();

    void on_cmdsave_clicked();

private:
    Ui::frmSettings *ui;
    QString lastDir;
};

#endif // FRMSETTINGS_H
