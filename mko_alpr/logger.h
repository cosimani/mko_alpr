#ifndef LOGGER_H
#define LOGGER_H

#define LOG_INF(X) Logger::getInstance()->inf(QString(X))
#define LOG_WAR(X) Logger::getInstance()->war(QString(X))
#define LOG_ERR(X) Logger::getInstance()->err(QString(X))

#define CONSOLE "CONSOLE"
#define LOG_FILE_EXT ".log"

#define PREF_INF "[INF] "
#define PREF_WAR "[WAR] "
#define PREF_ERR "[ERR] "

#define LOG_DIR "log_dir"

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QDateTime>

#include <stdexcept>

#include "database.h"

using namespace std;

class Logger : public QObject
{
    Q_OBJECT

private:

    static QString out;
    static QFile *file;

    static Logger *instance;
    explicit Logger(QObject *parent = NULL);

public:

    static Logger *getInstance();

    void log(QString text);
    void inf(QString text);
    void war(QString text);
    void err(QString text);

    QString getOut() const;
    void setOut(const QString &value);

    QFile *getFile() const;
    void setFile(QFile *value);

    ~Logger();
};

#endif // LOGGER_H
