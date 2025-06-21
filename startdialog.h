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
    QString currentRole;  // ✅ AGREGA ESTO
private slots:
    void handleLogin();  // ✅ Agregá esta línea

};


#endif // STARTDIALOG_H
