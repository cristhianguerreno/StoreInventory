#ifndef UPDATEITEMDIALOGUE_H
#define UPDATEITEMDIALOGUE_H

#include "Item.h"
#include <QDialog>


namespace Ui {
class UpdateItemDIalogue;
}

class UpdateItemDIalogue : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateItemDIalogue(Item* currentItem, QWidget *parent = nullptr);
    ~UpdateItemDIalogue();

    void confirmUpdate();
    void loadItemImage();
    QString getBrand();
    QString getSize(); // devuelve QString, aunque quizás conviene que devuelva int directamente
    QString getCategory();
    QString getDeposit();

private:
    Ui::UpdateItemDIalogue *ui;
    Item* currentItem; //to keep track wich is modified
    QString imageFilePath; //

};

#endif // UPDATEITEMDIALOGUE_H
