/*
#include "categorymanagerdialog.h"
#include "ui_categorymanagerdialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>

CategoryManagerDialog::CategoryManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CategoryManagerDialog)
{
    ui->setupUi(this);

    // Configurar título de la ventana
    setWindowTitle("Category Manager");

    // Conectar señales y slots
    connect(ui->lstCategories, &QListWidget::itemClicked,
            this, &CategoryManagerDialog::handleCategoryClick);

    connect(ui->btnAddCategory, &QPushButton::clicked,
            this, &CategoryManagerDialog::handleAddCategory);

    connect(ui->btnEditCategory, &QPushButton::clicked,
            this, &CategoryManagerDialog::handleEditCategory);

    connect(ui->btnDeleteCategory, &QPushButton::clicked,
            this, &CategoryManagerDialog::handleDeleteCategory);

    connect(ui->btnSaveCategories, &QPushButton::clicked,
            this, &CategoryManagerDialog::handleSaveCategories);

    // Cargar datos iniciales
    loadCategories();
    loadAllProducts();
}

CategoryManagerDialog::~CategoryManagerDialog()
{
    delete ui;
}

void CategoryManagerDialog::loadCategories()
{
    // Por ahora usaremos un archivo temporal, después migraremos a BD
    QFile inputFile("categories.txt");

    categoryList.clear();
    ui->lstCategories->clear();

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);

        while (!in.atEnd()) {
            QString category = in.readLine().trimmed();
            if (!category.isEmpty()) {
                categoryList.push_back(category);
                ui->lstCategories->addItem(category);
            }
        }
        inputFile.close();
    }

    // Si no hay categorías, agregar algunas por defecto
    if (categoryList.isEmpty()) {
        categoryList << "Alimentos" << "Bebidas" << "Limpieza" << "Higiene Personal";
        refreshCategoryList();
    }
}

void CategoryManagerDialog::loadAllProducts()
{
    // Cargar todos los productos del archivo existente
    QFile inputFile("products.txt");

    // Limpiar productos anteriores
    for (Item* temp : allProducts) {
        delete temp;
    }
    allProducts.clear();

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&inputFile);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList info = line.split(",");

            if (info.size() >= 7) {
                Item* product = new Item(info.at(0),           // name
                                         info.at(1).toInt(),     // quantity
                                         info.at(2),             // image path
                                         info.at(3),             // brand
                                         info.at(4).toInt(),     // size
                                         info.at(5),             // category
                                         info.at(6));            // deposit
                allProducts.push_back(product);
            }
        }
        inputFile.close();
    }
}

void CategoryManagerDialog::handleCategoryClick(QListWidgetItem* item)
{
    if (item != nullptr) {
        QString selectedCategory = item->text();
        filterProductsByCategory(selectedCategory);
    }
}

void CategoryManagerDialog::filterProductsByCategory(const QString& category)
{
    ui->lstProducts->clear();

    for (Item* product : allProducts) {
        if (product->getCategory() == category) {
            ui->lstProducts->addItem(product->getName() +
                                     " (Qty: " + QString::number(product->getQuantity()) + ")");
        }
    }
}

void CategoryManagerDialog::handleAddCategory()
{
    bool ok;
    QString newCategory = QInputDialog::getText(this, "Add Category",
                                                "Category name:", QLineEdit::Normal,
                                                "", &ok);

    if (ok && !newCategory.trimmed().isEmpty()) {
        newCategory = newCategory.trimmed();

        // Verificar que no exista ya
        if (!categoryList.contains(newCategory)) {
            categoryList.push_back(newCategory);
            ui->lstCategories->addItem(newCategory);
        } else {
            QMessageBox::information(this, "Category Exists",
                                     "This category already exists.");
        }
    }
}

void CategoryManagerDialog::handleEditCategory()
{
    int index = ui->lstCategories->currentRow();

    if (index >= 0 && index < categoryList.size()) {
        QString currentCategory = categoryList.at(index);

        bool ok;
        QString newCategory = QInputDialog::getText(this, "Edit Category",
                                                    "Category name:", QLineEdit::Normal,
                                                    currentCategory, &ok);

        if (ok && !newCategory.trimmed().isEmpty()) {
            newCategory = newCategory.trimmed();

            // Actualizar en la lista
            categoryList[index] = newCategory;
            ui->lstCategories->currentItem()->setText(newCategory);

            // Actualizar productos que tengan esta categoría
            for (Item* product : allProducts) {
                if (product->getCategory() == currentCategory) {
                    product->setCategory(newCategory);
                }
            }

            QMessageBox::information(this, "Category Updated",
                                     "Category and related products updated successfully.");
        }
    } else {
        QMessageBox::warning(this, "No Selection",
                             "Please select a category to edit.");
    }
}

void CategoryManagerDialog::handleDeleteCategory()
{
    int index = ui->lstCategories->currentRow();

    if (index >= 0 && index < categoryList.size()) {
        QString categoryToDelete = categoryList.at(index);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Category",
                                      "Are you sure you want to delete the category '" +
                                          categoryToDelete + "'?\n\nProducts in this category will not be deleted, but their category will be cleared.",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Remover de la lista
            categoryList.removeAt(index);
            delete ui->lstCategories->takeItem(index);

            // Limpiar categoría de productos relacionados
            for (Item* product : allProducts) {
                if (product->getCategory() == categoryToDelete) {
                    product->setCategory(""); // Limpiar categoría
                }
            }

            // Limpiar lista de productos mostrados
            ui->lstProducts->clear();
        }
    } else {
        QMessageBox::warning(this, "No Selection",
                             "Please select a category to delete.");
    }
}

void CategoryManagerDialog::handleSaveCategories()
{
    // Guardar categorías
    QFile outputFile("categories.txt");

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&outputFile);

        for (const QString& category : categoryList) {
            out << category << Qt::endl;
        }

        outputFile.close();
    }

    // Guardar productos actualizados
    QFile productsFile("products.txt");

    if (productsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&productsFile);

        for (Item* product : allProducts) {
            out << product->getName() << ",";
            out << product->getQuantity() << ",";
            out << product->getImageFilePath() << ",";
            out << product->getBrand() << ",";
            out << product->getSize() << ",";
            out << product->getCategory() << ",";
            out << product->getDeposit() << Qt::endl;
        }

        productsFile.close();
    }

    QMessageBox::information(this, "Saved", "Categories and products saved successfully.");
}

void CategoryManagerDialog::refreshCategoryList()
{
    ui->lstCategories->clear();
    for (const QString& category : categoryList) {
        ui->lstCategories->addItem(category);
    }
}
*/
