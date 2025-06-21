#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {
private:
    int id; // ID for database
    QString name;
    int quantity;
    QString imageFilePath;
    QString brand;
    int size;
    QString category;
    QString deposit;
    int minimumStock; // New field for minimun stock

public:
    // Updated Constructor
    Item(QString name, int quantity, QString imageFilePath, QString brand,
         int size, QString category, QString deposit, int minimumStock = 5, int id = -1);

    // Existing Getters and setters
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

    // New getters and setters
    void setId(int id);
    int getId() const;
    void setMinimumStock(int minimumStock);
    int getMinimumStock() const;

    // Method to verify if there is low stock
    bool isLowStock() const;
};

#endif // ITEM_H
