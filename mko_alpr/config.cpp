#include "config.h"

Config* Config::instance = NULL;
QVector<QStringList>* Config::strings = new QVector<QStringList>();

Config::Config(QObject *parent) :
    QObject(parent)
{
    init();
}

Config *Config::getInstance()
{
    if(!instance)
    {
        instance = new Config();
        LOG_INF("Config: successfully initialized");
    }
    return instance;
}

void Config::init()
{
    QString currentPath = QDir::currentPath();
    QString appName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QString inputFilePath = currentPath + CONFIG_DIR + appName + CONFIG_FILE_EXT;

    QFile inputFile(inputFilePath);
    if(inputFile.open(QIODevice::ReadOnly) == false)
    {
        LOG_ERR("Config: file not exists");
    }
    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
        QString line = in.readLine();

        if(line.contains(CONFIG_FILE_COMMENTS))
        {
            line = line.remove(line.indexOf(CONFIG_FILE_COMMENTS),
                               line.length() - line.indexOf(CONFIG_FILE_COMMENTS));
        }

        if(line.simplified().isEmpty()) {
            continue;
        }

        QStringList kv = line.split(CONFIG_FILE_ASSIGNMENT);

        if(kv.size() == 2)
        {
            for(int i = 0; i < 2; i++)
            {
                kv.operator [](i) = kv.at(i).simplified();
            }
        }
        strings->append(kv);
    }
    inputFile.close();
}

QString Config::getString(QString key)
{
    for(int i = 0; i < strings->size(); i++)
    {
        if(strings->at(i).at(0) == key)
        {
            return strings->at(i).at(1);
        }
    }

    LOG_ERR("Config: string not found");
    return "";
}

string Config::getStdString(QString key)
{
    return getString(key).toStdString();
}

const char *Config::getCharString(QString key)
{
    return getStdString(key).c_str();
}

Config::~Config()
{
    delete instance;
}
