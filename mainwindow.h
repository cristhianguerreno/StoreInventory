#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void hanldeMenuItemNew(); //creates slot for a clicking of new ite,
    void removeSelectedProduct();
    void handleItemClick(QListWidgetItem* item);//pass pointer
    void handleMenuItemEdit();//no se de donde salio
    void handleSaveItems();
    void handleLoadItems();
    void handleManageCategories();
    void handleManageBrand();
    void handleManageDeposit();
private:
    Ui::MainWindow *ui;
    QVector<Item*> productList;
};
#endif // MAINWINDOW_H
