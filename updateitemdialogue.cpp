#include "updateitemdialogue.h"
#include "ui_updateitemdialogue.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir> //incluido clau
#include "databasemanager.h"


/*UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateItemDIalogue) */
UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent), ui(new Ui::UpdateItemDIalogue), dbManager(dbManager)

{
    ui->setupUi(this);

    this->currentItem= currentItem; //refernce created points to the same object

    if (currentItem != nullptr)
    {
        ui-> lblItemName -> setText(currentItem -> getName());
        ui->sbQuantity->setValue(currentItem->getQuantity());//agrego clau no se pq
        ui->txtBrand->setText(currentItem->getBrand());//agregado
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
    QString brand = getBrand(); //agregado
    int size = getSize().toInt();//poner algo que no deje meter negativo
    if (size < 0) {
        QMessageBox::warning(this, "Error", "Size cannot be negative.");
        return;
    }
    QString category = getCategory();
    QString deposit = getDeposit();
    /*
    int minimumStock = getMinimumStock(); con sql
    currentItem->setMinimumStock(minimumStock);
    */

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
    //agregado sql
    currentItem->setMinimumStock(getMinimumStock()); // ¡Ahora sí se usa el campo!
   // DatabaseManager* dbManager;

    if (!dbManager->updateItem(currentItem)) {
        QMessageBox::warning(this, "Error", "No se pudo guardar los cambios en la base de datos.");
        return;
    }
    accept(); // Cierra el diálogo con resultado "aceptado"

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
    return ui->txtBrand->text();//metio claude
}

QString UpdateItemDIalogue::getSize() {
    return ui->txtSize->text();//metio claude
}

QString UpdateItemDIalogue::getCategory() {
    return ui->txtCategory->text();//metio claude
}

QString UpdateItemDIalogue::getDeposit() {
    return ui->txtDeposit->text();//metio claude
}

int UpdateItemDIalogue::getMinimumStock() {
    return ui->sbMinimumStock->value();
} //con sql

