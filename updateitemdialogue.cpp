#include "updateitemdialogue.h"
#include "ui_updateitemdialogue.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir> //Include clau
#include "databasemanager.h"


UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UpdateItemDIalogue), dbManager(dbManager)

{
    ui->setupUi(this);

    this->currentItem= currentItem; //refernce created points to the same object

    if (currentItem != nullptr)
    {
        ui-> lblItemName -> setText(currentItem -> getName());
        ui->sbQuantity->setValue(currentItem->getQuantity());//add clau
        ui->txtBrand->setText(currentItem->getBrand());//added
        ui->txtSize->setText(QString::number(currentItem -> getSize()));
        ui->txtCategory->setText(currentItem ->getCategory());
        ui->txtDeposit->setText(currentItem ->getDeposit());

        QPixmap pixmap(currentItem-> getImageFilePath());
        ui-> lblImage -> setPixmap(pixmap);
        ui->lblImage-> setScaledContents(true);

        ui->sbQuantity -> setValue( currentItem ->getQuantity());

        imageFilePath= currentItem -> getImageFilePath();
    } //end if

    //conections

    connect(ui-> btnConfirmEdit, &QPushButton::clicked,
            this, &UpdateItemDIalogue::confirmUpdate);

    connect(ui->btnLoadItemImage, &QPushButton::clicked,
            this, &UpdateItemDIalogue::loadItemImage);
}

UpdateItemDIalogue::~UpdateItemDIalogue()
{
    delete ui;
}

void UpdateItemDIalogue::confirmUpdate()
{
    QString name= ui->txtName->text();
    int quantity= ui-> sbQuantity -> value();
    QString brand = getBrand(); //added
    int size = getSize().toInt();//put something that doesn't let you enter negative
    if (size < 0) {
        QMessageBox::warning(this, "Error", "Size cannot be negative.");
        return;
    }
    QString category = getCategory();
    QString deposit = getDeposit();


    if (quantity >=0)
    {//call setters
        currentItem-> setName(name);
        currentItem-> setQuantity(quantity);
        currentItem-> setImageFilePath(imageFilePath);
        currentItem-> setBrand(brand);
        currentItem-> setSize(size);
        currentItem-> setCategory(category);
        currentItem-> setDeposit(deposit);
        this->close();
    } //end if
    else
    {
        QMessageBox mb;
        mb.setText("Quantity must be atleast 1");
        mb.exec();
    }// end else
    //added Sql
    currentItem->setMinimumStock(getMinimumStock()); // Now the field is used!


    if (!dbManager->updateItem(currentItem)) {
        QMessageBox::warning(this, "Error", "No se pudo guardar los cambios en la base de datos.");
        return;
    }
    accept(); // Close the dialog with the result "accepted"

} //end update
void UpdateItemDIalogue::loadItemImage()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "./", "Image Files (*.png *.jpg)");

    if (!filename.isEmpty()) {
        int lastSlash = filename.lastIndexOf("/");
        QString shortName = filename.mid(lastSlash + 1);
        QString localPath = "./images/" + shortName;

        if (!QFile::exists(localPath)) {
            if (!QFile::copy(filename, localPath)) {
                QMessageBox::warning(this, "Error", "The image could not be copied..");
                return;
            }
        }

        QPixmap pixmap(localPath);
        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Error", "The selected image could not be loaded.");
            return;
        }

        ui->lblImage->setPixmap(pixmap);
        ui->lblImage->setScaledContents(true);
        imageFilePath = localPath;
    }
}
//end UpDtaeItemDIologue

QString UpdateItemDIalogue::getBrand() {
    return ui->txtBrand->text();//insert claude
}

QString UpdateItemDIalogue::getSize() {
    return ui->txtSize->text();//insert claude
}

QString UpdateItemDIalogue::getCategory() {
    return ui->txtCategory->text();//insert claude
}

QString UpdateItemDIalogue::getDeposit() {
    return ui->txtDeposit->text();//insert claude
}

int UpdateItemDIalogue::getMinimumStock() {
    return ui->sbMinimumStock->value();
} //with sql

