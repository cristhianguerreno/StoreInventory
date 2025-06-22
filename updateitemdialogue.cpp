#include "updateitemdialogue.h"
#include "ui_updateitemdialogue.h"
#include "databasemanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

// Constructor: initializes UI and pre-fills fields with the current item's data
UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UpdateItemDIalogue), dbManager(dbManager)
{
    ui->setupUi(this);
    this->currentItem = currentItem;  // store the reference to the item being edited

    if (currentItem != nullptr)
    {
        // Pre-fill form fields with the item's current data
        ui->lblItemName->setText(currentItem->getName());
        ui->txtName->setText(currentItem->getName());
        ui->sbQuantity->setValue(currentItem->getQuantity());
        ui->txtBrand->setText(currentItem->getBrand());
        ui->txtSize->setText(QString::number(currentItem->getSize()));
        ui->txtCategory->setText(currentItem->getCategory());
        ui->txtDeposit->setText(currentItem->getDeposit());
        ui->sbMinimumStock->setValue(currentItem->getMinimumStock());

        QPixmap pixmap(currentItem->getImageFilePath());
        ui->lblImage->setPixmap(pixmap);
        ui->lblImage->setScaledContents(true);

        imageFilePath = currentItem->getImageFilePath();  // store current image path
    }

    // Connect button signals to respective slots
    connect(ui->btnConfirmEdit, &QPushButton::clicked,
            this, &UpdateItemDIalogue::confirmUpdate);

    connect(ui->btnLoadItemImage, &QPushButton::clicked,
            this, &UpdateItemDIalogue::loadItemImage);
}

// Destructor: cleans up UI
UpdateItemDIalogue::~UpdateItemDIalogue()
{
    delete ui;
}

// Slot that handles updating the item with the form values
void UpdateItemDIalogue::confirmUpdate()
{
    QString name = ui->txtName->text();
    int quantity = ui->sbQuantity->value();
    QString brand = getBrand();
    int size = getSize().toInt();

    if (size < 0) {
        QMessageBox::warning(this, "Error", "Size cannot be negative.");
        return;
    }

    QString category = getCategory();
    QString deposit = getDeposit();
    int minimumStock = getMinimumStock();

    if (quantity >= 0)
    {
        // Update the item using setter methods
        currentItem->setName(name);
        currentItem->setQuantity(quantity);
        currentItem->setImageFilePath(imageFilePath);
        currentItem->setBrand(brand);
        currentItem->setSize(size);
        currentItem->setCategory(category);
        currentItem->setDeposit(deposit);
        currentItem->setMinimumStock(minimumStock);
    }
    else
    {
        QMessageBox::warning(this, "Invalid Quantity", "Quantity must be at least 0.");
        return;
    }

    // Save updated item to the database
    if (!dbManager->updateItem(currentItem)) {
        QMessageBox::warning(this, "Error", "Failed to save changes to the database.");
        return;
    }

    accept();  // Close dialog with "Accepted" result
}

// Slot that handles loading a new image for the item
void UpdateItemDIalogue::loadItemImage()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "./", "Image Files (*.png *.jpg)");

    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString shortName = filename.mid(lastSlash + 1);
        QString localPath = "./images/" + shortName;

        // Copy image to local images folder if it doesn't already exist
        if (!QFile::exists(localPath)) {
            if (!QFile::copy(filename, localPath)) {
                QMessageBox::warning(this, "Error", "Failed to copy the image.");
                return;
            }
        }

        // Load the image and display it
        QPixmap pixmap(localPath);
        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Error", "Failed to load the selected image.");
            return;
        }

        ui->lblImage->setPixmap(pixmap);
        ui->lblImage->setScaledContents(true);
        imageFilePath = localPath;  // Update path
    }
}

// Getters for fields from the form
QString UpdateItemDIalogue::getBrand() {
    return ui->txtBrand->text();
}

QString UpdateItemDIalogue::getSize() {
    return ui->txtSize->text();
}

QString UpdateItemDIalogue::getCategory() {
    return ui->txtCategory->text();
}

QString UpdateItemDIalogue::getDeposit() {
    return ui->txtDeposit->text();
}

int UpdateItemDIalogue::getMinimumStock() {
    return ui->sbMinimumStock->value();
}
