#ifndef UTIL_H
#define UTIL_H

#include <QObject>

#include "logger.h"

#ifdef DEBUG_OSI
#include "openalpr/config.h"
#endif

class Util : public QObject
{
    Q_OBJECT

private:

    static Util *instance;
    explicit Util(QObject *parent=NULL);

public:

    static Util *getInstance();
    QString toCamelCase(const QString& s);
    ~Util();

#ifdef DEBUG_OSI
    void publicarInfoAlprConfig( alpr::Config * config );
#endif
};

#endif // UTIL_H
