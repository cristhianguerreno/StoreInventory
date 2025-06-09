
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

public:
    Item(QString name, int quantity, QString imageFilePath);

    void setName(QString name);
    QString getName() const;

    void setQuantity(int quantity);
    int getQuantity() const;

    void setImageFilePath(QString imageFilePath);
    QString getImageFilePath() const;
};

#endif
