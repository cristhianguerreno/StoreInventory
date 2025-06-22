#include "additemdialogue.h"
#include "ui_additemdialogue.h"
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QIntValidator>
#include <QRegularExpression> // Replaced QRegExp with QRegularExpression

// Constructor for AddItemDialogue
AddItemDialogue::AddItemDialogue(Item*& newItem, DatabaseManager* dbManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddItemDialogue)
    , dbManager(dbManager)
    , newItem(&newItem)
{
    ui->setupUi(this);

    // Set default image path
    imageFilePath = "none.png";

    // Set validators for numeric inputs
    ui->txtSize->setValidator(new QIntValidator(0, 10000, this));
    ui->sbMinimumStock->setRange(0, 1000);
    ui->sbMinimumStock->setValue(5); // Default value for minimum stock

    // Limit the maximum number of characters for text inputs
    ui->txtProductName->setMaxLength(100);
    ui->txtBrand->setMaxLength(50);
    ui->txtCategory->setMaxLength(50);
    ui->txtDeposit->setMaxLength(50);

    // Connect buttons to their corresponding slots
    connect(ui->btnConfirmAdd, &QPushButton::clicked, this, &AddItemDialogue::confirmAdd);
    connect(ui->btnLoadImage, &QPushButton::clicked, this, &AddItemDialogue::loadItemImage);

    // Create "images" directory if it doesn't exist
    QDir dir("images");
    if (!dir.exists()) {
        dir.mkdir(".");
    }
}

// Destructor
AddItemDialogue::~AddItemDialogue() {
    delete ui;
}

// Slot executed when confirm button is clicked
void AddItemDialogue::confirmAdd() {
    QString productName = ui->txtProductName->text().trimmed();
    int quantity = ui->sbQuantity->value();
    QString brand = getBrand().trimmed();
    int size = getSize();
    QString category = getCategory().trimmed();
    QString deposit = getDeposit().trimmed();
    int minimumStock = getMinimumStock();

    // Validate input values
    QStringList errors;
    if (productName.isEmpty()) {
        errors << "Product name cannot be empty.";
    }
    if (quantity < 1) {
        errors << "Quantity must be at least 1.";
    }
    if (brand.isEmpty()) {
        errors << "Brand cannot be empty.";
    }
    if (size < 0) {
        errors << "Size cannot be negative.";
    }
    if (category.isEmpty()) {
        errors << "Category cannot be empty.";
    }
    if (deposit.isEmpty()) {
        errors << "Deposit cannot be empty.";
    }
    if (minimumStock < 0) {
        errors << "Minimum stock cannot be negative.";
    }

    // Validate product name using regular expression
    QRegularExpression nameRegex("^[A-Za-z0-9\\s-]+$");
    if (!productName.isEmpty() && !nameRegex.match(productName).hasMatch()) {
        errors << "Product name contains invalid characters (use letters, numbers, spaces, or hyphens).";
    }

    // Show warning if any validation errors exist
    if (!errors.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", errors.join("\n"));
        return;
    }

    // Create the new item instance with validated data
    *newItem = new Item(productName, quantity, imageFilePath, brand,
                        size, category, deposit, minimumStock);

    // Warn if the item is already low on stock
    if ((*newItem)->isLowStock()) {
        QMessageBox::warning(this, "Stock Warning",
                             QString("WARNING: The item '%1' is already low in stock.\n"
                                     "Quantity: %2\nMinimum Stock: %3")
                                 .arg(productName)
                                 .arg(quantity)
                                 .arg(minimumStock));
    }

    // Close the dialog with an accepted result
    this->accept();
}

// Slot executed when load image button is clicked
void AddItemDialogue::loadItemImage() {
    QString filename = QFileDialog::getOpenFileName(this, "Select Image", "./", "Image Files (*.png *.jpg)");

    // If no image is selected, show info message and keep default
    if (filename.isEmpty()) {
        QMessageBox::information(this, "No Image Selected", "No image was selected. Default image will be used.");
        return;
    }

    // Extract file name from full path
    int lastSlash = filename.lastIndexOf('/');
    QString shortName = filename.mid(lastSlash + 1);
    QString localPath = "./images/" + shortName;

    // Copy image to local path if it doesn't already exist
    if (!QFile::exists(localPath)) {
        if (!QFile::copy(filename, localPath)) {
            QMessageBox::warning(this, "Error", "Failed to copy the selected image.");
            return;
        }
    }

    // Load and display the image
    QPixmap pixmap(localPath);
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load the image. Please select a valid image file.");
        return;
    }

    ui->lblImage->setPixmap(pixmap);
    ui->lblImage->setScaledContents(true);
    imageFilePath = localPath;
}

// Getter for brand input
QString AddItemDialogue::getBrand() {
    return ui->txtBrand->text();
}

// Getter for size input
int AddItemDialogue::getSize() {
    return ui->txtSize->text().toInt();
}

// Getter for category input
QString AddItemDialogue::getCategory() {
    return ui->txtCategory->text();
}

// Getter for deposit input
QString AddItemDialogue::getDeposit() {
    return ui->txtDeposit->text();
}

// Getter for minimum stock value
int AddItemDialogue::getMinimumStock() {
    return ui->sbMinimumStock->value();
}
