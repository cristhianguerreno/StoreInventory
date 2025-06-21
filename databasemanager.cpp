
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
    // db = QSqlDatabase::addDatabase("QMYSQL");
    //de claude
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // Before: MySql
    db.setDatabaseName("inventario.db");       // This would be the local file
    if (!db.open()) {
        qDebug() << "Error connecting to SQLite:" << db.lastError().text();
        return false;
    }

    qDebug() << "Successful connection to SQLite";

    if (!createTables()) {
        return false;
    }

    createDefaultUsers(); // ← ADD THIS LINE

    return true;
}



bool DatabaseManager::createTables() {
    QSqlQuery query;

//Create table
    QString createTableQuery = R"(
    CREATE TABLE IF NOT EXISTS products (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        quantity INTEGER NOT NULL,
        image_path TEXT,
        image_data BLOB,
        brand TEXT,
        size INTEGER,
        category TEXT,
        deposit TEXT,
        minimum_stock INTEGER DEFAULT 5,
        created_at TEXT DEFAULT (datetime('now')),
        updated_at TEXT DEFAULT (datetime('now'))
    )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creando tabla:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::insertItem(Item* item) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO products (name, quantity, image_path, image_data, brand, size, category, deposit, minimum_stock)
        VALUES (:name, :quantity, :image_path, :image_data, :brand, :size, :category, :deposit, :minimum_stock)
    )");

    query.bindValue(":name", item->getName());
    query.bindValue(":quantity", item->getQuantity());
    query.bindValue(":image_path", item->getImageFilePath());

    //Convert image to binary data
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

    // Obtain generated ID
    item->setId(query.lastInsertId().toInt());
    return true;
}

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

    query.bindValue(":id", item->getId());
    query.bindValue(":name", item->getName());
    query.bindValue(":quantity", item->getQuantity());
    query.bindValue(":image_path", item->getImageFilePath());

    // Update image if it changed
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

QList<Item*> DatabaseManager::getAllItems() {
    QList<Item*> items;
    QSqlQuery query("SELECT id, name, quantity, image_path, image_data, brand, size, category, deposit, minimum_stock FROM products");

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

QByteArray DatabaseManager::pixmapToByteArray(const QPixmap& pixmap) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return byteArray;
}

QPixmap DatabaseManager::byteArrayToPixmap(const QByteArray& byteArray) {
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    return pixmap;
}

void DatabaseManager::createDefaultUsers()
{
    QSqlQuery query;

    // Create users table if it does not exist
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

    // Insertar admin
    query.prepare("INSERT OR IGNORE INTO users (username, password, role) "
                  "VALUES ('admin', 'admin123', 'admin')");
    if (!query.exec()) {
        qDebug() << "Error inserting admin:" << query.lastError().text();
    }

    // Insertar user normal
    query.prepare("INSERT OR IGNORE INTO users (username, password, role) "
                  "VALUES ('user', 'user123', 'user')");
    if (!query.exec()) {
        qDebug() << "Error inserting user:" << query.lastError().text();
    }

    qDebug() << "Default users created (if they did not exist)";
}

