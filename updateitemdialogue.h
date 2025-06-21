#ifndef UPDATEITEMDIALOGUE_H
#define UPDATEITEMDIALOGUE_H

#include "Item.h"
#include <QDialog>
#include "databasemanager.h"



namespace Ui {
class UpdateItemDIalogue;
}

class UpdateItemDIalogue : public QDialog
{
    Q_OBJECT

public:
        //  explicit UpdateItemDIalogue(Item* currentItem, QWidget *parent = nullptr);
    //~UpdateItemDIalogue();

    explicit UpdateItemDIalogue(Item* currentItem, DatabaseManager* dbManager, QWidget *parent = nullptr);

    void confirmUpdate();
    void loadItemImage();
    QString getBrand();
    QString getSize(); // devuelve QString, aunque quizás conviene que devuelva int directamente
    QString getCategory();
    QString getDeposit();


    ~UpdateItemDIalogue();

private:
    Ui::UpdateItemDIalogue *ui;
    Item* currentItem; //to keep track wich is modified
    QString imageFilePath; //
    int getMinimumStock();// meter con sql
    DatabaseManager* dbManager;

};

#endif // UPDATEITEMDIALOGUE_H
