#include "usermanagerdialog.h"
#include "ui_usermanagerdialog.h"
#include <QMessageBox>

UserManagerDialog::UserManagerDialog(DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UserManagerDialog), dbManager(dbManager)
{
    ui->setupUi(this);

    // Opciones de rol
    ui->cbRole->addItem("user");
    ui->cbRole->addItem("admin");

    connect(ui->btnCreate, &QPushButton::clicked, this, &UserManagerDialog::handleCreateUser);
}

UserManagerDialog::~UserManagerDialog()
{
    delete ui;
}

void UserManagerDialog::handleCreateUser()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();
    QString role = ui->cbRole->currentText();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor completa todos los campos.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if (query.exec()) {
        QMessageBox::information(this, "Éxito", "Usuario creado exitosamente.");
        ui->txtUsername->clear();
        ui->txtPassword->clear();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo crear el usuario.\n" + query.lastError().text());
    }
}
