#include "mainwindow.h"
#include "additemdialogue.h"
#include "ui_mainwindow.h"
#include "updateitemdialogue.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect the action of triggering menuNewProduct
    //to the function in MainWindow::hanldeMenuItemNew
    connect(ui->menuNewProduct, &QAction::triggered,
            this, &MainWindow::hanldeMenuItemNew) ;

    connect(ui->btnRemove, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct) ;

    connect(ui->lstProducts, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);

    connect(ui->menuEditSelectedProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);

    connect(ui->menuSaveProducts, &QAction::triggered,
            this, &MainWindow::handleSaveItems);

    connect(ui->menuLoadProducts, &QAction::triggered,
            this, &MainWindow::handleLoadItems);

/*
    connect(ui->menuBrand, &QAction::triggered,
            this, &MainWindow::handleManageBrand);

    connect(ui->menuCategory, &QAction::triggered,
            this, &MainWindow::handleManageCategories);

    connect(ui->menuDeposit, &QAction::triggered,
            this, &MainWindow::handleManageDeposit);
*/

};

MainWindow::~MainWindow()
{
    //free up product list memory
    for (Item* product : productList) //for ptr in list
    {
        delete product;
    }
    productList.clear();

    delete ui;
}

void MainWindow::hanldeMenuItemNew()
{
    //creats a menu item
    Item* newItem = nullptr; //initializes the nullptr because doesnt point anything yet
    AddItemDialogue addItemDialogue(newItem, nullptr); //pass new item by reference

    addItemDialogue.setModal(true);
    addItemDialogue.exec();

    if ( newItem != nullptr ) //   if is not null it means that something has been selected
    {
        //introducting the full item data
        productList.push_back(newItem); //push newitem in the list
        //because we do this at same the they will be parallel arrays
        ui->lstProducts->addItem(newItem->getName()); //just for viewing

    }
}

void MainWindow::removeSelectedProduct()
{
    int index= ui->lstProducts->currentRow();
    if (index>=0)
    {
        //remove from vector
        Item* theItem = productList.at(index);
        delete theItem;
        productList.removeAt(index);
        //remove from list widget
        delete ui->lstProducts->currentItem();
    } //end if

    // set image to"none.png"
    QPixmap pixmap("none.png");
    ui->lblImage->setPixmap(pixmap);

    ui->lblProductName->setText("");
    ui->lblQuantity->setNum(0);
    ui->lblBrand->setText("");
    ui->lblSize->setText("");
    ui->lblCategory->setText("");
    ui->lblDeposit->setText("");


} //end of removeSelectedProduct

void MainWindow::handleItemClick(QListWidgetItem* item){
    int index= item->listWidget()->currentRow();

    if (index!=-1) //if its -1 means nothing valid was selected
    {
        Item* currentItem = productList.at(index);
        if (currentItem!= nullptr)
        {
            ui-> lblProductName -> setText(currentItem -> getName());
            ui-> lblQuantity -> setText(QString::number(currentItem -> getQuantity()));
            ui->lblBrand->setText(currentItem->getBrand());//cambiado
            ui->lblSize->setText(QString::number(currentItem->getSize()));
            ui->lblCategory->setText(currentItem->getCategory());
            ui->lblDeposit->setText(currentItem->getDeposit());


            QPixmap pixmap(currentItem -> getImageFilePath());
            ui-> lblImage -> setPixmap(pixmap);
            ui-> lblImage -> setScaledContents(true);

        } //end if


    }//end   if
}//end handleItemClick

void MainWindow::handleMenuItemEdit()
{
    int index = ui->lstProducts->currentRow();

    if (index !=  -1) //cht pq si es 0 apunta al primero
    {
        Item* currentItem = productList.at(index);
        if (currentItem != nullptr)
        {
            UpdateItemDIalogue updateItemDialogue(currentItem, nullptr);
            updateItemDialogue.exec();

            ui->lstProducts->currentItem()->setText(currentItem->getName()); //aggchgpt


            //make sure UI is updated
            ui->lblProductName -> setText(currentItem->getName());
            ui->lblQuantity -> setText(QString::number(currentItem->getQuantity()));
            ui->lblBrand ->setText(currentItem->getBrand()); //cambiado
            ui->lblSize ->setText(QString::number(currentItem->getSize()));
            ui->lblCategory ->setText(currentItem->getCategory());
            ui->lblDeposit ->setText(currentItem->getDeposit());

            QPixmap pixmap(currentItem -> getImageFilePath());
            ui->lblImage ->setPixmap(pixmap);
            ui->lblImage->setScaledContents(true);

        } //end inner if


    }        //end if
} //end handleMenuItem


void MainWindow::handleSaveItems()
{
    QFile outputFile("products.txt");

    outputFile.open(QIODevice::WriteOnly |
                    QIODevice::Text);

    QTextStream out(&outputFile);

    for (Item* product : productList) //range base for loop
    {
        out<<product->getName()<<",";
        out<<product->getQuantity()<<",";
        out<<product->getImageFilePath()<<",";
        out<<product->getBrand()<<",";
        out<<product->getSize()<<",";
        out<<product->getCategory()<<",";
        out<<product->getDeposit()<<Qt::endl;
    } //end for


    out.flush();
    outputFile.close();


} //end handsaveitems

void MainWindow::handleLoadItems()
{
    QFile inputFile("products.txt");

    inputFile.open(QIODevice::ReadOnly |
                   QIODevice::Text);
    QTextStream in(&inputFile);

    //clear current list and vector
    for (Item* temp : productList) // for item pointer temp in productlist //if dont some data can be duplicate
    {
        delete temp; //delete     memory
    } //end for

    productList.clear();//data model actual data
    ui->lstProducts-> clear(); // viewer list

    while (! in.atEnd())
    {
        QString line= in.readLine();
        QStringList info=line.split(","); //we telling to split every time , is present

        if (info.size() >= 7) {//cambiar si se quiero meter mas datos
            // Agregar al list widget
            ui->lstProducts->addItem(info.at(0));


        //handle vector     ///cambiado para meter mas cosas
            Item* product = new Item (info.at(0),
                                 info.at(1).toInt(), info.at(2), info.at(3),
                                     info.at(4).toInt(), info.at(5),info.at(6)); //name, quantity, image path

        productList.push_back(product);

    }
    }
        //end while

    in.flush();
    inputFile.close();

} //endhandloadItems

/*
void MainWindow::handleManageBrand()
{
    BrandManagerDialog brandDialog(this);
    brandDialog.setModal(true);
    brandDialog.exec();
}//end handlemanagebrand

void MainWindow::handleManageCategories()
{
    // Crear y mostrar el diálogo de gestión de categorías
    CategoryManagerDialog categoryDialog(this);
    categoryDialog.setModal(true);
    categoryDialog.exec();
}//endhandlemanagecategories

void MainWindow::handleManageDeposit()
{
    DepositManagerDialog depositDialog(this);
    depositDialog.setModal(true);
    depositDialog.exec();
}//end handlemanagedeposit
*/
