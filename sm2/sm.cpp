#include "sm.h"

sm::sm(QObject *parent)
    : QObject{parent}
{}

SmResponse sm::calc(int quality, int repetitions, int previousInterval, double previousEaseFactor)
{
    int interval;
    double easeFactor;

    if (quality >= 3) {
        switch (repetitions) {
        case 0:
            interval = 1;
            break;
        case 1:
            interval = 6;
            break;
        default:
            interval = std::round(previousInterval * previousEaseFactor);
            break;
        }

        repetitions++;
        easeFactor = previousEaseFactor + (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
    } else {
        repetitions = 0;
        interval = 1;
        easeFactor = previousEaseFactor;
    }

    if (easeFactor < 1.3) {
        easeFactor = 1.3;
    }

    return SmResponse(interval, repetitions, easeFactor);
}

void sm::sortQueue(QVector<SmResponse> &LearnQueue)
{

}
