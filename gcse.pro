TEMPLATE = app
TARGET = gcse
QT += core \
    gui
HEADERS += src/log.h \
    src/classifier.h \
    src/grammar.h \
    src/prodcondition.h \
    src/prodaction.h \
    src/prodrule.h \
    src/symbol.h \
    src/mainwindow.h \
    src/grammareditor.h \
    src/random.h
SOURCES += src/log.cpp \
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
