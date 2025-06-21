#include "usermanagerdialog.h"
#include "ui_usermanagerdialog.h"
#include <QMessageBox>

UserManagerDialog::UserManagerDialog(DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UserManagerDialog), dbManager(dbManager)
{
    ui->setupUi(this);

    // Options with role
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
        QMessageBox::warning(this, "Error", "Please complete all fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if (query.exec()) {
        QMessageBox::information(this,"Success", "User created successfully.");
        ui->txtUsername->clear();
        ui->txtPassword->clear();
    } else {
        QMessageBox::warning(this, "Error", "The user could not be created.\n" + query.lastError().text());
    }
}
