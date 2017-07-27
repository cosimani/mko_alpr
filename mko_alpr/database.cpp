#include "database.h"

Database* Database::instance = NULL;

Database::Database( QObject *parent ) : QObject( parent ),
                                        databaseOk( false )
{
    database = QSqlDatabase::addDatabase( Config::getInstance()->getString( DATABASE_DRIVER ) );
}

QString Database::getLoginQuery()
{
    return Config::getInstance()->getString( LOGIN_QUERY );
}

QString Database::getSessionQuery()
{
    return Config::getInstance()->getString( SESSION_QUERY );
}

QString Database::getPlateDetectionQuery()
{
    return Config::getInstance()->getString( PLATE_DETECTION_QUERY );
}

QString Database::getCandidateQuery()
{
    return Config::getInstance()->getString( CANDIDATE_QUERY );
}

QString Database::getLogQuery()
{
    return Config::getInstance()->getString( LOG_QUERY );
}

Database *Database::getInstance()
{
    if( instance == NULL )
    {
        instance = new Database();
        LOG_INF( "Database: successfully initialized" );
    }
    return instance;
}

void Database::connect()
{
    Database::database.setDatabaseName( Config::getInstance()->getString( DATABASE_URI ) );

    if( database.open() == false )
    {
        LOG_ERR( "Database: cannot open connection" );
    }

    LOG_INF( "Database: connected ok" );

    databaseOk = true;
}

void Database::disconnect()
{
    if( database.isOpen() )
    {
        database.close();
        LOG_INF( "Database: disconnected ok" );
    }
}

int Database::login( QString user, QString password )
{
    QString encryptedPassword =
            QString(QCryptographicHash::hash(
                        QString(password).toUtf8(),
                        QCryptographicHash::Md5).toHex());

    QSqlQuery query;

    query.prepare(getLoginQuery());
    query.bindValue(":user", user);
    query.bindValue(":password", encryptedPassword);

    if(query.exec() == false)
    {
        LOG_ERR(QString("Database: " + query.lastError().text()));
        return -1;
    }

    if(query.next())
    {
        int result = query.value(0).toInt();

        switch(result)
        {
        case 0:
            LOG_WAR(QString("Database: invalid username or password [" + user + ":" + encryptedPassword +  "]"));
            break;

        default:
            if(result > 0) {
                LOG_INF("Database: loggin successful");
            }
        }
        return result;
    }

    LOG_WAR("Database: unexpected login query result");
    return -1;
}

int Database::startSession(int adminId)
{
    QSqlQuery query;

    query.prepare(getSessionQuery());
    query.bindValue(":adminId", adminId);

    if(query.exec() == false)
    {
        LOG_ERR(QString("Database: " + query.lastError().text()));
        return -1;
    }

    return query.lastInsertId().toInt();
}

int Database::plateDetection(int sessionId)
{
    QSqlQuery query;

    query.prepare(getPlateDetectionQuery());
    query.bindValue(":sessionId", sessionId);

    if(query.exec() == false)
    {
        LOG_ERR(QString("Database: " + query.lastError().text()));
        return -1;
    }

    return query.lastInsertId().toInt();
}

int Database::candidate(QString domain, QString matches, float confidence, int plateDetectionId)
{
    QSqlQuery query;

    query.prepare(getCandidateQuery());
    query.bindValue(":d", domain);
    query.bindValue(":m", matches);
    query.bindValue(":c", confidence);
    query.bindValue(":pdid", plateDetectionId);

    if(query.exec() == false)
    {
        LOG_ERR(QString("Database: " + query.lastError().text()));
        return -1;
    }

    return query.lastInsertId().toInt();
}

int Database::log(QString text, int level)
{
    QSqlQuery query;

    query.prepare(getLogQuery());
    query.bindValue(":m", text);
    query.bindValue(":l", level);

    if(query.exec() == false)
    {
        return -1;
    }

    return query.lastInsertId().toInt();
}

Database::~Database()
{
    this->disconnect();
}
