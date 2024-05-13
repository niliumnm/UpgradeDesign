#ifndef SQLUTIL_H
#define SQLUTIL_H

#include <QObject>
#include<QSqlDatabase>
#include"idgenerator.h"
#include<sm2/smresponse.h>
#include<qvector.h>
#include<entity/topicbase.h>
class SqlUtil : public QObject
{
    Q_OBJECT
public:
    enum NoteType {
        TypeArticle,
        TypeQuestion,
    };
    Q_ENUM(NoteType)
    void init();

    void insertArticle(const uint64_t &id,const QString &html);
    void getArticle(const uint64_t &id,QString &html);
    void setArticle(const uint64_t &id,const QString &html);

    void insertQuestion(const uint64_t &id, const QString& noteQuestion, const QString& noteAnswer);
    void setQuestion(const uint64_t &id, const QString& noteQuestion, const QString& noteAnswer);
    void getQuestion(const uint64_t &id, QString& noteQuestion, QString& noteAnswer);

    void insertNote(const uint64_t &id, const QString& noteString, int noteType, const QString& noteQuestion, const QString& noteAnswer);
    void setNote(const uint64_t &noteId, const QString& noteString, int noteType, const QString& noteQuestion, const QString& noteAnswer, const QString &resId);
    void getNote(const uint64_t &noteId, QString& noteString, int &noteType, QString& noteQuestion, QString& noteAnswer,QString& resId);
    void processText(QString &s);

    void insertSmResponse(const uint64_t &noteId,const SmResponse&response);
    bool getSmResponse(const uint64_t &noteId, SmResponse&response);
    void setSmResponse(const uint64_t &noteId, const SmResponse&response);
    void updateCardId(const uint64_t &noteId, const uint64_t &newCardId);
    QVector<SmResponse>getAllCard();
    uint64_t getNoteIdByCardId(const uint64_t&cardId);
    uint64_t getResId(const uint64_t &noteId);

    //获得文章
    QVector<TopicBase*>* getAllArticle();
    QVector<TopicBase*>* getAllQuestion();
    ~SqlUtil();

    static SqlUtil*getInstance();

signals:
    void learn();
private:
    static SqlUtil*instance;
    QString databaseName;
    QSqlDatabase db;
    IdGenerator *idGenerator;
    explicit SqlUtil(QObject *parent = nullptr);


};

#endif // SQLUTIL_H
