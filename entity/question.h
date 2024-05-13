#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include "topicbase.h"

class Question : public TopicBase
{
    Q_OBJECT
public:
    explicit Question(QObject *parent = nullptr);
    Question(uint64_t id, QString question,QString anser,uint64_t resId);

    // TopicBase interface
public:
    virtual NoteType getType() override;
    uint64_t getId() const;
    void setId(uint64_t newId);

    void setType(TopicBase::NoteType newType);

    void setQuestion(const QString &newQuestion);
    QString getQuestion();

    QString getAnser() const;
    void setAnser(const QString &newAnser);

    uint64_t getResId() const;
    void setResId(uint64_t newResId);

private:
    uint64_t id;
    NoteType type=TypeQuestion;
    QString question;
    QString anser;
    uint64_t resId;
};



#endif // QUESTION_H
