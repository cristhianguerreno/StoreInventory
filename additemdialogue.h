#ifndef ADDITEMDIALOGUE_H
#define ADDITEMDIALOGUE_H

#include <QDialog>
#include "Item.h"

namespace Ui {
class AddItemDialogue;
}

class AddItemDialogue : public QDialog
{
    Q_OBJECT

public:            //take a item pointer by refernce
    explicit AddItemDialogue(Item*& newItem,QWidget *parent = nullptr);
    ~AddItemDialogue();
    void confirmAdd();
    void loadItemImage();

private:
    Ui::AddItemDialogue *ui;
    Item**newItem;//points to the pointer to modify the actual pointer
    QString imageFilePath;
};

#endif // ADDITEMDIALOGUE_H
