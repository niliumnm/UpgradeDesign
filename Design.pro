QT       += core gui
QT += sql
QT += core5compat
RESOURCES += theme/dark/darkstyle.qrc
RESOURCES += theme/light/lightstyle.qrc


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sql/idgenerator.cpp \
    Sql/sqlUtil.cpp \
    Widget/tstandarditem.cpp \
    Widget/tstandarditemmodel.cpp \
    entity/article.cpp \
    entity/question.cpp \
    entity/topicbase.cpp \
    learnwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    sm2/card.cpp \
    sm2/datautil.cpp \
    sm2/sm.cpp \
    sm2/smresponse.cpp

HEADERS += \
    Sql/idgenerator.h \
    Sql/sqlUtil.h \
    Widget/tstandarditem.h \
    Widget/tstandarditemmodel.h \
    entity/article.h \
    entity/question.h \
    entity/topicbase.h \
    learnwindow.h \
    mainwindow.h \
    sm2/card.h \
    sm2/datautil.h \
    sm2/sm.h \
    sm2/smresponse.h

FORMS += \
    learnwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
