#ifndef ADDITEMDIALOGUE_H
#define ADDITEMDIALOGUE_H

#include <QDialog>
#include "Item.h"
#include "databasemanager.h"

namespace Ui {
class AddItemDialogue;
}

class AddItemDialogue : public QDialog
{
    Q_OBJECT

public:
    // Constructor that receives a reference to an Item pointer
    explicit AddItemDialogue(Item*& newItem, QWidget *parent = nullptr);

    // Constructor that receives a reference to an Item pointer and a database manager
    explicit AddItemDialogue(Item*& newItem, DatabaseManager* dbManager, QWidget *parent = nullptr);

    ~AddItemDialogue();

    // Confirm the addition of the item (validate and save)
    void confirmAdd();

    // Open a file dialog and load an image for the item
    void loadItemImage();

    // Database manager used to interact with the database
    DatabaseManager* dbManager;

private:
    Ui::AddItemDialogue *ui;

    // Pointer to pointer of Item: allows modifying the actual Item object
    Item** newItem;

    // Path of the selected image
    QString imageFilePath;

    // Get item details from UI inputs
    QString getBrand();
    int getSize();
    QString getCategory();
    QString getDeposit();
    int getMinimumStock(); // Retrieves the minimum stock value from the form
};

#endif // ADDITEMDIALOGUE_H
