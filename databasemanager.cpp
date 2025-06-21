
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
    //boorro claude
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}
/*
bool DatabaseManager::initializeDatabase() {
    // Configurar conexión a MySQL
    db = QSqlDatabase::addDatabase("QMYSQL");//agg claude
    db.setHostName("localhost");      // Cambiar por tu host
    db.setDatabaseName("inventario"); // Nombre de tu base de datos
    db.setUserName("userproyecto");           // Tu usuario MySQL
    db.setPassword("proyectooop");               // Tu contraseña MySQL
    db.setPort(3306);                // Puerto MySQL

    if (!db.open()) {
        qDebug() << "Error al conectar con MySQL:" << db.lastError().text();
        return false;
    }
*/
/*
bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // ← CORRECTO
    db.setHostName("localhost");
    db.setDatabaseName("inventario");
    db.setUserName("userproyecto");
    db.setPassword("proyectooop");
    db.setPort(3306);

    if (!db.open()) {
        qDebug() << "Error al conectar con MySQL:" << db.lastError().text();
        return false;
    }

    qDebug() << "Conexión exitosa a MySQL";
    return createTables();
}
*/
bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // CAMBIO: antes era QMYSQL
    db.setDatabaseName("inventario.db");       // Este será el archivo local
    if (!db.open()) {
        qDebug() << "Error al conectar con SQLite:" << db.lastError().text();
        return false;
    }

    qDebug() << "Conexión exitosa con SQLite";
    return createTables();
}


bool DatabaseManager::createTables() {
    QSqlQuery query;

    // Crear tabla de productos
    /*
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(255) NOT NULL,
            quantity INT NOT NULL,
            image_path VARCHAR(500),
            image_data LONGBLOB,
            brand VARCHAR(255),
            size INT,
            category VARCHAR(255),
            deposit VARCHAR(255),
            minimum_stock INT DEFAULT 5,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
        )
*/
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

    // Convertir imagen a datos binarios
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
        qDebug() << "Error insertando item:" << query.lastError().text();
        return false;
    }

    // Obtener el ID generado
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

    // Actualizar imagen si cambió
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
        qDebug() << "Error actualizando item:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::deleteItem(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error eliminando item:" << query.lastError().text();
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
