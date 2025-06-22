#include "startdialog.h"
#include "ui_startdialog.h"
#include <QSqlQuery>
#include <QMessageBox>

// Constructor: initializes the dialog and sets up the UI
StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);  // Set up the UI as defined in the .ui file

    // Connect the login button to the login handler function
    connect(ui->btnLogin, &QPushButton::clicked,
            this, &StartDialog::handleLogin);
}

// Destructor: deletes the UI to release memory
StartDialog::~StartDialog()
{
    delete ui;
}

// Getter function to return the current user's role
QString StartDialog::getCurrentRole() const {
    return currentRole;  // This variable is set after a successful login
}

// Slot function called when the login button is clicked
void StartDialog::handleLogin()
{
    // Retrieve the username and password entered by the user
    QString username = ui->txtUsername->text().trimmed();  // Removes leading/trailing spaces
    QString password = ui->txtPassword->text();

    // Prepare the SQL query to check the user's credentials
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    // Execute the query
    if (query.exec() && query.next()) {
        // If a matching user is found, store the role and accept the dialog
        currentRole = query.value(0).toString();
        accept();  // Close the dialog with an Accepted status
    } else {
        // Show an error message if login fails
        QMessageBox::warning(this, "Login failed", "Incorrect username or password");
    }
}

