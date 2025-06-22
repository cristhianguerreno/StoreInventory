#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

    // Returns the role selected by the user (e.g., admin, user)
    QString getCurrentRole() const;

private:
    Ui::StartDialog *ui;
    QString currentRole;  // Stores the currently selected user role

private slots:
    void handleLogin();  // Slot triggered when the login button is clicked
};

#endif // STARTDIALOG_H
