#include "updateitemdialogue.h"
#include "ui_updateitemdialogue.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir> //incluido clau

UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateItemDIalogue)
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
                QMessageBox::warning(this, "Error", "No se pudo copiar la imagen.");
                return;
            }
        }

        QPixmap pixmap(localPath);
        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Error", "No se pudo cargar la imagen seleccionada.");
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
