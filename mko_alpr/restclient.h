#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#define URL_EMPRESA "http://190.57.233.66:8086/xml_demo/xml_api.get_objeto_3"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QObject>

class RestClient : public QObject
{
    Q_OBJECT

    QNetworkAccessManager * nam;

public:

    RestClient( QObject *parent = 0 );

    /**
     * @brief HTTPGet metodo para hacer un GET a la url que se la pasa como parametro.
     * permite pasar solo un parametro.
     * @param url direccion donde se hara el GET
     * @param keyParam nombre clave del parametro que se pasa
     * @param ValueParam valor del parametro que se pasa
     * @param usr opcional, si se necesita autorizacion se pasa el usuario
     * @param pass opcional, si se necesita autorizacion se pasa el usuario
     */
    void HTTPGet(QString url, QString keyParam, QString ValueParam, QString usr = "", QString pass = "");

    QNetworkAccessManager *getNam() const;
    void setNam(QNetworkAccessManager *value);

private slots:

    /**
     * @brief slot_response obtiena la respuesta del metodo
     * HTTPGET(), verifica si el codigo http esta entre [200-300),
     * y en ese caso activa la señal, sig_responsePatente().
     * Sino no hace nada.
     */
    void slot_GETresponse(QNetworkReply*r);

signals:
    void sig_responsePatente( QString dominio, bool registrada );
};

#endif // RESTCLIENT_H
