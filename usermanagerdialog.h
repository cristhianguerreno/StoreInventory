#ifndef USERMANAGERDIALOG_H
#define USERMANAGERDIALOG_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
class UserManagerDialog;
}

// Dialog for managing users (e.g., creating new accounts)
class UserManagerDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor that receives a pointer to the database manager
    explicit UserManagerDialog(DatabaseManager* dbManager, QWidget *parent = nullptr);

    // Destructor
    ~UserManagerDialog();

private slots:
    // Slot triggered when the user clicks to create a new user
    void handleCreateUser();

private:
    Ui::UserManagerDialog *ui;
    DatabaseManager* dbManager; // Pointer to the database manager for user-related operations
};

#endif // USERMANAGERDIALOG_H
