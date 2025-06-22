#include "item.h"
#include <QString>

// Constructor with all attributes
Item::Item(QString name, int quantity, QString imageFilePath, QString brand,
           int size, QString category, QString deposit, int minimumStock, int id) {
    this->name = name;
    this->quantity = quantity;
    this->imageFilePath = imageFilePath;
    this->brand = brand;
    this->size = size;
    this->category = category;
    this->deposit = deposit;
    this->minimumStock = minimumStock;
    this->id = id;
}

// Set the name of the item
void Item::setName(QString name) {
    this->name = name;
}

// Get the name of the item
QString Item::getName() const {
    return name;
}

// Set the quantity in stock
void Item::setQuantity(int quantity) {
    this->quantity = quantity;
}

// Get the quantity in stock
int Item::getQuantity() const {
    return quantity;
}

// Set the image file path
void Item::setImageFilePath(QString imageFilePath) {
    this->imageFilePath = imageFilePath;
}

// Get the image file path
QString Item::getImageFilePath() const {
    return imageFilePath;
}

// Set the brand name
void Item::setBrand(QString brand) {
    this->brand = brand;
}

// Get the brand name
QString Item::getBrand() const {
    return brand;
}

// Set the item size
void Item::setSize(int size) {
    this->size = size;
}

// Get the item size
int Item::getSize() const {
    return size;
}

// Set the category
void Item::setCategory(QString category) {
    this->category = category;
}

// Get the category
QString Item::getCategory() const {
    return category;
}

// Set the deposit location
void Item::setDeposit(QString deposit) {
    this->deposit = deposit;
}

// Get the deposit location
QString Item::getDeposit() const {
    return deposit;
}

// Set the item ID
void Item::setId(int id) {
    this->id = id;
}

// Get the item ID
int Item::getId() const {
    return id;
}

// Set the minimum stock threshold
void Item::setMinimumStock(int minimumStock) {
    this->minimumStock = minimumStock;
}

// Get the minimum stock threshold
int Item::getMinimumStock() const {
    return minimumStock;
}

// Check whether the item is under low stock
bool Item::isLowStock() const {
    return quantity <= minimumStock;
}
