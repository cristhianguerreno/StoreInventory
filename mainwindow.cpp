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


    // If user has 'admin' role, show user management menu option
    if (role == "admin") {
        QAction* manageUsersAction = new QAction("Manage Users", this);
        connect(manageUsersAction, &QAction::triggered, this, [this]() {
            UserManagerDialog dlg(dbManager, this);
            dlg.exec();
        });
        ui->menuFile->addAction(manageUsersAction);
    }

    // Attempt to initialize database connection
    if (!dbManager->initializeDatabase()) {
        QMessageBox::critical(this, "Database Error",
                              "Could not connect to the MySQL database");
    }

    // Load inventory items from database on startup
    loadItemsFromDatabase();

    // Connect UI actions to their handlers
    connect(ui->menuNewProduct, &QAction::triggered,
            this, &MainWindow::hanldeMenuItemNew);
    connect(ui->btnRemove, &QPushButton::clicked,
            this, &MainWindow::removeSelectedProduct);
    connect(ui->lstProducts, &QListWidget::itemClicked,
            this, &MainWindow::handleItemClick);
    connect(ui->menuEditSelectedProduct, &QAction::triggered,
            this, &MainWindow::handleMenuItemEdit);


    /*
    // Set up timer to periodically check for low stock items
    QTimer* stockCheckTimer = new QTimer(this);
    connect(stockCheckTimer, &QTimer::timeout, this, &MainWindow::checkLowStock);
    stockCheckTimer->start(60000); // every 60 seconds  */

    // Do an initial check for low stock items
    checkLowStock();
}

MainWindow::~MainWindow()
{
    // Clean up dynamically allocated memory for products
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
    AddItemDialogue addItemDialogue(newItem, dbManager);

    addItemDialogue.setModal(true);
    // If dialog accepted and item was created
    if (addItemDialogue.exec() == QDialog::Accepted && newItem != nullptr) {
        if (dbManager->insertItem(newItem)) {
            productList.push_back(newItem);
            addItemToList(newItem); // visually add to list with stock coloring

            QMessageBox::information(this, "Success", "Product successfully added");
        } else {
            QMessageBox::warning(this, "Error", "Failed to insert product into database");
            delete newItem;
        }
    }
}

void MainWindow::removeSelectedProduct()
{
    int index = ui->lstProducts->currentRow();
    if (index >= 0) {
        Item* theItem = productList.at(index);

        // Attempt to remove item from database
        if (dbManager->deleteItem(theItem->getId())) {
            delete theItem;
            productList.removeAt(index);
            delete ui->lstProducts->currentItem();

            // Clear UI display since item was removed
            clearProductDisplay();

            QMessageBox::information(this, "Success", "Product successfully deleted");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete product from database");
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
                if (dbManager->updateItem(currentItem)) {
                    updateItemInList(index, currentItem);
                    updateProductDisplay(currentItem);

                    QMessageBox::information(this, "Success", "Product updated successfully");
                } else {
                    QMessageBox::warning(this, "Error", "Failed to update product in database");
                }
            }
        }
    }
}

void MainWindow::handleSaveItems()
{
    // Data is automatically saved in MySQL, no file save needed
    QMessageBox::information(this, "Information",
                             "Data is automatically saved in the MySQL database");
}

void MainWindow::handleLoadItems()
{
    loadItemsFromDatabase();
    QMessageBox::information(this, "Success", "Data loaded from database");
}

void MainWindow::loadItemsFromDatabase()
{
    // Clear current product list and UI list
    for (Item* temp : productList) {
        delete temp;
    }
    productList.clear();
    ui->lstProducts->clear();

    // Load products from the database and update UI list
    productList = dbManager->getAllItems();

    for (Item* product : productList) {
        addItemToList(product);
    }
}

void MainWindow::addItemToList(Item* item)
{
    QString displayText = item->getName();

    // Add warning label for low stock
    if (item->isLowStock()) {
        displayText += " ⚠️ LOW STOCK";
    }

    QListWidgetItem* listItem = new QListWidgetItem(displayText);

    // Apply color coding based on stock level
    if (item->isLowStock()) {
        listItem->setBackground(QBrush(QColor(255, 200, 200))); // Light red background
        listItem->setForeground(QBrush(QColor(139, 0, 0)));     // Dark red text
    } else if (item->getQuantity() <= item->getMinimumStock() + 5) {
        listItem->setBackground(QBrush(QColor(255, 255, 200))); // Light yellow
        listItem->setForeground(QBrush(QColor(139, 139, 0)));   // Dark yellow
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
            listItem->setBackground(QBrush(QColor(255, 255, 255))); // Default white background
            listItem->setForeground(QBrush(QColor(0, 0, 0)));       // Default black text
        }

        listItem->setText(displayText);
    }
}

void MainWindow::updateProductDisplay(Item* item)
{
    // Update UI labels with product information
    ui->lblProductName->setText(item->getName());
    ui->lblQuantity->setText(QString::number(item->getQuantity()));
    ui->lblBrand->setText(item->getBrand());
    ui->lblSize->setText(QString::number(item->getSize()));
    ui->lblCategory->setText(item->getCategory());
    ui->lblDeposit->setText(item->getDeposit());
    ui->lblMinimumStock->setText(QString::number(item->getMinimumStock()));

    // Show stock level warning
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

    // Display product image
    QPixmap pixmap(item->getImageFilePath());
    ui->lblImage->setPixmap(pixmap);
    ui->lblImage->setScaledContents(true);
}

void MainWindow::clearProductDisplay()
{
    // Reset product display labels and image
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
            lowStockItems.append(QString("%1 (Qty: %2, Min: %3)")
                                     .arg(item->getName())
                                     .arg(item->getQuantity())
                                     .arg(item->getMinimumStock()));
        }
    }

    if (!lowStockItems.isEmpty()) {
        // Update window title with low stock warning
        setWindowTitle("Inventory System - ⚠️ LOW STOCK (" +
                       QString::number(lowStockItems.size()) + " items)");

        // Show low stock alert once every 5 minutes
        static QTimer* notificationTimer = new QTimer(this);
        if (!notificationTimer->isActive()) {
            notificationTimer->start(300000); // 5 minutes

            QString message = "The following products are low on stock:\n\n" +
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


