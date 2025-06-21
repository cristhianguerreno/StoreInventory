
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
    QSqlDatabase db; // ← ESTA LÍNEA ES CRUCIAL
    QByteArray pixmapToByteArray(const QPixmap& pixmap);
    QPixmap byteArrayToPixmap(const QByteArray& byteArray);

public:
    DatabaseManager();
    ~DatabaseManager();

    bool initializeDatabase();
    bool createTables();
    void createDefaultUsers();

    // CRUD operations
    bool insertItem(Item* item);
    bool updateItem(Item* item);
    bool deleteItem(int id);
    QList<Item*> getAllItems();
    Item* getItemById(int id);

    // Utility methods
    //QByteArray pixmapToByteArray(const QPixmap& pixmap);
    //QPixmap byteArrayToPixmap(const QByteArray& byteArray);
    bool saveImageToDatabase(const QString& imagePath);
};

#endif // DATABASEMANAGER_H
