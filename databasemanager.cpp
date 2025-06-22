#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QFile>
#include "item.h"
#include "qvariant.h"

DatabaseManager::DatabaseManager() {
    // Constructor (no database connection yet)
}

DatabaseManager::~DatabaseManager() {
    // Closes the database connection if it is still open
    if (db.isOpen()) {
        db.close();
    }
}

// Initializes and connects to the SQLite database
// Also creates the necessary tables and default users
bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // Use SQLite as database engine
    db.setDatabaseName("inventario.db");       // Local database file

    if (!db.open()) {
        qDebug() << "Failed to connect to SQLite:" << db.lastError().text();
        return false;
    }

    qDebug() << "Successfully connected to SQLite";

    // Create required tables
    if (!createTables()) {
        return false;
    }

    // Insert default admin and user accounts
    createDefaultUsers();

    return true;
}

// Creates the 'products' table if it doesn't exist
bool DatabaseManager::createTables() {
    QSqlQuery query;

    // SQL command to create a table to store products and item data
    QString createTableQuery = R"(
    CREATE TABLE IF NOT EXISTS products (
        id INTEGER PRIMARY KEY AUTOINCREMENT,       -- Unique product ID
        name TEXT NOT NULL,                         -- Product name
        quantity INTEGER NOT NULL,                  -- Quantity in stock
        image_path TEXT,                            -- Path to image file
        image_data BLOB,                            -- Binary image data
        brand TEXT,                                 -- Brand name
        size INTEGER,                               -- Size of product
        category TEXT,                              -- Product category
        deposit TEXT,                               -- Deposit location
        minimum_stock INTEGER DEFAULT 5,            -- Minimum stock before warning
        created_at TEXT DEFAULT (datetime('now')),  -- Timestamp of creation
        updated_at TEXT DEFAULT (datetime('now'))   -- Timestamp of last update
    )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating products table:" << query.lastError().text();
        return false;
    }

    return true;
}

// Inserts a new item into the 'products' table
bool DatabaseManager::insertItem(Item* item) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO products (name, quantity, image_path, image_data, brand, size, category, deposit, minimum_stock)
        VALUES (:name, :quantity, :image_path, :image_data, :brand, :size, :category, :deposit, :minimum_stock)
    )");

    // Bind values from the Item object to the SQL query
    query.bindValue(":name", item->getName());
    query.bindValue(":quantity", item->getQuantity());
    query.bindValue(":image_path", item->getImageFilePath());

    // Convert image file into binary data and bind it
    QPixmap pixmap(item->getImageFilePath());
    if (!pixmap.isNull()) {
        QByteArray imageData = pixmapToByteArray(pixmap);
        query.bindValue(":image_data", imageData);
    } else {
        query.bindValue(":image_data", QVariant());
    }

    query.bindValue(":brand", item->getBrand());
    query.bindValue(":size", item->getSize());
    query.bindValue(":category", item->getCategory());
    query.bindValue(":deposit", item->getDeposit());
    query.bindValue(":minimum_stock", item->getMinimumStock());

    if (!query.exec()) {
        qDebug() << "Error inserting item:" << query.lastError().text();
        return false;
    }

    // Store the generated ID back into the Item object
    item->setId(query.lastInsertId().toInt());
    return true;
}

// Updates an existing item in the 'products' table by ID
bool DatabaseManager::updateItem(Item* item) {
    QSqlQuery query;
    query.prepare(R"(
        UPDATE products SET
            name = :name,
            quantity = :quantity,
            image_path = :image_path,
            image_data = :image_data,
            brand = :brand,
            size = :size,
            category = :category,
            deposit = :deposit,
            minimum_stock = :minimum_stock
        WHERE id = :id
    )");

    // Bind updated values from the Item object
    query.bindValue(":id", item->getId());
    query.bindValue(":name", item->getName());
    query.bindValue(":quantity", item->getQuantity());
    query.bindValue(":image_path", item->getImageFilePath());

    // Convert and bind updated image
    QPixmap pixmap(item->getImageFilePath());
    if (!pixmap.isNull()) {
        QByteArray imageData = pixmapToByteArray(pixmap);
        query.bindValue(":image_data", imageData);
    } else {
        query.bindValue(":image_data", QVariant());
    }

    query.bindValue(":brand", item->getBrand());
    query.bindValue(":size", item->getSize());
    query.bindValue(":category", item->getCategory());
    query.bindValue(":deposit", item->getDeposit());
    query.bindValue(":minimum_stock", item->getMinimumStock());

    if (!query.exec()) {
        qDebug() << "Error updating item:" << query.lastError().text();
        return false;
    }

    return true;
}

// Deletes an item from the database based on its ID
bool DatabaseManager::deleteItem(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error deleting item:" << query.lastError().text();
        return false;
    }

    return true;
}

// Retrieves and returns a list of all items from the database
QList<Item*> DatabaseManager::getAllItems() {
    QList<Item*> items;
    QSqlQuery query("SELECT id, name, quantity, image_path, image_data, brand, size, category, deposit, minimum_stock FROM products");

    // Loop through all rows returned and create Item objects
    while (query.next()) {
        Item* item = new Item(
            query.value("name").toString(),
            query.value("quantity").toInt(),
            query.value("image_path").toString(),
            query.value("brand").toString(),
            query.value("size").toInt(),
            query.value("category").toString(),
            query.value("deposit").toString(),
            query.value("minimum_stock").toInt(),
            query.value("id").toInt()
            );

        items.append(item);
    }

    return items;
}

// Retrieves a single item from the database using its ID
Item* DatabaseManager::getItemById(int id) {
    QSqlQuery query;
    query.prepare("SELECT id, name, quantity, image_path, image_data, brand, size, category, deposit, minimum_stock FROM products WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return new Item(
            query.value("name").toString(),
            query.value("quantity").toInt(),
            query.value("image_path").toString(),
            query.value("brand").toString(),
            query.value("size").toInt(),
            query.value("category").toString(),
            query.value("deposit").toString(),
            query.value("minimum_stock").toInt(),
            query.value("id").toInt()
            );
    }

    return nullptr;
}

// Converts a QPixmap object into a byte array (used for storing in DB)
QByteArray DatabaseManager::pixmapToByteArray(const QPixmap& pixmap) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return byteArray;
}

// Converts a byte array from DB back into a QPixmap image
QPixmap DatabaseManager::byteArrayToPixmap(const QByteArray& byteArray) {
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    return pixmap;
}

// Creates the 'users' table and inserts default admin and user accounts
void DatabaseManager::createDefaultUsers() {
    QSqlQuery query;

    // SQL command to create the users table
    QString createUserTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role TEXT NOT NULL
        )
    )";

    if (!query.exec(createUserTableQuery)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return;
    }

    // Insert admin account if not already exists
    query.prepare("INSERT OR IGNORE INTO users (username, password, role) "
                  "VALUES ('admin', 'admin123', 'admin')");
    if (!query.exec()) {
        qDebug() << "Error inserting default admin:" << query.lastError().text();
    }

    // Insert standard user account if not already exists
    query.prepare("INSERT OR IGNORE INTO users (username, password, role) "
                  "VALUES ('user', 'user123', 'user')");
    if (!query.exec()) {
        qDebug() << "Error inserting default user:" << query.lastError().text();
    }

    qDebug() << "Default users created (if they didn't already exist)";
}
