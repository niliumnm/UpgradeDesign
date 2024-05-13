#ifndef LEARNWINDOW_H
#define LEARNWINDOW_H

#include <QMainWindow>
#include<QTreeWidgetItem>
#include<qtreewidget.h>
#include<QVector>
#include<sm2/smresponse.h>
#include<Sql/sqlUtil.h>
#include<sm2/datautil.h>
#include<QStandardItemModel>
#include<entity/topicbase.h>
namespace Ui {
class LearnWindow;
}

class LearnWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LearnWindow(QWidget *parent = nullptr);
    void getLearnQueue();
    void printQueue(QVector<TopicBase*>*sortedQueue);

    ~LearnWindow();

public slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actTree_triggered();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void receiveTreeModel(QStandardItemModel*model);
    void handleSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);


    QVector<TopicBase*>* getArticleQueue();
    QVector<TopicBase*>* getQuestionQueue();
    QVector<TopicBase*>* randomSort(QVector<TopicBase*>*articleQueue,QVector<TopicBase*>*questionQueue);

    QTreeView* getTreeviewPointer();

    void handleLearn();
    void handleResponse(int quality);
private:
    Ui::LearnWindow *ui;
    SqlUtil*sqlUtil;
    DataUtil*dataUtil;
    QStandardItemModel*m_model;
    QDate today;
    QVector<TopicBase*>* m_learnQueue=nullptr;
    bool learning = false;
signals:
    //点击学习和点击回答
    void learn();
    void postResponse(int quality);


};

#endif // LEARNWINDOW_H
