#include "usermanagerdialog.h"
#include "ui_usermanagerdialog.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

// Constructor: initializes UI and populates the role combo box
UserManagerDialog::UserManagerDialog(DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UserManagerDialog), dbManager(dbManager)
{
    ui->setupUi(this);

    // Add options to the role combo box
    ui->cbRole->addItem("user");
    ui->cbRole->addItem("admin");

    // Connect the "Create" button to its handler function
    connect(ui->btnCreate, &QPushButton::clicked, this, &UserManagerDialog::handleCreateUser);
}

// Destructor: clean up UI
UserManagerDialog::~UserManagerDialog()
{
    delete ui;
}

// Slot that handles creating a new user in the database
void UserManagerDialog::handleCreateUser()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text();
    QString role = ui->cbRole->currentText();

    // Validar que no estén vacíos
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please fill in all the fields.");
        return;
    }

    // Verificar si el usuario ya existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Duplicate User", "The username already exists. Please choose another.");
            return;  // No continuar con la inserción
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to verify existing users.\n" + checkQuery.lastError().text());
        return;
    }

    // Insertar usuario nuevo
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) "
                  "VALUES (:username, :password, :role)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":role", role);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "User created successfully.");
        ui->txtUsername->clear();
        ui->txtPassword->clear();
    } else {
        QMessageBox::warning(this, "Database Error",
                             "Failed to create user.\n" + query.lastError().text());
    }
}
