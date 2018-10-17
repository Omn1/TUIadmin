#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T23:59:16
#
#-------------------------------------------------

QT       += core gui widgets network charts

TARGET = WidgetsForTUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        orderwidget.cpp \
    dishwidget.cpp \
    ingredientwidget.cpp \
    orderinfowidget.cpp \
    dishinfowidget.cpp \
    jsondownloader.cpp \
    ordertable.cpp \
    warehousetable.cpp \
    dishadderwidget.cpp \
    ingredientselector.cpp \
    jsonsender.cpp \
    ingredientadderwidget.cpp \
    ingredientsupplierwidget.cpp \
    mainmenuwidget.cpp \
    mainwindow.cpp \
    editmenuwidget.cpp \
    statisticsengine.cpp

HEADERS += \
        orderwidget.h \
    dishwidget.h \
    ingredientwidget.h \
    orderinfowidget.h \
    dishinfowidget.h \
    jsondownloader.h \
    ordertable.h \
    warehousetable.h \
    dishadderwidget.h \
    ingredientselector.h \
    jsonsender.h \
    ingredientadderwidget.h \
    ingredientsupplierwidget.h \
    mainmenuwidget.h \
    mainwindow.h \
    editmenuwidget.h \
    statisticsengine.h

FORMS += \
    ingredientwidget.ui \
    orderinfowidget.ui \
    dishinfowidget.ui \
    dishadderwidget.ui \
    ingredientselector.ui \
    ingredientadderwidget.ui \
    ingredientsupplierwidget.ui \
    mainmenuwidget.ui \
    editmenuwidget.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
