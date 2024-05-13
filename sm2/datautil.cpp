#include "datautil.h"
#include<QDate>
#include<mainwindow.h>
DataUtil *DataUtil::instance=nullptr;
DataUtil *DataUtil::getInstance()
{
    if(!instance){
        instance=new DataUtil;
    }
    return instance;
}

DataUtil::StartStatus DataUtil::start()
{
    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 读取上次启动的日期
    if(!settings){
        qDebug()<<"setting为空";
    }
    if(settings->contains("LastLaunchDate")){
        QDate lastLaunchDate = settings->value("LastLaunchDate").toDate();
        if (lastLaunchDate != currentDate) {
            // 今天还没有记录过启动时间，进行记录
            settings->setValue("LastLaunchDate", currentDate);

            // 执行启动操作
            // ...

            // 输出启动记录
            QDateTime currentTime = QDateTime::currentDateTime();
            QString logMessage = QString("启动时间： %1").arg(currentTime.toString());
            qDebug() << logMessage;
            return TodayStarted;
        }
    }else{
        // 今天还没有记录过启动时间，进行记录
        settings->setValue("LastLaunchDate", currentDate);

        // 执行启动操作
        // ...

        // 输出启动记录
        QDateTime currentTime = QDateTime::currentDateTime();
        QString logMessage = QString("启动时间： %1").arg(currentTime.toString());
        qDebug() << logMessage;
        return TodayFirstStart;
    }

}

int DataUtil::daysSinceLastStart()
{
    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 读取上次启动的日期
    if(!settings){
        qDebug()<<"fale";
    }
    if(settings->contains("LastLaunchDate")){
        QDate lastLaunchDate = settings->value("LastLaunchDate").toDate();
        if (lastLaunchDate != currentDate) {
            int days=currentDate.daysTo(lastLaunchDate);
            return days;
            // 今天还没有记录过启动时间，进行记录
         }
    }
    return 0;//表示第一次启动
}



DataUtil::DataUtil(QObject *parent)
    : QObject{parent}
{
    settings=MainWindow::getSettings();
}
