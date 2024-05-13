#include "question.h"
#include "entity/topicbase.h"

Question::Question(QObject *parent)
    : TopicBase{parent}
{

}

Question::Question(uint64_t id, QString question, QString anser, uint64_t resId):id(id),question(question),anser(anser),resId(resId)
{

}

TopicBase::NoteType Question::getType()
{
    return NoteType::TypeQuestion;
}

uint64_t Question::getId() const
{
    return id;
}

void Question::setId(uint64_t newId)
{
    id = newId;
}

void Question::setType(TopicBase::NoteType newType)
{
    type =  newType;
}

void Question::setQuestion(const QString &newQuestion)
{
    question = newQuestion;
}

QString Question::getAnser() const
{
    return anser;
}

void Question::setAnser(const QString &newAnser)
{
    anser = newAnser;
}

uint64_t Question::getResId() const
{
    return resId;
}

void Question::setResId(uint64_t newResId)
{
    resId = newResId;
}
QString Question::getQuestion()
{
    return question;
}


