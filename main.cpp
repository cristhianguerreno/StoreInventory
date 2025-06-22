#include "mainwindow.h"
#include "qapplication.h"
#include "startdialog.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager dbManager;
    dbManager.initializeDatabase();  // Initialize the database connection and create tables if needed

    StartDialog login;
    if (login.exec() == QDialog::Accepted) {
        // If login was successful, retrieve user role and open main window
        QString userRole = login.getCurrentRole();

        MainWindow w(userRole, &dbManager);
        w.show();
        return a.exec();
    }

    // If login dialog was closed or cancelled, exit the application
    return 0;
}
