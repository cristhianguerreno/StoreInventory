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
    if (login.exec() == QDialog::Accepted) {  // ✅ Solo entra si el login fue exitoso
        QString userRole = login.getCurrentRole();

        MainWindow w(userRole, &dbManager);
        w.show();
        return a.exec();
    }

    // ❌ Si cerró el diálogo o hizo cancel, no continúa
    return 0;
}

