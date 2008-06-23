TEMPLATE = app
TARGET = gcse
QT += core \
    gui
HEADERS += src/params.h \
    src/sentence.h \
    src/ga.h \
    src/log.h \
    src/classifier.h \
    src/grammar.h \
    src/prodcondition.h \
    src/prodaction.h \
    src/prodrule.h \
    src/symbol.h \
    src/mainwindow.h \
    src/grammareditor.h \
    src/random.h
SOURCES += src/params.cpp \
    src/sentence.cpp \
    src/ga.cpp \
    src/log.cpp \
    src/classifier.cpp \
    src/grammar.cpp \
    src/prodcondition.cpp \
    src/prodaction.cpp \
    src/prodrule.cpp \
    src/symbol.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/grammareditor.cpp \
    src/random.cpp
FORMS += gui/mainwindow.ui \
    gui/grammareditor.ui
RESOURCES += gui/icons.qrc
CONFIG += debug_and_release
