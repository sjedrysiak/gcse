/*
 * MyException.h
 *
 *  Created on: 2008-08-09
 *      Author: sylver
 */

#ifndef MYEXCEPTION_H_
#define MYEXCEPTION_H_

#include <QtCore>
#include <QString>

#define EXCMSG __FILE__, __LINE__

class MyException: public QtConcurrent::Exception
{
public:
	MyException(const QString& str = "Exception", const char* file = "", int line = -1);
	virtual QString getMsg() const;
	virtual void raise() const
	{
		throw *this;
	}
	virtual Exception* clone() const
	{
		return new MyException(*this);
	}
	operator QString() const
	{
		return getMsg();
	}
	virtual ~MyException() throw()
	{
	}
protected:
	QString msg;
	QString file;
	QString line;
};

class ArgumentOutOfRangeException: public MyException
{
public:
	ArgumentOutOfRangeException(const QString& str = "Argument out of range exception", const char* file = "", int line = -1) :
		MyException(str, file, line)
	{
	}
	virtual Exception* clone() const
	{
		return new ArgumentOutOfRangeException(*this);
	}
};

#endif /* MYEXCEPTION_H_ */
