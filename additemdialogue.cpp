#include "additemdialogue.h"
#include "ui_additemdialogue.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QIntValidator>

// Constructor
AddItemDialogue::AddItemDialogue(Item*& newItem, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddItemDialogue)
    , dbManager(dbManager)
{
    ui->setupUi(this);
    this->newItem = &newItem;

    // Set default image path
    imageFilePath = "none.png";

    // Set input validators
    ui->txtSize->setValidator(new QIntValidator(0, 10000, this));
    ui->sbMinimumStock->setRange(0, 1000);
    ui->sbMinimumStock->setValue(5);  // Default minimum stock

    // Connect button signals to slots
    connect(ui->btnConfirmAdd, &QPushButton::clicked, this, &AddItemDialogue::confirmAdd);
    connect(ui->btnLoadImage, &QPushButton::clicked, this, &AddItemDialogue::loadItemImage);

    // Ensure images folder exists
    QDir dir("images");
    if (!dir.exists()) {
        dir.mkdir(".");
    }
}

// Destructor
AddItemDialogue::~AddItemDialogue() {
    delete ui;
}

// Confirm button logic
void AddItemDialogue::confirmAdd() {
    QString productName = ui->txtProductName->text();
    int quantity = ui->sbQuantity->value();
    QString brand = getBrand();
    int size = getSize();
    QString category = getCategory();
    QString deposit = getDeposit();
    int minimumStock = getMinimumStock();

    if (!productName.trimmed().isEmpty() && quantity >= 1) {
        *newItem = new Item(productName, quantity, imageFilePath, brand,
                            size, category, deposit, minimumStock);

        // Warn if the new item is already low in stock
        if ((*newItem)->isLowStock()) {
            QMessageBox::warning(this, "Stock Warning",
                                 QString("WARNING: The item '%1' is already low in stock.\n"
                                         "Quantity: %2\nMinimum Stock: %3")
                                     .arg(productName)
                                     .arg(quantity)
                                     .arg(minimumStock));
        }

        this->close();
    } else {
        QMessageBox::warning(this, "Invalid Input", "You must provide a valid name and quantity of at least 1.");
    }
}

// Load image from file system
void AddItemDialogue::loadItemImage() {
    QString filename = QFileDialog::getOpenFileName(this, "Select Image", "./", "Image Files (*.png *.jpg)");
    if (filename.isEmpty()) return;

    int lastSlash = filename.lastIndexOf('/');
    QString shortName = filename.mid(lastSlash + 1);
    QString localPath = "./images/" + shortName;

    // Create local copy if it doesn't exist
    if (!QFile::exists(localPath)) {
        if (!QFile::copy(filename, localPath)) {
            QMessageBox::warning(this, "Error", "Failed to copy the selected image.");
            return;
        }
    }

    QPixmap pixmap(localPath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load the image.");
        return;
    }

    ui->lblImage->setPixmap(pixmap);
    ui->lblImage->setScaledContents(true);
    imageFilePath = localPath;
}

// Getter methods
QString AddItemDialogue::getBrand() {
    return ui->txtBrand->text();
}

int AddItemDialogue::getSize() {
    return ui->txtSize->text().toInt();
}

QString AddItemDialogue::getCategory() {
    return ui->txtCategory->text();
}

QString AddItemDialogue::getDeposit() {
    return ui->txtDeposit->text();
}

int AddItemDialogue::getMinimumStock() {
    return ui->sbMinimumStock->value();
}
