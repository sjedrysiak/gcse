TEMPLATE = app
TARGET = gcse
QT += core \
    gui \
    testlib
HEADERS += src/Symbol.h \
    src/Random.h \
    src/Sentence.h \
    src/Condition.h \
    src/Action.h \
    src/Classifier.h
SOURCES += src/Symbol.cpp \
    src/Random.cpp \
    src/Sentence.cpp \
    src/Condition.cpp \
    src/Action.cpp \
    src/Classifier.cpp \
    src/tests.cpp
FORMS += 
RESOURCES += 
CONFIG += debug_and_release
