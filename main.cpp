#include "mainwindow.h"
#include "qapplication.h"
#include "startdialog.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager dbManager;
    dbManager.initializeDatabase();

    StartDialog login;
    if (login.exec() == QDialog::Accepted) {  // ✅ Only enter if the login was successful
        QString userRole = login.getCurrentRole();

        MainWindow w(userRole, &dbManager);
        w.show();
        return a.exec();
    }

    // ❌ If you closed the dialog or canceled, it does not continue.
    return 0;
}

