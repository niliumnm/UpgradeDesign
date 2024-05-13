#include "sqlUtil.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<quuid.h>
#include<QSqlError>
#include<QRegularExpression>
#include<QRegExp>
#include<qvector.h>
#include<entity/article.h>
#include<entity/question.h>
#include<entity/topicbase.h>
SqlUtil* SqlUtil::instance=nullptr;
SqlUtil::SqlUtil(QObject *parent)
    : QObject{parent}
{
    databaseName = "./database.db";
    init();
    SmResponse smres(0,0,0);
    // insertSmResponse(13206019701818760854,smres);
    //getSmResponse(13206019701818760854,smres);
    // QDate date(2019,1,1);
    // smres.lastReviewDate=date;
    // setSmResponse(13206019701818760854,smres);
    getNoteIdByCardId(667343706316187531);
}

void SqlUtil::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./mydatabase.db");
    if(db.open())
    {
        QSqlQuery query;
        QString sql="CREATE TABLE \"note\" (\"id\" TEXT NOT NULL, \"string\" TEXT, \"type\" INTEGER, \"question\" TEXT,\"anser\" TEXT,\"resId\" TEXT DEFAULT 0,PRIMARY  KEY(\"id\"))";
        query.exec(sql);
        /*
            CREATE TABLE "card" (
                "id"	TEXT NOT NULL,
                "interval"	INTEGER,
                "repetitions"	INTEGER,
                "easeFactor"	REAL,
                "lastReviewDate"	TEXT,
                PRIMARY KEY("id")
            );
        */
        sql="CREATE TABLE \"card\" (\"id\"	TEXT NOT NULL,\"interval\"INTEGER,\"repetitions\"INTEGER,\"easeFactor\"REAL,\"lastReviewDate\"TEXT,PRIMARY KEY(\"id\"))";
        query.exec(sql);
    }else{
        qDebug()<<"打开失败";
    }
}

void SqlUtil::insertArticle(const uint64_t &id,const QString &html)
{
    insertNote(id,html,TypeArticle,"","");
}

void SqlUtil::getArticle(const uint64_t &id,QString &html)
{
    QString StrId=QString::number(id);
    QString sql = QString("SELECT string FROM note WHERE id = '%1'").arg(StrId);
    QSqlQuery query(sql);
    if (query.next()) {
        // 获取结果中的文本数据
        QString text = query.value(0).toString();


        // text.replace("\\\"","\"");
        // qDebug()<<"getArticle取出的html"<<text;
        QString searchText = "\\\\\\\"";
        QChar c='\"';
        QString replaceText = c;//
        QString result = text.replace(QRegularExpression(searchText), replaceText);
        html=text;
        // 使用获取到的文本数据进行后续处理
        // ...
    } else {
        // 没有找到匹配的记录
        // 处理相应的逻辑
    }
}

void SqlUtil::setArticle(const uint64_t &id,const QString &html)
{
    QString StrId=QString::number(id);
    QString StrHtml=html;
    StrHtml.replace("'", "''");
    StrHtml.replace("\"", "\\\"");

    // 创建更新语句
    QString sql = "UPDATE note SET string = '"+StrHtml+"' WHERE id = '"+StrId+"'";
    QSqlQuery query;


    // 执行更新操作
    if (query.exec(sql)) {
        // 更新成功
    } else {
        // 更新失败，处理错误
        qDebug() << "更新失败：" << query.lastError().text();
    }
    // qDebug()<<"执行的sql"<<sql;

}

void SqlUtil::insertQuestion(const uint64_t &id, const QString &noteQuestion, const QString &noteAnswer)
{
    insertNote(id,"这是一个问答",TypeQuestion,noteQuestion,noteAnswer);
}

void SqlUtil::setQuestion(const uint64_t &id, const QString &noteQuestion, const QString &noteAnswer)
{
    setNote(id,"这是一个问答",TypeQuestion,noteQuestion,noteAnswer,"0");
}

void SqlUtil::getQuestion(const uint64_t &id, QString &noteQuestion, QString &noteAnswer)
{
    QString noteString;
    int noteType;
    QString resId;
    getNote(id,noteString,noteType,noteQuestion,noteAnswer,resId);
}

void SqlUtil::insertNote(const uint64_t &id, const QString& noteString, int noteType, const QString& noteQuestion, const QString& noteAnswer)
{
    QString StrId=QString::number(id);
    QString StrNote=noteString;
    StrNote.replace("'", "''");
    StrNote.replace("\"", "\\\"");

    QString sql="INSERT INTO note (id, string, type, question, anser) VALUES ('"+StrId+"', '"+noteString+"',"+QString::number(noteType)+", '"+noteQuestion+"', '"+noteAnswer+"')";
    qDebug()<<sql;
    QSqlQuery query;

    if (!query.exec(sql)) {
        qDebug()<<"insertNote插入错误";
    }
}

