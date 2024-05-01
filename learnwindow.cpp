#include "learnwindow.h"
#include "ui_learnwindow.h"
#include<QTreeWidget>
#include<QTreeWidgetItem>
LearnWindow::LearnWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LearnWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->ansTextEdit->setVisible(false);
}

LearnWindow::~LearnWindow()
{
    delete ui;
}

void LearnWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current != nullptr) {
        if(current->text(0)=="摘录实例"){
            ui->mainStackedWidget->setCurrentIndex(0);
        }else if(current->text(0)=="问答示例"){
            ui->mainStackedWidget->setCurrentIndex(1);
        }
        ui->stackedWidget->setCurrentIndex(0);
    }

}


void LearnWindow::on_actTree_triggered()
{
    bool checked=ui->actTree->isChecked();
    if(checked){
        ui->dockWidget->show();
    }else{
        ui->dockWidget->hide();
    }
}


void LearnWindow::on_pushButton_7_clicked()
{
    ui->ansTextEdit->setVisible(true);
    ui->stackedWidget->setCurrentIndex(2);
}


void LearnWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

