#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QObject>
#include<QSettings>
class DataUtil : public QObject
{
    Q_OBJECT

public:
    enum StartStatus{
        TodayFirstStart,
        TodayStarted,
        HugeGapSinceLast
    };
    Q_ENUM(StartStatus)

    static DataUtil*getInstance();
    //记录中写入今天启动 如果今天启动过就不重复写了
    StartStatus start();
    int daysSinceLastStart();
    void pullOffCard();

signals:

private:
    explicit DataUtil(QObject *parent = nullptr);
    static DataUtil*instance;
    QSettings *settings=nullptr;
};

#endif // DATAUTIL_H

/*


*/
