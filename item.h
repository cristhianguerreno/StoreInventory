
#ifndef ITEM_H
#define ITEM_H
#include <QString>

/*
class Item
{
public:
    Item();
    Item(QString name, int quantity, QString imageFilePath="none.png");

    void setName(QString name);
    QString getName()const;

    void setQuantity(int quantity);
    int getQuantity(int quantity) const;

    void setImageFilePath(QString imageFilePath);
    QString getImageFilePath(QString imageFilePath)const;

private:
    QString name;
    QString imageFilePath;
    int quantity;
};

#endif // ITEM_H     //ORIGINAL */


class Item
{
private:
    QString name;
    int quantity;
    QString imageFilePath;
    QString brand;
    int size;
    QString category;
    QString deposit;

public:
    Item(QString name, int quantity, QString imageFilePath = "none.png", QString brand = "",
         int size=0, QString category = "", QString deposit = "");

    void setName(QString name);
    QString getName() const;

    void setQuantity(int quantity);
    int getQuantity() const;

    void setImageFilePath(QString imageFilePath);
    QString getImageFilePath() const;

    void setBrand(QString brand);
    QString getBrand() const;

    void setSize(int size);
    int getSize() const;

    void setCategory(QString category);
    QString getCategory() const;

    void setDeposit(QString deposit);
    QString getDeposit() const;
};

#endif // ITEM_H

