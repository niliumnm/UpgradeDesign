#ifndef LEARNWINDOW_H
#define LEARNWINDOW_H

#include <QMainWindow>
#include<QTreeWidgetItem>
#include<qtreewidget.h>
namespace Ui {
class LearnWindow;
}

class LearnWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LearnWindow(QWidget *parent = nullptr);
    ~LearnWindow();

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);


    void on_actTree_triggered();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::LearnWindow *ui;
};

#endif // LEARNWINDOW_H
