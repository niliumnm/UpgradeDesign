#ifndef TOPICBASE_H
#define TOPICBASE_H

#include <QObject>

class TopicBase : public QObject
{
    Q_OBJECT

public:
    explicit TopicBase(QObject *parent = nullptr);
    enum NoteType {
        TypeArticle,
        TypeQuestion,
    };
    Q_ENUM(NoteType)
    virtual NoteType getType()=0;

signals:
};

#endif // TOPICBASE_H