void SqlUtil::setNote(const uint64_t &noteId, const QString& noteString, int noteType, const QString& noteQuestion, const QString& noteAnswer,const QString &resId)
{
    QString StrId=QString::number(noteId);
    QString StrHtml=noteString;
    StrHtml.replace("'", "''");
    StrHtml.replace("\"", "\\\"");

    // 创建更新语句
    QString sql = "UPDATE note SET string = '"+StrHtml+"',type = "+QString::number(noteType)+",question = '"+noteQuestion+"',anser = '"+noteAnswer+"'"+",resId='"+resId+"' WHERE id = '"+StrId+"'";

    QSqlQuery query;
    qDebug()<<"执行的sql"<<sql;

    // 执行更新操作
    if (query.exec(sql)) {
        // 更新成功
    } else {
        // 更新失败，处理错误
        qDebug() << "更新失败：" << query.lastError().text();
    }
    // qDebug()<<"执行的sql"<<sql;

}

void SqlUtil::getNote(const uint64_t &noteId, QString &noteString, int &noteType, QString &noteQuestion, QString &noteAnswer,QString& resId)
{
    QString StrId=QString::number(noteId);
    QString sql = QString("SELECT string,type,question,anser,resId FROM note WHERE id = '%1'").arg(StrId);
    QSqlQuery query(sql);
    if (query.next()) {
        // 获取结果中的文本数据
        QString text = query.value(0).toString();
        noteType = query.value(1).toInt();
        noteQuestion = query.value(2).toString();
        noteAnswer = query.value(3).toString();
        resId = query.value(4).toString();



        // text.replace("\\\"","\"");
        // qDebug()<<"getArticle取出的html"<<text;
        QString searchText = "\\\\\\\"";
        QChar c='\"';
        QString replaceText = c;//
        QString result = text.replace(QRegularExpression(searchText), replaceText);
        noteString=text;
        // 使用获取到的文本数据进行后续处理
        // ...
    } else {
        // 没有找到匹配的记录
        // 处理相应的逻辑
    }
}

void SqlUtil::insertSmResponse(const uint64_t &noteId,const SmResponse&response)
{
    //插入时间
    QDate today=QDate::currentDate();
    uint64_t cardId=getResId(noteId);
    if(cardId==0){
        //把noteID更新成新生成的cardID
        uint64_t newCardId=idGenerator->generateGlobalId();
        QString newCardIdStr=QString::number(newCardId);
        //根据cardID插入

        QString sql="INSERT INTO card (id, interval, repetitions, easeFactor,lastReviewDate) VALUES ('"+newCardIdStr+"', "+QString::number(response.interval)+","+QString::number(response.repetitions)+", "+QString::number(response.easeFactor)+","+today.toString("yyyy-MM-dd")+")";
        qDebug()<<sql;
        QSqlQuery query;

        if (!query.exec(sql)) {
            qDebug()<<"insertNote插入错误";
        }
        updateCardId(noteId,newCardId);
    }else{
        qDebug()<<"对应的SmResponse已存在";
    }

}

bool SqlUtil::getSmResponse(const uint64_t &noteId, SmResponse &response)
{

    uint64_t cardId=getResId(noteId);
    QString StrId=QString::number(cardId);

    QString sql = QString("SELECT id, interval,repetitions,easeFactor, lastReviewDate FROM card WHERE id = '%1'").arg(StrId);
    QSqlQuery query(sql);
    if (query.next()) {
        // 获取结果中的文本数据
        // if(response==nullptr){
        //     qDebug()<<"getSmResponse: 空的response!";
        //     return ;
        // }
        response.id = query.value(0).toULongLong();
        response.interval = query.value(1).toInt();
        response.repetitions = query.value(2).toInt();
        response.easeFactor = query.value(3).toDouble();
        QString lastReviewDateStr = query.value(4).toString();
        response.lastReviewDate = QDate::fromString(lastReviewDateStr, "yyyy-MM-dd");

    } else {
        // 没有找到匹配的记录
        // 处理相应的逻辑
        qDebug()<<"getSmResponse: 没有查到相应的记录";
        return false;
    }
    return true;
}

uint64_t SqlUtil::getResId(const uint64_t &noteId)
{
    QString noteString;
    int noteType;
    QString noteQuestion;
    QString  noteAnswer;
    QString resId;
    getNote(noteId,noteString,noteType,noteQuestion,noteAnswer,resId);
    uint64_t id = resId.toULongLong();
    return id;
}

