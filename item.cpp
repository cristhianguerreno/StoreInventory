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
*/

#include "item.h"
#include <QString>
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

void Item::setName(QString name) {
    this->name = name;
}

QString Item::getName() const {
    return name;
}

void Item::setQuantity(int quantity) {
    this->quantity = quantity;
}

int Item::getQuantity() const {
    return quantity;
}

void Item::setImageFilePath(QString imageFilePath) {
    this->imageFilePath = imageFilePath;
}

QString Item::getImageFilePath() const {
    return imageFilePath;
}

void Item::setBrand(QString brand) {
    this->brand = brand;
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
    this->category = category;
}

QString Item::getCategory() const {
    return category;
}

void Item::setDeposit(QString deposit) {
    this->deposit = deposit;
}

QString Item::getDeposit() const {
    return deposit;
}

void Item::setId(int id) {
    this->id = id;
}

int Item::getId() const {
    return id;
}

void Item::setMinimumStock(int minimumStock) {
    this->minimumStock = minimumStock;
}

int Item::getMinimumStock() const {
    return minimumStock;
}

bool Item::isLowStock() const {
    return quantity <= minimumStock;
}
