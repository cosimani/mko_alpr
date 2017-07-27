#include "configobject.h"
#include <QDebug>

ConfigObject::ConfigObject(QObject *parent) : QObject(parent),
                                              fuente( SIN_DEFINIR ),
                                              indexCamUSB( -1 )
{

}

ConfigObject::Fuente ConfigObject::getFuente() const
{
    return fuente;
}

void ConfigObject::setFuente(const Fuente &value)
{
    fuente = value;
}

int ConfigObject::getIndexCamUSB() const
{
    return indexCamUSB;
}

void ConfigObject::setIndexCamUSB(int value)
{
    indexCamUSB = value;
}

QString ConfigObject::getFolder() const
{
    return folder;
}

void ConfigObject::setFolder(const QString &value)
{
    folder = value;
}

QString ConfigObject::getIpCam() const
{
    return ipCam;
}

void ConfigObject::setIpCam(const QString &value)
{
    ipCam = value;
}

void ConfigObject::clear()
{
    this->setFuente( SIN_DEFINIR );
    this->setFolder( "" );
    this->setIndexCamUSB( -1 );
    this->setIpCam( "" );
    this->setUrlWebService( "" );
}

QString ConfigObject::getUrlWebService() const
{
    return urlWebService;
}

void ConfigObject::setUrlWebService(const QString &value)
{
    urlWebService = value;
}
