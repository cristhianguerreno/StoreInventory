
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

public:            //take a item pointer by refernce
    explicit AddItemDialogue(Item*& newItem,QWidget *parent = nullptr);
    explicit AddItemDialogue(Item*& newItem, DatabaseManager* dbManager, QWidget *parent = nullptr);
    DatabaseManager* dbManager;
    ~AddItemDialogue();
    void confirmAdd();
    void loadItemImage();

private:
    Ui::AddItemDialogue *ui;
    Item**newItem;//points to the pointer to modify the actual pointer
    QString imageFilePath;
    QString getBrand();//agregado
    int getSize();
    QString getCategory();
    QString getDeposit();
    int getMinimumStock(); //meter con sql

};

#endif // ADDITEMDIALOGUE_H
