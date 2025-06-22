#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include "item.h"

class DatabaseManager {
private:
    QSqlDatabase db; // Main database connection

    // Converts a QPixmap to a QByteArray (for storing in the database)
    QByteArray pixmapToByteArray(const QPixmap& pixmap);

    // Converts a QByteArray back to a QPixmap (for displaying images)
    QPixmap byteArrayToPixmap(const QByteArray& byteArray);

public:
    DatabaseManager();
    ~DatabaseManager();

    // Initializes and opens the database connection
    bool initializeDatabase();

    // Creates required tables if they do not exist
    bool createTables();

    // Creates default users if none exist
    void createDefaultUsers();

    // CRUD operations for Item
    bool insertItem(Item* item);
    bool updateItem(Item* item);
    bool deleteItem(int id);
    QList<Item*> getAllItems();
    Item* getItemById(int id);

    // Saves an image to the database from a given file path
    bool saveImageToDatabase(const QString& imagePath);
};

#endif // DATABASEMANAGER_H
