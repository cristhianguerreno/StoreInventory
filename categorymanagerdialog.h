/*
#ifndef CATEGORYMANAGERDIALOG_H
#define CATEGORYMANAGERDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QVector>
#include "Item.h"

namespace Ui {
class CategoryManagerDialog;
}

class CategoryManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryManagerDialog(QWidget *parent = nullptr);
    ~CategoryManagerDialog();

private slots:
    void handleCategoryClick(QListWidgetItem* item);
    void handleAddCategory();
    void handleEditCategory();
    void handleDeleteCategory();
    void handleSaveCategories();

private:
    Ui::CategoryManagerDialog *ui;
    QVector<QString> categoryList;
    QVector<Item*> allProducts; // Para filtrar productos por categoría

    void loadCategories();
    void loadAllProducts();
    void filterProductsByCategory(const QString& category);
    void refreshCategoryList();
};

#endif // CATEGORYMANAGERDIALOG_H
*/
