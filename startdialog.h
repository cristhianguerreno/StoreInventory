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
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();
    QString getCurrentRole() const;

private:
    Ui::StartDialog *ui;
    QString currentRole;  // ✅ ADD THIS
private slots:
    void handleLogin();  // ✅ add this line

};


#endif // STARTDIALOG_H
