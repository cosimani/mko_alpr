#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <QObject>

class ConfigObject : public QObject
{
    Q_OBJECT
public:
    enum Fuente  { SIN_DEFINIR, IPCAM, USBCAM, FOLDER };

    explicit ConfigObject(QObject *parent = 0);

    Fuente getFuente() const;
    void setFuente(const Fuente &value);

    int getIndexCamUSB() const;
    void setIndexCamUSB(int value);

    QString getFolder() const;
    void setFolder(const QString &value);

    QString getIpCam() const;
    void setIpCam(const QString &value);

    /**
     * @brief clear Borra toda la info de este objeto
     */
    void clear();

    QString getUrlWebService() const;
    void setUrlWebService(const QString &value);

private:
    Fuente fuente;

    /**
     * @brief indexCamUSB Es el numero de camara USB. El que se usara para el metodo open de VideCapture
     */
    int indexCamUSB;

    /**
     * @brief folder Carpeta donde estan las imagenes y/o videos que se desean procesar
     */
    QString folder;

    /**
     * @brief ipCam IP de la camara que abastece de imagenes en tiempo real
     */
    QString ipCam;

    QString urlWebService;


signals:

public slots:
};

#endif // CONFIGOBJECT_H
