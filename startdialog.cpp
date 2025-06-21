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
    return currentRole;  // donde currentRole es una variable que almacenas después del login
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
        currentRole = query.value(0).toString();  // guarda el rol del usuario
        accept();  // ✅ CIERRA correctamente el diálogo e informa que fue exitoso
    } else {
        QMessageBox::warning(this, "Login failed", "Incorrect username or password");
    }
}


