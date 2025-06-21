#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item {
private:
    int id; // ID para la base de datos
    QString name;
    int quantity;
    QString imageFilePath;
    QString brand;
    int size;
    QString category;
    QString deposit;
    int minimumStock; // Nuevo campo para stock mínimo

public:
    // Constructor actualizado
    Item(QString name, int quantity, QString imageFilePath, QString brand,
         int size, QString category, QString deposit, int minimumStock = 5, int id = -1);

    // Getters y setters existentes
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

    // Nuevos getters y setters
    void setId(int id);
    int getId() const;
    void setMinimumStock(int minimumStock);
    int getMinimumStock() const;

    // Método para verificar si está en stock bajo
    bool isLowStock() const;
};

#endif // ITEM_H
