#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QTextCharFormat>
#include<QFontComboBox>
#include<QStandardItem>
#include<QTreeWidgetItem>
#include<QStandardItemModel>
#include"Sql/idgenerator.h"
#include<QStack>
#include"sql/sqlUtil.h"
#include<QSettings>
#include<learnwindow.h>
#include<sm2/datautil.h>
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
    void mergeFormat(QTextCharFormat fmt);
    bool boldcheck();
    bool italiccheck();
    bool underlinecheck();
    void saveTreeViewToJson(QStandardItem* item, QJsonObject& jsonObject);
    void saveSubTreeToJson(QTreeWidgetItem* item, QJsonObject& jsonObject);
    void test();
    static QSettings *getSettings();
    void setSettings(QSettings *newSettings);
    void initTreeMenu();
    void setDebugOutput(const QString &targetFilePath, const bool &argDateFlag = false);
    ~MainWindow();


private slots:

    void on_btnLearnMode_clicked();

    void on_actBold_triggered();

    void on_actItalic_triggered();

    void on_actUnderline_triggered();

    void on_actAlignL_triggered();

    void on_actAlignR_triggered();

    void on_actAlignC_triggered();

    void on_actAddList_triggered();

    void on_actRandomList_triggered();

    void actH(int size);

    void on_actH1_triggered();

    void on_actH2_triggered();

    void on_actH3_triggered();

    void on_actH4_triggered();

    void on_actH5_triggered();

    void on_actH6_triggered();

    void on_actAddImg_triggered();

    void on_actOutput_triggered();

    void on_actInput_triggered();

    void on_FontComboChanged(const QFont&);

    void do_changeComboFont();

    void on_actTable_triggered();

    void on_actLast_triggered();

    void on_actNext_triggered();

    void restoreTreeViewFromJson(const QJsonObject& jsonObject, QStandardItem* parentItem);


    void on_actClip_triggered();

    void on_actFind_triggered();

    void restoreTree();

    void saveTree();

    void setTreeTestData();

    void do_treeSelectChanged(const QModelIndex &current, const QModelIndex &previous);

    void do_saveNote();

    void derorateText(QString &s);


    void on_actNewNote_triggered();

    void on_actDel_triggered();

    void on_actLearn_triggered();

    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void OnRightClicked(const QPoint& point);



private:
    Ui::MainWindow *ui;
    QLabel*labelStatus;
    QFontComboBox *fontComboBox;
    bool dontChangeFont=false;
    QStandardItemModel* m_standardItemModel=nullptr;
    IdGenerator *generator;
    QItemSelectionModel* selectionModel;
    QModelIndex  lastTreeNode;
    QModelIndex  nextTreeNode;
    SqlUtil*sqlUtil;
    LearnWindow*learnWindow=nullptr;
    static QSettings*settings;
    DataUtil*dataUtil;
    QMenu*treeMenu=nullptr;

signals:
    void transferTreeModel(QStandardItemModel*model);

};
#endif // MAINWINDOW_H
