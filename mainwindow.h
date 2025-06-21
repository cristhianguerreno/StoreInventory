

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"
#include "databasemanager.h"
#include <QListWidgetItem>


//
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
//
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString role, DatabaseManager* dbManager, QWidget *parent = nullptr);


    ~MainWindow();

    void hanldeMenuItemNew(); //creates slot for a clicking of new ite,
    void removeSelectedProduct();
    void handleItemClick(QListWidgetItem* item);//pass pointer
    void handleMenuItemEdit();//no se de donde salio
    void handleSaveItems();
    void handleLoadItems();



private:
    Ui::MainWindow *ui;
    QVector<Item*> productList;
    DatabaseManager* dbManager;
    QString role;
    void loadItemsFromDatabase();
    void addItemToList(Item* item);
    void updateItemInList(int index, Item* item);
    void updateProductDisplay(Item* item);
    void clearProductDisplay();

    //meter con sql
private slots:
    void checkLowStock();
   // void openUserManager();  // ✅ dentro de slots


};
#endif // MAINWINDOW_H
