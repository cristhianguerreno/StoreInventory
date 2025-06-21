#ifndef USERMANAGERDIALOG_H
#define USERMANAGERDIALOG_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class UserManagerDialog;
}

class UserManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagerDialog(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~UserManagerDialog();

private slots:
    void handleCreateUser();

private:
    Ui::UserManagerDialog *ui;
    DatabaseManager* dbManager;
};

#endif // USERMANAGERDIALOG_H