QVector<TopicBase*> *SqlUtil::getAllArticle()
{
    QVector<TopicBase*> *articleList = new QVector<TopicBase*>;

    // 执行 SQL 查询语句
    QSqlQuery query;
    if (query.exec("SELECT * FROM note WHERE note.type = 0"))
    {
        // 遍历查询结果
        while (query.next())
        {

            // 从查询结果中提取字段值
            QString sid = query.value("id").toString();
            uint64_t id=sid.toULongLong();
            QString text = query.value("string").toString();
            QString sResId = query.value("resId").toString();
            uint64_t resId=sResId.toULongLong();

            // 根据字段值创建 TopicBase 对象，并添加到 QVector 中
            TopicBase* newArticle = new Article(id, text, resId);
            articleList->append(newArticle);
        }
    }
    else
    {
        // 查询失败，处理错误
        qDebug() << "Failed to execute query: " << query.lastError().text();
    }

    return articleList;
}

QVector<TopicBase*> *SqlUtil::getAllQuestion()
{
    QVector<TopicBase*> *questionList = new QVector<TopicBase*>;

    // 执行 SQL 查询语句
    QSqlQuery query;
    if (query.exec("SELECT * FROM note WHERE note.type = 1"))
    {
        // 遍历查询结果
        while (query.next())
        {

            // 从查询结果中提取字段值
            QString sid = query.value("id").toString();
            uint64_t id=sid.toULongLong();

            QString question = query.value("question").toString();
            QString anser = query.value("anser").toString();

            QString sResId = query.value("resId").toString();
            uint64_t resId=sResId.toULongLong();

            // 根据字段值创建 TopicBase 对象，并添加到 QVector 中
            TopicBase* newQuestion = new Question(id, question,anser,resId);
            questionList->append(newQuestion);
        }
    }
    else
    {
        // 查询失败，处理错误
        qDebug() << "Failed to execute query: " << query.lastError().text();
    }
    return questionList;

}

void SqlUtil::setSmResponse(const uint64_t &noteId, const SmResponse &response)
{
    uint64_t cardId=getResId(noteId);
    QString StrId=QString::number(cardId);

    // 创建更新语句
    QString sql = "UPDATE card SET interval = "+QString::number(response.interval)+",repetitions = "+QString::number(response.repetitions)+",easeFactor = "+QString::number(response.easeFactor)+", lastReviewDate=' "+response.lastReviewDate.toString("yyyy-MM-dd")+"'  WHERE id = '"+StrId+"'";

    QSqlQuery query;
    qDebug()<<"执行的sql"<<sql;

    // 执行更新操作
    if (query.exec(sql)) {
        // 更新成功
    } else {
        // 更新失败，处理错误
        qDebug() << "更新失败：" << query.lastError().text();
    }
    // qDebug()<<"执行的sql"<<sql;
}

void SqlUtil::updateCardId(const uint64_t &noteId, const uint64_t &newCardId)
{
    QString StrId=QString::number(noteId);

    // 创建更新语句
    QString sql = "UPDATE note SET resId = '"+QString::number(newCardId)+"' WHERE id = '"+StrId+"'";

    QSqlQuery query;
    qDebug()<<"执行的sql"<<sql;

    // 执行更新操作
    if (query.exec(sql)) {
        // 更新成功
    } else {
        // 更新失败，处理错误
        qDebug() << "更新失败：" << query.lastError().text();
    }
    // qDebug()<<"执行的sql"<<sql;
}

QVector<SmResponse> SqlUtil::getAllCard()
{
    QVector<SmResponse> allCards;
    // 执行查询语句
    QSqlQuery query;
    QString sql = "SELECT id, interval, repetitions, easeFactor, lastReviewDate FROM card";
    if (!query.exec(sql)) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return allCards;
    }
    // 遍历查询结果
    while (query.next()) {
        uint64_t id = query.value(0).toULongLong();
        int interval = query.value(1).toInt();
        int repetitions = query.value(2).toInt();
        double easeFactor = query.value(3).toDouble();
        QString lastReviewDateStr = query.value(4).toString();
        QDate lastReviewDate = QDate::fromString(lastReviewDateStr, "yyyy-MM-dd");
        SmResponse card(id,interval, repetitions, easeFactor,lastReviewDate);
        allCards.append(card);
    }
    return allCards;
}

uint64_t SqlUtil::getNoteIdByCardId(const uint64_t &cardId)
{
    //SELECT id FROM note WHERE resId='1112'
    QString StrId=QString::number(cardId);
    QString sql = QString("SELECT id FROM note WHERE resId = '%1'").arg(StrId);
    QSqlQuery query(sql);
    if (query.next()) {
        // 获取结果中的文本数据
        QString noteId = query.value(0).toString();
        uint64_t id=noteId.toULongLong();
        return id;
        // 使用获取到的文本数据进行后续处理
        // ...
    } else {
        //没查到
        return -1;
    }
}


SqlUtil::~SqlUtil()
{
    db.close();
}

SqlUtil *SqlUtil::getInstance()
{
    if (!instance)
    {
        instance = new SqlUtil();
    }
    return instance;
}


