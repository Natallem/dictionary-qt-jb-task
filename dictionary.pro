QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    main_window.cpp \
    dictionary.cpp \
    searching_worker.cpp \
    dictionary_util.cpp \
    searching_result.cpp

HEADERS += \
        main_window.h \
        dictionary.h \
        searching_worker.h \
        dictionary_util.h \
        searching_result.h \
        constants.h

FORMS += \
    mainwindow.ui

OTHER_FILES += words_processed.txt

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
