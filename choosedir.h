#ifndef CHOOSEDIR_H
#define CHOOSEDIR_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class chooseDir;
}

class chooseDir : public QDialog
{
    Q_OBJECT

public:
    explicit chooseDir(QWidget *parent = 0);
    ~chooseDir();
    QString seldir;

private slots:
    void on_cmdCancel_clicked();
    void itemExpanded(QModelIndex index);

    void on_cmdok_clicked();

private:
    Ui::chooseDir *ui;
};

#endif // CHOOSEDIR_H
