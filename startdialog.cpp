#include "startdialog.h"
#include "qsqlquery.h"
#include "ui_startdialog.h"
#include <QSqlQuery>
#include <QMessageBox>
StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartDialog)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &StartDialog::handleLogin);

}

StartDialog::~StartDialog()
{
    delete ui;
}
QString StartDialog::getCurrentRole() const {
    return currentRole;  // where currentRole is a variable that you store after login
}
void StartDialog::handleLogin()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();

    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        currentRole = query.value(0).toString();  // saves the user's role
        accept();  // ✅ Correctly CLOSE the dialog and report that it was successful
    } else {
        QMessageBox::warning(this, "Login failed", "Incorrect username or password");
    }
}


