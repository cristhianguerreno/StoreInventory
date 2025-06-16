#include "additemdialogue.h"
#include "ui_additemdialogue.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QIntValidator> //validar size


AddItemDialogue::AddItemDialogue(Item*& newItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddItemDialogue)
{
    ui->setupUi(this);
    this ->newItem=&newItem;//give me the pointer of the new item

    imageFilePath="none.png";//default

    ui->txtSize->setValidator(new QIntValidator(0, 10000, this));//solo enteris de 0 a 1000ml

    //registering  the event
    connect(ui->btnConfirmAdd, &QPushButton::clicked ,this,
            &AddItemDialogue::confirmAdd);

    connect(ui->btnLoadImage, &QPushButton::clicked, this,
            &AddItemDialogue::loadItemImage);

    QDir pathDir("none.png");
    if (!pathDir.exists())
    {
        //creat
        pathDir.mkdir("./images/");
    }
}

AddItemDialogue::~AddItemDialogue()
{
    delete ui;
}

void AddItemDialogue::confirmAdd()
{    //obtain values from the interface
    QString productName = ui->txtProductName->text();
    int quantity = ui->sbQuantity->value();
    QString brand = getBrand();//agregado
    int size = getSize();
    QString category = getCategory();
    QString deposit = getDeposit();


  //  *newItem = new Item(name, quantity, image, brand); // usando constructor largo //agragado

    //trimmed() takes any blank space before or after
    //if it isnt empty and atleast add 1
    if (productName.trimmed()!= " " && quantity>=1)
    {
        //creates new pointer with values
        *newItem = new Item(productName,quantity, imageFilePath, brand, size, category, deposit);
        this -> close();
    }
    else
    {
        QMessageBox mb; //if one of the two fields is wrong will display
        mb.setText("You must have a valid name and atleast add 1 item");
        mb.exec();
    }
} //end confirmAdd

void AddItemDialogue::loadItemImage()
{

    /*

    QString filename;

    filename= QFileDialog::getOpenFileName(this, "Open Image", "./",
                                            "Image Files (*.png *.jpg)");
    if (filename != "") //if isnt empty
    {
        int lastSlash= filename.lastIndexOf("/");   //find last index
        //gives the name of the file

        QString shortName = filename.right(filename.size() - lastSlash - 1);

        //File Manipulation: takes filename and puts in the local directory
        //so if its deleted we dont have problems

        if (!QFile::exists(localPath)) {
            if (!QFile::copy(filename, localPath)) {
                QMessageBox::warning(this, "Error", "No se pudo copiar la imagen.");
                return;
            }
        }


        QPixmap pixmap("./images/" + shortName);  //load the image

        if (pixmap.isNull()) {
            QMessageBox::warning(this, "Error", "Failed to load the image.");
            return;
        }
        ui->lblImage->setPixmap(pixmap); //
        ui->lblImage->setScaledContents(true);  //w/o this the image can be cut

        imageFilePath="./images/" + shortName;
        //update the internal data
        */


    QString filename = QFileDialog::getOpenFileName(this, "Open Image", "./", "Image Files (*.png *.jpg)");

    if (filename.isEmpty()) return; // Si el usuario no seleccionó nada

    // Extraer nombre corto del archivo
    int lastSlash = filename.lastIndexOf("/");
    QString shortName = filename.mid(lastSlash + 1);

    // Asegurar que la carpeta ./images/ existe
    QDir dir;
    if (!dir.exists("images")) {
        dir.mkdir("images");
    }

    // Ruta destino en carpeta local
    QString localPath = "./images/" + shortName;

    // Copiar solo si aún no existe
    if (!QFile::exists(localPath)) {
        if (!QFile::copy(filename, localPath)) {
            QMessageBox::warning(this, "Error", "No se pudo copiar la imagen.");
            return;
        }
    }

    // Intentar cargar la imagen
    QPixmap pixmap(localPath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Error", "No se pudo cargar la imagen seleccionada.");
        return;
    }

    // Mostrar la imagen
    ui->lblImage->setPixmap(pixmap);
    ui->lblImage->setScaledContents(true);
    imageFilePath = localPath;

    } //end loadImage

    QString AddItemDialogue::getBrand() {//agregado
        return ui ->txtBrand -> text();
}
    int AddItemDialogue::getSize() {
        return ui -> txtSize ->text().toInt();//convierte qlineedit a int
    }
    QString AddItemDialogue::getCategory() {
        return ui -> txtCategory -> text();
    }
    QString AddItemDialogue::getDeposit() {
        return ui -> txtDeposit -> text();
    }
