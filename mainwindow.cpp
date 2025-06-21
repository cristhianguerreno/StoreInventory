/*
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

/
    connect(ui->menuBrand, &QAction::triggered,
            this, &MainWindow::handleManageBrand);

    connect(ui->menuCategory, &QAction::triggered,
            this, &MainWindow::handleManageCategories);

    connect(ui->menuDeposit, &QAction::triggered,
            this, &MainWindow::handleManageDeposit);


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

#include "mainwindow.h"
#include "additemdialogue.h"
#include "ui_mainwindow.h"
#include "updateitemdialogue.h"
#include "databasemanager.h"
#include "usermanagerdialog.h"


#include <QMessageBox>
#include <QTimer>
#include <QBrush>
#include <QColor>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QString r, DatabaseManager* db, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    dbManager(db),
    role(r)

{
    ui->setupUi(this);

    if (role == "admin") {
        QAction* manageUsersAction = new QAction("Manage Users", this);
        connect(manageUsersAction, &QAction::triggered, this, [this]() {
            UserManagerDialog dlg(dbManager, this);
            dlg.exec();
        });

        ui->menuFile->addAction(manageUsersAction);
    }


    // Initialize database
    if (!dbManager->initializeDatabase()) {
        QMessageBox::critical(this, "Database Error",
                              "Could not connect to MySQL database");
    }

    // Load data from the database
    loadItemsFromDatabase();

    // Event Connections
    connect(ui->menuNewProduct, &QAction::triggered,
            this, &MainWindow::hanldeMenuItemNew);

    connect(ui->btnRemove, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct);

    connect(ui->lstProducts, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);

    connect(ui->menuEditSelectedProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);


    // Timer to check for low stock periodically
    QTimer* stockCheckTimer = new QTimer(this);
    connect(stockCheckTimer, &QTimer::timeout, this, &MainWindow::checkLowStock);
    stockCheckTimer->start(60000); // Verify every 5 minutes

    // Initial stock check
    checkLowStock();
}

MainWindow::~MainWindow()
{
    for (Item* product : productList) {
        delete product;
    }
    productList.clear();

    delete dbManager;
    delete ui;
}

void MainWindow::hanldeMenuItemNew()
{
    Item* newItem = nullptr;
    // AddItemDialogue addItemDialogue(newItem, nullptr);
    AddItemDialogue addItemDialogue(newItem, dbManager);

    addItemDialogue.setModal(true);
    if (addItemDialogue.exec() == QDialog::Accepted && newItem != nullptr) {
        // Save to database
        if (dbManager->insertItem(newItem)) {
            productList.push_back(newItem);
            addItemToList(newItem); // Method for adding with colors

            QMessageBox::information(this, "Success", "Product added successfully");
        } else {
            QMessageBox::warning(this, "Error", "The product could not be saved to the database");
            delete newItem;
        }
    }
}

void MainWindow::removeSelectedProduct()
{
    int index = ui->lstProducts->currentRow();
    if (index >= 0) {
        Item* theItem = productList.at(index);

        // Deletes from database
        if (dbManager->deleteItem(theItem->getId())) {
            delete theItem;
            productList.removeAt(index);
            delete ui->lstProducts->currentItem();

            // Clean the view
            clearProductDisplay();

            QMessageBox::information(this, "Success", "Product successfully removed");
        } else {
            QMessageBox::warning(this, "Error", "The product could not be deleted from the database");
        }
    }
}

void MainWindow::handleItemClick(QListWidgetItem* item)
{
    int index = item->listWidget()->currentRow();

    if (index != -1) {
        Item* currentItem = productList.at(index);
        if (currentItem != nullptr) {
            updateProductDisplay(currentItem);
        }
    }
}

void MainWindow::handleMenuItemEdit()
{
    int index = ui->lstProducts->currentRow();

    if (index != -1) {
        Item* currentItem = productList.at(index);
        if (currentItem != nullptr) {

            UpdateItemDIalogue updateItemDialogue(currentItem, dbManager);

            if (updateItemDialogue.exec() == QDialog::Accepted) {
                // Update in database
                if (dbManager->updateItem(currentItem)) {
                    // Update the visual list
                    updateItemInList(index, currentItem);
                    updateProductDisplay(currentItem);

                    QMessageBox::information(this, "Success", "Product updated successfully");
                } else {
                    QMessageBox::warning(this, "Error", "The product could not be updated in the database");
                }
            }
        }
    }
}

void MainWindow::handleSaveItems()
{
    // With MySQL, data is now saved automatically.
    QMessageBox::information(this, "Information",
                             "Data is automatically saved to the MySQL database");
}

void MainWindow::handleLoadItems()
{
    loadItemsFromDatabase();
    QMessageBox::information(this, "Success", "Data loaded from database");
}

void MainWindow::loadItemsFromDatabase()
{
    // Clear current list
    for (Item* temp : productList) {
        delete temp;
    }
    productList.clear();
    ui->lstProducts->clear();

    // Load from database
    productList = dbManager->getAllItems();

    for (Item* product : productList) {
        addItemToList(product);
    }
}

void MainWindow::addItemToList(Item* item)
{
    QString displayText = item->getName();

    // Add visual indicator if low stock
    if (item->isLowStock()) {
        displayText += " ⚠️ LOW STOCK";
    }

    QListWidgetItem* listItem = new QListWidgetItem(displayText);

    // Color according to stock
    if (item->isLowStock()) {
        listItem->setBackground(QBrush(QColor(255, 200, 200))); // Light red background
        listItem->setForeground(QBrush(QColor(139, 0, 0)));     // Dark red text
    } else if (item->getQuantity() <= item->getMinimumStock() + 5) {
        listItem->setBackground(QBrush(QColor(255, 255, 200))); // Light yellow background
        listItem->setForeground(QBrush(QColor(139, 139, 0)));   // Dark yellow text
    }

    ui->lstProducts->addItem(listItem);
}

void MainWindow::updateItemInList(int index, Item* item)
{
    QListWidgetItem* listItem = ui->lstProducts->item(index);
    if (listItem) {
        QString displayText = item->getName();

        if (item->isLowStock()) {
            displayText += " ⚠️ LOW STOCK";
            listItem->setBackground(QBrush(QColor(255, 200, 200)));
            listItem->setForeground(QBrush(QColor(139, 0, 0)));
        } else if (item->getQuantity() <= item->getMinimumStock() + 5) {
            listItem->setBackground(QBrush(QColor(255, 255, 200)));
            listItem->setForeground(QBrush(QColor(139, 139, 0)));
        } else {
            listItem->setBackground(QBrush(QColor(255, 255, 255))); // Normal white
            listItem->setForeground(QBrush(QColor(0, 0, 0)));       // Normal black
        }

        listItem->setText(displayText);
    }
}

void MainWindow::updateProductDisplay(Item* item)
{
    ui->lblProductName->setText(item->getName());
    ui->lblQuantity->setText(QString::number(item->getQuantity()));
    ui->lblBrand->setText(item->getBrand());
    ui->lblSize->setText(QString::number(item->getSize()));
    ui->lblCategory->setText(item->getCategory());
    ui->lblDeposit->setText(item->getDeposit());
    ui->lblMinimumStock->setText(QString::number(item->getMinimumStock())); // New label

    // Show stock status
    if (item->isLowStock()) {
        ui->lblStockStatus->setText("⚠️ LOW STOCK");
        ui->lblStockStatus->setStyleSheet("color: red; font-weight: bold;");
    } else if (item->getQuantity() <= item->getMinimumStock() + 5) {
        ui->lblStockStatus->setText("⚠️ AVERAGE STOCK");
        ui->lblStockStatus->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        ui->lblStockStatus->setText("✅ STOCK OK");
        ui->lblStockStatus->setStyleSheet("color: green; font-weight: bold;");
    }

    QPixmap pixmap(item->getImageFilePath());
    ui->lblImage->setPixmap(pixmap);
    ui->lblImage->setScaledContents(true);
}

void MainWindow::clearProductDisplay()
{
    QPixmap pixmap("none.png");
    ui->lblImage->setPixmap(pixmap);

    ui->lblProductName->setText("");
    ui->lblQuantity->setNum(0);
    ui->lblBrand->setText("");
    ui->lblSize->setText("");
    ui->lblCategory->setText("");
    ui->lblDeposit->setText("");
    ui->lblMinimumStock->setText("");
    ui->lblStockStatus->setText("");
}

void MainWindow::checkLowStock()
{
    QStringList lowStockItems;

    for (Item* item : productList) {
        if (item->isLowStock()) {
            lowStockItems.append(QString("%1 (Cantidad: %2, Mínimo: %3)")
                                     .arg(item->getName())
                                     .arg(item->getQuantity())
                                     .arg(item->getMinimumStock()));
        }
    }

    if (!lowStockItems.isEmpty()) {
        // Update window title with alert
        setWindowTitle("Inventory system - ⚠️ LOW STOCK (" +
                       QString::number(lowStockItems.size()) + " products)");

        // Optional: Show notification every so often
        static QTimer* notificationTimer = new QTimer(this);
        if (!notificationTimer->isActive()) {
            notificationTimer->start(300000); // Every 5 minutes

            QString message = "The following products are in low stock:\n\n" +
                              lowStockItems.join("\n");

            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle("Low Stock Alert");
            msgBox.setText(message);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    } else {
        setWindowTitle("Inventory System");
    }
}

