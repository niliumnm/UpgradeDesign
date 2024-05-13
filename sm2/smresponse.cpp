#include "smresponse.h"

SmResponse::SmResponse(QObject *parent)
    : QObject{parent}
{


}

SmResponse::SmResponse(int interval, int repetitions, double easeFactor,QDate lastReviewDate): interval(interval),repetitions(repetitions),easeFactor(easeFactor),lastReviewDate(lastReviewDate)
{

}

SmResponse::SmResponse(const int64_t &id, const int &interval, const int &repetitions, const double &easeFactor, const QDate &lastReviewDate):id(id),interval(interval),repetitions(repetitions),easeFactor(easeFactor),lastReviewDate(lastReviewDate)
{

}

SmResponse::SmResponse(const int &interval, const int &repetitions, const double &easeFactor):interval(interval),repetitions(repetitions),easeFactor(easeFactor)
{

}

SmResponse::SmResponse(const SmResponse &other):QObject(other.parent()), interval(other.interval), repetitions(other.repetitions), easeFactor(other.easeFactor),lastReviewDate(other.lastReviewDate)
{

}

SmResponse &SmResponse::operator=(const SmResponse &other)
{
    if (this != &other)
    {
        interval = other.interval;
        repetitions = other.repetitions;
        easeFactor = other.easeFactor;
        lastReviewDate=other.lastReviewDate;
        // 可以根据需要进行其他成员变量的赋值
    }
    return *this;
}
