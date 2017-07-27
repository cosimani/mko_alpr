#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#define ESTADO_1 QString::fromUtf8( "Detectando patentes *")
#define ESTADO_2 QString::fromUtf8( "Detectando patentes **")
#define ESTADO_3 QString::fromUtf8( "Detectando patentes ***")
#define ESTADO_4 QString::fromUtf8( "Detectando patentes ****")
#define ESTADO_5 QString::fromUtf8( "Detectando patentes *****")
#define ESTADO_6 QString::fromUtf8( "Detectando patentes ******")
#define ESTADO_7 QString::fromUtf8( "Detectando patentes *******")
#define ESTADO_8 QString::fromUtf8( "Detectando patentes ********")
#define ESTADO_9 QString::fromUtf8( "Detectando patentes *********")
#define ESTADO_10 QString::fromUtf8("Detectando patentes **********")

#include <QWidget>
#include <QTimer>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdexcept>

#ifdef DEBUG_OSI
#include "openalpr/config.h"
#endif

#include "alpr.h"
#include "config.h"
#include "logger.h"
#include "database.h"
#include "camera.h"

#include "configobject.h"

#include "restclient.h"

namespace Ui
{
    class Principal;
}

class Principal : public QWidget
{
    Q_OBJECT

public:
    explicit Principal(QWidget *parent = NULL);
    void setSessionId(int sessionId);
    void setUser(QString user);
    void setInterface(bool enable);
    void initCameraAndDetection( ConfigObject *configObject );
    void releaseCameraAndDetection();
    ~Principal();

    void setTabFocus(bool focus);
private:
    Ui::Principal *ui;

    int contadorEfectoEstado;
    Camera * camera;
    QTimer * timerEstado;
    QTimer * timerPatentes;
    int sessionId;
    QStringList sessionDomains;

    RestClient *restClient;

    void dimensionarWidgets();

    ConfigObject * configObject;

protected:
    void showEvent( QShowEvent * );
    void hideEvent( QHideEvent * );

    void resizeEvent(QResizeEvent *);

private slots:
    void slot_estado();
    void slot_prueba();
    void slot_testsuite();
    void slot_cerrarSesion();

    void process( cv::Mat * frame );

    void slot_dominioConsultado( QString dominio, bool registrado );

    void slot_procesarVideo();

signals:
    void closeSession();
};

#endif // PRINCIPAL_H
