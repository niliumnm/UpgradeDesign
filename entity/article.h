#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>
#include "topicbase.h"
#include<QString>
class Article : public TopicBase
{
    Q_OBJECT
public:
    explicit Article(QObject *parent = nullptr);
    Article(uint64_t id, QString text,uint64_t resId);

    // TopicBase interface
public:
    virtual NoteType getType() override;
    uint64_t getId() const;
    void setId(uint64_t newId);

    QString getString() const;
    void setString(const QString &newString);

    uint64_t getResId() const;
    void setResId(uint64_t newResId);

    void setType(TopicBase::NoteType newType);

private:
    uint64_t id;
    QString string;
    NoteType type=TopicBase::TypeArticle;
    uint64_t resId;
};



#endif // ARTICLE_H
