
#include "restclient.h"
#include <QByteArray>
#include <QDebug>
#include <QVariant>
#include <QUrlQuery>

RestClient::RestClient( QObject * parent ) : QObject( parent ),
                                             nam( new QNetworkAccessManager() )
{
    connect( this->getNam(), SIGNAL( finished( QNetworkReply* ) ),
             this, SLOT( slot_GETresponse( QNetworkReply* ) ) );

//    HTTPGet( URL_EMPRESA, "p_objeto", "JEJ735", "TEST", "test" );
}

void RestClient::HTTPGet(QString urlDir, QString keyParam ,QString ValueParam, QString usr, QString pass)
{
    if(urlDir.isEmpty() || keyParam.isEmpty() || ValueParam.isEmpty()){
        qDebug() << "HTTPGet() Error: url o params vacios";
        return;
    }

    // convierto a base64 el usario y password
    QString auth( usr + ":" + pass );
    QByteArray baAuth( auth.toUtf8() );
    baAuth = baAuth.toBase64();

    // armo la url donde voy a hacer el GET
    QUrl url( urlDir + "?" + keyParam + "=" + ValueParam );
    QNetworkRequest nr( url );
    // seteo la autorizacion
    nr.setRawHeader( "Authorization", "Basic " + baAuth );
    getNam()->get( nr );

    qDebug() << url.toString();

    // Esta fomra de autenticar esta basado en: https://en.wikipedia.org/wiki/Basic_access_authentication
}

void RestClient::slot_GETresponse( QNetworkReply * r )
{
    int HTTPstatus = r->attribute( QNetworkRequest::Attribute::HttpStatusCodeAttribute ).toInt();
    qDebug() << "HTTP status code:" << HTTPstatus;

//    qDebug() << "La respuesta fue:" << r->request().url();

    QUrlQuery urlQuery( r->request().url() );
    QString dominioConsultado = urlQuery.queryItemValue( "p_objeto" );

    if( HTTPstatus >= 200 || HTTPstatus < 300 )  {

        QByteArray data = r->readAll();
        qDebug() << "response:" << data << data.size();

        if ( data.contains("S") )  {
//            qDebug() << "Dominio consultado (EXISTE):" << dominioConsultado;
            emit sig_responsePatente( dominioConsultado, true );
        }

        if ( data.contains("N") )  {
//            qDebug() << "Dominio consultado (NO EXISTE):" << dominioConsultado;
            emit sig_responsePatente( dominioConsultado, false );
        }
    }
    else {
        qDebug() << "slot_response Error: codigo de respuesta " << HTTPstatus;
    }
}

QNetworkAccessManager *RestClient::getNam() const
{
    return nam;
}

void RestClient::setNam(QNetworkAccessManager *value)
{
    nam = value;
}
