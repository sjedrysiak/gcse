TEMPLATE = app
TARGET = gcse
QT += core \
    gui \
    testlib
HEADERS += gui/SettingsDialog.h \
    gui/MainWindow.h \
    src/MyException.h \
    src/Symbol.h \
    src/Random.h \
    src/Sentence.h \
    src/Condition.h \
    src/Action.h \
    src/Classifier.h \
    src/Grammar.h \
    src/Params.h \
    src/CYK.h \
    src/GA.h \
    src/GCS.h
SOURCES += src/Params.cpp \
    gui/SettingsDialog.cpp \
    gui/MainWindow.cpp \
    src/MyException.cpp \
    src/Symbol.cpp \
    src/Random.cpp \
    src/Sentence.cpp \
    src/Condition.cpp \
    src/Action.cpp \
    src/Classifier.cpp \
    src/Grammar.cpp \
    src/CYK.cpp \
    src/GA.cpp \
    src/GCS.cpp \
    src/main.cpp
FORMS += gui/SettingsDialog.ui \
    gui/MainWindow.ui
RESOURCES += gui/icons.qrc
CONFIG += debug_and_release
