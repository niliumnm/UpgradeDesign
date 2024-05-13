#ifndef SMRESPONSE_H
#define SMRESPONSE_H

#include <QObject>
#include<QDate>
class SmResponse : public QObject
{
    Q_OBJECT
public:
    explicit SmResponse(QObject *parent = nullptr);
    SmResponse(int,int,double,QDate);
    SmResponse(const int64_t&id, const int&interval,const int&repetitions,const double&easeFactor,const QDate&lastReviewDate);
    SmResponse(const int&interval,const int&repetitions,const double&easeFactor);//计算用的
    SmResponse(const SmResponse& other);
    SmResponse& operator=(const SmResponse& other);
public:
    uint64_t id;
    int interval;
    int repetitions;
    double easeFactor;
    QDate lastReviewDate;

signals:
};

#endif // SMRESPONSE_H
