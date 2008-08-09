#include "MyException.h"

MyException::MyException(const QString& str, const char* file, int line) :
	QtConcurrent::Exception(), msg(str), file(file), line(QString::number(line))
{}

QString MyException::getMsg() const
{
	return this->file + ((this->line == "-1") ? "" : ":" + this->line + ": ") + this->msg;
}

