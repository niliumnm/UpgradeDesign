#ifndef SM_H
#define SM_H

#include <QObject>
#include<sm2/smresponse.h>

class sm : public QObject
{
    Q_OBJECT
public:
    explicit sm(QObject *parent = nullptr);
    SmResponse calc(int quality, int repetitions, int previousInterval, double previousEaseFactor);
    void sortQueue(QVector<SmResponse>&LearnQueue);
    void QuickSort(QVector<SmResponse>&LearnQueue,int low,int high);

signals:
};

#endif // SM_H
