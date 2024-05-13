#include "learnwindow.h"
#include "ui_learnwindow.h"
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<Sql/sqlUtil.h>
#include<QSettings>
#include<mainwindow.h>
#include<QMessageBox>
#include<QVector>
#include<entity/topicbase.h>
#include<entity/question.h>
#include<entity/article.h>
#include<QRandomGenerator>
#include <algorithm> // 包含 std::shuffle
#include <random> // 包含 std::random_device 和 std::mt19937
LearnWindow::LearnWindow(QWidget * parent): QMainWindow(parent), ui(new Ui::LearnWindow) {
    ui -> setupUi(this);

    // ui->treeWidget->setModel(MainWindow);
    ui -> stackedWidget -> setCurrentIndex(0);
    ui -> ansTextEdit -> setVisible(false);
    sqlUtil=SqlUtil::getInstance();
    sqlUtil->getAllCard();

    QSettings* settings=MainWindow::getSettings();
    dataUtil=DataUtil::getInstance();

    QString str= settings->value(QString("General/savePath")).toString();
    qDebug()<<settings->fileName();

    today = QDate::currentDate();


    if(dataUtil->daysSinceLastStart()>1){
        QMessageBox::StandardButton answer = QMessageBox::warning(this, "提示", "您已经有一段时间未打开学习，是否将过去未学的卡片延期", QMessageBox::Yes | QMessageBox::No);

        if (answer == QMessageBox::Yes)
        {
            // 用户点击了"Yes"按钮
            // 执行相关操作...
        }
        else if (answer == QMessageBox::No)
        {
            // 用户点击了"No"按钮
            // 执行相关操作...
        }
    }

    getLearnQueue();
    if(!m_learnQueue){
        QMessageBox::warning(this,"警告","初始化失败，请重启软件");
    }
    connect(this,&LearnWindow::learn,this,&LearnWindow::handleLearn);
    connect(this,&LearnWindow::postResponse,this,&LearnWindow::handleResponse);

}

void LearnWindow::getLearnQueue()
{
    QVector<TopicBase*>*learnQueue=new QVector<TopicBase*>;
    QVector<TopicBase*>*articleQueue=getArticleQueue();
    QVector<TopicBase*>*questionQueue=getQuestionQueue();
    learnQueue= randomSort(articleQueue,questionQueue);
    m_learnQueue=learnQueue;

    // printQueue(questionQueue);

    /*
while (!itemQueue.isEmpty()) {
    BaseItem* item = itemQueue.dequeue();

    // 使用多态性执行相应的操作
    if (Topic* topic = dynamic_cast<Topic*>(item)) {
        // 处理 Topic 对象
    } else if (Response* response = dynamic_cast<Response*>(item)) {
        // 处理 Response 对象
    }

    // 释放对象的内存
    delete item;
}
    */
    /*
    QVector<SmResponse>allCard=sqlUtil->getAllCard();
    /*
     *
     *  关闭时记录上次启动时间
        记录上次启动时间1号，卡片里有间隔1 3 5
        遍历找出所有应该今天复习的
        如果发现有间隔+上次启动时间<今天的 | 今天-上次启动时间>1天的-->提示推迟
    */
    /*
    for(SmResponse card:allCard){
        //今天5.9 卡片5.8
        if(card.lastReviewDate.daysTo(today)>1){
            //把所有的卡片都后移card.lastReviewDate.daysTo(today)+1天
        }
    }
    getArticleQueue();
    */



}

void LearnWindow::printQueue(QVector<TopicBase*>*sortedQueue)
{

    for (TopicBase* item : *sortedQueue) {
        if (Article* article = dynamic_cast<Article*>(item)) {
            // item 是 Article 类型
            uint64_t id = article->getId();
            qDebug() << "Article ID:" << QString::number(id);
        } else if (Question* question = dynamic_cast<Question*>(item)) {
            // item 是 Question 类型
            uint64_t id = question->getId();
            qDebug() << "Question ID:" << QString::number(id);
        } else {
            qDebug() << "异常类";
        }
        // 添加更多的派生类判断条件，以覆盖所有可能的派生类类型
    }

}

LearnWindow::~LearnWindow() {
    if(正在学习){
        把取出来current的元素放回去
    }

    delete ui;
}

void LearnWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous) {
    if (current != nullptr) {
        if (current -> text(0) == "摘录实例") {
            ui -> mainStackedWidget -> setCurrentIndex(0);
        } else if (current -> text(0) == "问答示例") {
            ui -> mainStackedWidget -> setCurrentIndex(1);
        }
        ui -> stackedWidget -> setCurrentIndex(0);
    }

}

void LearnWindow::on_actTree_triggered() {
    bool checked = ui -> actTree -> isChecked();
    if (checked) {
        ui -> dockWidget -> show();
    } else {
        ui -> dockWidget -> hide();
    }
}

void LearnWindow::on_pushButton_7_clicked() {
    ui -> ansTextEdit -> setVisible(true);
    ui -> stackedWidget -> setCurrentIndex(2);
}

void LearnWindow::on_pushButton_10_clicked() {
    emit learn();
    learning=true;
    ui -> stackedWidget -> setCurrentIndex(1);
}

void LearnWindow::receiveTreeModel(QStandardItemModel *model)
{
    this->m_model=model;
    ui->treeView->setModel(m_model);
}

void LearnWindow::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{

}



void LearnWindow::handleLearn()
{
    if(m_learnQueue==nullptr){
        //learnQueue初始化失败
        QMessageBox::warning(this,"警告","初始化失败");
        return;
    }
    if(!(m_learnQueue->empty())){
        //mqueue不是空的
        TopicBase*currentTopic=m_learnQueue->first();

        //取出第一个元素

        //展示 判断类型

    }else{
        QMessageBox::warning(this,"恭喜","您已完成今天的复习");
    }
    if(队列还有)
        正在答题flag=1
    队头元素拷贝出来
    展示
    设置当前元素
}

void LearnWindow::handleResponse(int quality)
{
     计算+发送
}

QVector<TopicBase*>* LearnWindow::getArticleQueue()
{
    QVector<TopicBase*> *allArticle=sqlUtil->getAllArticle();
    QVector<TopicBase*> *res=new QVector<TopicBase*>;
    for(auto it = std::begin(*allArticle); it != std::end(*allArticle); ++it){
        TopicBase* article = *it;
        Article* derivedPtr = dynamic_cast<Article*>(article);
        SmResponse response;
        if(sqlUtil->getSmResponse(derivedPtr->getId(),response)){ //获取到对应的response 说明记录的有时间
            int i=response.lastReviewDate.daysTo(today);
            int j=response.interval;
            if(response.lastReviewDate.daysTo(today)==response.interval){
                res->push_back(article);
                qDebug()<<"getArticle"<<QString::number(derivedPtr->getId())<<" "<<QString::number(response.id);

            }
        }
    }
    return res;
}

QVector<TopicBase *> *LearnWindow::getQuestionQueue()
{
    QVector<TopicBase*> *allQuestion=sqlUtil->getAllQuestion();
    QVector<TopicBase*> *res=new QVector<TopicBase*>;
    for(auto it = std::begin(*allQuestion); it != std::end(*allQuestion); ++it){
        TopicBase* question = *it;
        Question* derivedPtr = dynamic_cast<Question*>(question);
        SmResponse response;
        if(sqlUtil->getSmResponse(derivedPtr->getId(),response)){ //获取到对应的response 说明记录的有时间
            int i=response.lastReviewDate.daysTo(today);
            int j=response.interval;
            if(response.lastReviewDate.daysTo(today)==response.interval){
                res->push_back(question);
                qDebug()<<"getQuestion"<<QString::number(derivedPtr->getId())<<" "<<QString::number(response.id);

            }
        }
    }
    return res;
}

QVector<TopicBase *> *LearnWindow::randomSort(QVector<TopicBase *> *articleQueue, QVector<TopicBase *> *questionQueue)
{
    QVector<TopicBase*>* mergedQueue = new QVector<TopicBase*>;

    // 合并第一个队列
    for (TopicBase* article : *articleQueue) {
        mergedQueue->append(article);
    }

    // 合并第二个队列
    for (TopicBase* question : *questionQueue) {
        mergedQueue->append(question);
    }

    // 随机排序
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(mergedQueue->begin(), mergedQueue->end(), gen);

    return mergedQueue;
}

QTreeView *LearnWindow::getTreeviewPointer()
{
    return ui->treeView;
}
