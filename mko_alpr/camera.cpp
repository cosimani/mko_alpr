#include "camera.h"

#include <QDebug>
#include <QDir>

Camera::Camera( QObject *parent ) : QObject( parent ),
                                    videoCapture( new VideoCapture() ),
                                    timer( new QTimer( this ) ),
                                    frame( new Mat() ),
                                    tipoCaptura( USBCAM ),
                                    folderObject( new FolderObject( this ) )
{
    connect( timer, SIGNAL( timeout() ), SLOT( process() ) );

    connect( folderObject, SIGNAL( signal_noHayMasParaProcesar() ),
             timer, SLOT( stop() ) );
}

QTimer *Camera::getTimer() const
{
    return timer;
}

void Camera::procesarVideo( QString archivo )
{
    this->release();

    tipoCaptura = VIDEO;

    videoCapture->open( archivo.toStdString().c_str() );

    if ( ! videoCapture->isOpened() )  {
        qDebug() << "No se pueden adquirir imagenes del video seleccionado";
        return;
    }

    timer->start( 10 );

}

void Camera::initCamaraUSB( int indexCamera )
{
    tipoCaptura = USBCAM;

    videoCapture->open( indexCamera );

    if ( ! videoCapture->isOpened() )  {
        qDebug() << "No se pueden adquirir imagenes de la camara USB";
        return;
    }

    timer->start( 10 );
}

/**
 * @brief Camera::initFolder
 * @param directorio
 */
void Camera::initFolder( QString directorio )
{
    tipoCaptura = FOLDER;

    folderObject->setDirectorio( directorio );

    timer->start( 500 );
}

void Camera::initCamaraIP( QString url )
{
    tipoCaptura = IPCAM;

    try  {
        //    videoCapture->open( indexCamera );
        videoCapture->open( url.toStdString().c_str() );
        //    videoCapture->open( "rtp://127.0.0.1" );
    }
    catch( ... )  {
        qDebug() << "Exception";
    }

    if ( ! videoCapture->isOpened() )  {
        qDebug() << "No se pueden adquirir imagenes de la camara IP";
        return;
    }

    timer->start( 10 );
}


void Camera::release()
{
    timer->stop();
    videoCapture->release();    
}

Camera::~Camera()
{
    videoCapture->release();
}

void Camera::process()
{
    switch( tipoCaptura )  {

    case VIDEO:
        if ( ! videoCapture->isOpened() )
            return;

        videoCapture->operator >>( *frame );

//        cv::flip( *frame, *frame, 1 );

        // Con esta signal se envia el nuevo Mat para que lo pueda usar CameraWidget
        emit newCameraFrame( frame );
        break;


    case USBCAM:
        if ( ! videoCapture->isOpened() )
            return;

        videoCapture->operator >>( *frame );

//        cv::flip( *frame, *frame, 1 );

        // Con esta signal se envia el nuevo Mat para que lo pueda usar CameraWidget
        emit newCameraFrame( frame );
        break;

    case IPCAM:        
        if ( ! videoCapture->isOpened() )
            return;

        videoCapture->operator >>( *frame );

        // Con esta signal se envia el nuevo Mat para que lo pueda usar CameraWidget
        emit newCameraFrame( frame );
        break;

    case FOLDER:
        frame = folderObject->getNextResource();
//        qDebug() << "despues";

//        videoCapture->open( listaImagenes.at( ) );

//        if ( ! videoCapture->isOpened() )
//            return;

//        videoCapture->operator >>( *frame );

//        cv::flip( *frame, *frame, 1 );

        // Con esta signal se envia el nuevo Mat para que lo pueda usar CameraWidget
        emit newCameraFrame( frame );
        break;

    }


}
