#ifndef CARD_H
#define CARD_H

#include <QObject>
#include<Qdate>
class card : public QObject
{
    Q_OBJECT
public:
    explicit card(QObject *parent = nullptr);
    uint64_t noteId;
    QString noteString;
    int noteType;
    QString noteQuestion;
    QString noteAnswer;
    QDate lastReviewDate;

    int interval;
    int repetitions;
    double easeFactor;
signals:
};

#endif // CARD_H
