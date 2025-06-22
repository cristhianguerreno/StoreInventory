#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"
#include "databasemanager.h"
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor: receives user role and database manager
    MainWindow(QString role, DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~MainWindow();

    // Slot methods for user actions
    void hanldeMenuItemNew();               // Triggered when user clicks to create a new item
    void removeSelectedProduct();           // Removes the selected product from the list
    void handleItemClick(QListWidgetItem* item); // Handles item click events in the UI
    void handleMenuItemEdit();              // Opens edit dialog for selected item
    void handleSaveItems();                 // Saves current items (if needed)
    void handleLoadItems();                 // Loads items into the application

private:
    Ui::MainWindow *ui;
    QVector<Item*> productList;             // Stores pointers to items
    DatabaseManager* dbManager;             // Database interface
    QString role;                           // User role (e.g., admin, user)

    // Internal helper methods
    void loadItemsFromDatabase();           // Load items from the database
    void addItemToList(Item* item);         // Add an item to the QListWidget
    void updateItemInList(int index, Item* item); // Update an item at a given index
    void updateProductDisplay(Item* item);  // Update the item details in the UI
    void clearProductDisplay();             // Clear product detail display

    void exportProductsToJson();   // o exportProductsToCsv()
    void importProductsFromJson(); // o importProductsFromCsv()

private slots:
    void checkLowStock();                   // Check for items below minimum stock
    // void openUserManager();              // Uncomment to enable user manager
};

#endif // MAINWINDOW_H
