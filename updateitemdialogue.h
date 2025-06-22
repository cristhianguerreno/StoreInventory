#ifndef UPDATEITEMDIALOGUE_H
#define UPDATEITEMDIALOGUE_H

#include <QDialog>
#include "Item.h"
#include "databasemanager.h"

namespace Ui {
class UpdateItemDIalogue;
}

class UpdateItemDIalogue : public QDialog
{
    Q_OBJECT

public:
    // Constructor: receives pointer to the current item and the database manager
    explicit UpdateItemDIalogue(Item* currentItem, DatabaseManager* dbManager, QWidget *parent = nullptr);

    // Destructor
    ~UpdateItemDIalogue();

    // Confirm and apply updates to the item
    void confirmUpdate();

    // Loads a new image for the item
    void loadItemImage();

    // Accessor methods to retrieve updated data from the form
    QString getBrand();
    QString getSize();         // Currently returns QString; consider returning int directly
    QString getCategory();
    QString getDeposit();
    int getMinimumStock();     // Retrieves the minimum stock value (used with SQL)

private:
    Ui::UpdateItemDIalogue *ui;
    Item* currentItem;         // Pointer to the item being edited
    QString imageFilePath;     // Stores the file path of the selected image
    DatabaseManager* dbManager; // Pointer to the database manager
};

#endif // UPDATEITEMDIALOGUE_H
