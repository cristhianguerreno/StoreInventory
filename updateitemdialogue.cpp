#include "updateitemdialogue.h"
#include "ui_updateitemdialogue.h"

#include <QFileDialog>
#include <QMessageBox>

UpdateItemDIalogue::UpdateItemDIalogue(Item* currentItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateItemDIalogue)
{
    ui->setupUi(this);

    this->currentItem= currentItem; //refernce created points to the same object

    if (currentItem != nullptr)
    {
        ui-> lblItemName -> setText(currentItem -> getName());

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
    int quantity= ui-> sbQuantity -> value();
    if (quantity >=1)
    {
        currentItem-> setQuantity(quantity);
        currentItem-> setImageFilePath(imageFilePath);
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



} //end UpDtaeItemDIologue
