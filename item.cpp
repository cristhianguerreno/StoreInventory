/*
#include <QString>
#include "item.h"

Item::Item(QString name, int quantity, QString imageFilePath) {
    this ->name=name;
    this ->quantity=quantity;
    this ->imageFilePath=imageFilePath;
}

void Item::setName(QString name){
    this ->name=name;
}

QString Item::getName() const{
    return name;
}

void Item::setQuantity(int quantity){
    this->quantity=quantity;
}

int Item::getQuantity(int quantity) const {
    return quantity;
}

void Item::setImageFilePath(QString imageFilePath){
    this ->imageFilePath=imageFilePath;
}

QString Item::getImageFilePath(QString imageFilePath)const{
    return imageFilePath;
}//origunal
*/
#include "item.h"

Item::Item(QString name, int quantity, QString imageFilePath, QString brand, int size, QString category, QString deposit) {
    this->name = name;
    this->quantity = quantity;
    this->imageFilePath = imageFilePath;
    this->brand = brand;//agregar
    this->size = size ;
    this->category = category;
    this->deposit = deposit;

}

void Item::setName(QString name){
    this->name = name;
}

QString Item::getName() const {
    return name;
}

void Item::setQuantity(int quantity){
    this->quantity = quantity;
}

int Item::getQuantity() const {
    return quantity;
}

void Item::setImageFilePath(QString imageFilePath){
    this->imageFilePath = imageFilePath;
}

QString Item::getImageFilePath() const {
    return imageFilePath;
}
//agregar
void Item::setBrand(QString brand) {
    this->brand=brand;
}

QString Item::getBrand() const {
    return brand;
}

void Item::setSize(int size) {
    this->size = size;
}

int Item::getSize() const {
    return size;
}

void Item::setCategory(QString category) {
    this->category=category;
}

QString Item::getCategory() const {
    return category;
}

void Item::setDeposit(QString deposit) {
    this->deposit=deposit;
}

QString Item::getDeposit() const {
    return deposit;
}
