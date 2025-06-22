#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {
private:
    int id;                         // ID for the database (used as primary key)
    QString name;                   // Item name
    int quantity;                   // Available quantity
    QString imageFilePath;         // Path to item image
    QString brand;                 // Item brand
    int size;                      // Size (e.g., for clothing or packaging)
    QString category;              // Category of the item
    QString deposit;               // Deposit or storage location
    int minimumStock;             // Minimum stock before warning (default is 5)

public:
    // Constructor with default values for minimumStock and id
    Item(QString name, int quantity, QString imageFilePath, QString brand,
         int size, QString category, QString deposit, int minimumStock = 5, int id = -1);

    // Getters and setters
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

    void setId(int id);
    int getId() const;

    void setMinimumStock(int minimumStock);
    int getMinimumStock() const;

    // Returns true if the quantity is below the minimumStock threshold
    bool isLowStock() const;
};

#endif // ITEM_H
