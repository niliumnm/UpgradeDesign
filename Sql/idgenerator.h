#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <QObject>

class IdGenerator : public QObject
{
    Q_OBJECT
public:
    static IdGenerator* getInstance(); // 静态成员函数，用于获取单例对象
    uint64_t generateGlobalId();
private:
    explicit IdGenerator(QObject *parent = nullptr);
    static IdGenerator* instance; // 静态成员变量，保存单例对象


signals:
};

#endif // IDGENERATOR_H
