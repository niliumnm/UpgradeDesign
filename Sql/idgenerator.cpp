#include "idgenerator.h"

#include<QUuid>

IdGenerator* IdGenerator::instance = nullptr; // 初始化静态成员变量

IdGenerator::IdGenerator(QObject * parent): QObject {
        parent
    } {}

IdGenerator *IdGenerator::getInstance()
{
    if (!instance)
    {
        instance = new IdGenerator();
    }
    return instance;
}

uint64_t IdGenerator::generateGlobalId() {
    QUuid uuid = QUuid::createUuid();
    QByteArray uuidBytes = uuid.toRfc4122();

    // 将 QByteArray 转换为 uint64_t
    const uint64_t * data = reinterpret_cast <
        const uint64_t * > (uuidBytes.constData());
    uint64_t globalId = * data;

    return globalId;
}
