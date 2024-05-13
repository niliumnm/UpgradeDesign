#include "article.h"
#include<entity/topicbase.h>
Article::Article(QObject *parent)
    : TopicBase{parent}
{

}

Article::Article(uint64_t id, QString text, uint64_t resId):id(id),string(text),resId(resId)
{

}

TopicBase::NoteType Article::getType()
{
    return NoteType::TypeArticle;
}


uint64_t Article::getId() const
{
    return id;
}

void Article::setId(uint64_t newId)
{
    id = newId;
}

QString Article::getString() const
{
    return string;
}

void Article::setString(const QString &newString)
{
    string = newString;
}

uint64_t Article::getResId() const
{
    return resId;
}

void Article::setResId(uint64_t newResId)
{
    resId = newResId;
}

void Article::setType(TopicBase::NoteType newType)
{
    type = newType;
}


