#include "logger.h"

Logger* Logger::instance = NULL;

QString Logger::out = CONSOLE;
QFile* Logger::file = NULL;

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}

Logger *Logger::getInstance()
{
    if(!instance)
    {
        instance = new Logger();
        LOG_INF( "Logger: successfully initialized" );
    }
    return instance;
}

QString Logger::getOut() const
{
    return out;
}

void Logger::setOut( const QString &value )
{
    out = value;
    LOG_INF( QString( "Logger: logging to " + this->getOut() ) );
}

QFile *Logger::getFile() const
{
    return file;
}

void Logger::setFile(QFile *value)
{
    file = value;
}

void Logger::log( QString text )
{
    if( getOut() == CONSOLE )
    {
        qDebug() << text;
        return;
    }

    if( getFile() == NULL )
    {
        file = new QFile(
                    getOut() + QDateTime::currentDateTime().toString() + LOG_FILE_EXT);
    }

    if(!getFile()->isOpen())
    {
        getFile()->open(QIODevice::ReadWrite);
    }

    if(getFile()->isOpen())
    {
        QTextStream stream(getFile());
        stream << text << endl;
    }
    else
    {
        setOut( CONSOLE );
        log(text);
    }
}

void Logger::inf( QString text )
{
#ifndef SIN_DEBUG
    log( QString( PREF_INF + text ) );
#endif
    if( Database::getInstance()->databaseOk ) Database::getInstance()->log( text, LINF );
}

void Logger::war( QString text )
{
#ifndef SIN_DEBUG
    log( QString( PREF_WAR + text ) );
#endif
   if( Database::getInstance()->databaseOk ) Database::getInstance()->log( text, LWAR );
}

void Logger::err( QString text )
{
#ifndef SIN_DEBUG
    log( QString( PREF_ERR + text ) );
#endif
   if( Database::getInstance()->databaseOk ) Database::getInstance()->log( text, LERR );
    throw new runtime_error(text.toStdString());
}

Logger::~Logger()
{
    delete file;
    delete instance;
}
