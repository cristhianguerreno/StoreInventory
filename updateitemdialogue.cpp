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
    QString filename;

    filename= QFileDialog::getOpenFileName(this,"Open Image Files",
                                            "./","Image FIles (*png *jpg)");

    if (filename != " ") //if has some value //if not they click cancel
    {
        int lastSlash= filename.lastIndexOf("/");
        QString shortName= filename.right(filename.size() - lastSlash - 1); //because starts from 0

        QFile::copy(filename, "./images/" + shortName);//copy from original location to images
        QPixmap pixmap("./images/"+shortName); //load ir on our memoery

        ui ->lblImage -> setPixmap(pixmap);
        ui->lblImage ->setScaledContents(true);

         imageFilePath = "./images/" + shortName; //cht actualiza el path

    }    //end if



//aca cambio las coassas claude no cambie pq funciona este y ademas no tiene que ver con brand

} //end UpDtaeItemDIologue

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
