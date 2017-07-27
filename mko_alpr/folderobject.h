#ifndef FOLDEROBJECT_H
#define FOLDEROBJECT_H

#include <QObject>
#include <QFileInfoList>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QMediaPlayer>
#include <QGLWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QFileInfo>

class Grabber : public QAbstractVideoSurface  {
    Q_OBJECT

public:

    Grabber( QObject *parent = 0 ) : QAbstractVideoSurface( parent )  {

    }

    QList< QVideoFrame::PixelFormat > supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle ) const
    {
        if( handleType == QAbstractVideoBuffer::NoHandle )  {
             return QList< QVideoFrame::PixelFormat >() << QVideoFrame::Format_RGB32
                                                        << QVideoFrame::Format_ARGB32
                                                        << QVideoFrame::Format_ARGB32_Premultiplied
                                                        << QVideoFrame::Format_RGB32
                                                        << QVideoFrame::Format_RGB24
                                                        << QVideoFrame::Format_BGRA32
                                                        << QVideoFrame::Format_BGRA32_Premultiplied
                                                        << QVideoFrame::Format_BGR32
                                                        << QVideoFrame::Format_Jpeg
                                                        << QVideoFrame::Format_CameraRaw
                                                        << QVideoFrame::Format_AdobeDng;

        }
        else  {
            return QList< QVideoFrame::PixelFormat >();
        }
    }

    bool start( const QVideoSurfaceFormat &format )  {
        const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( format.pixelFormat() );
        const QSize size = format.frameSize();

        if ( imageFormat != QImage::Format_Invalid && !size.isEmpty() )  {
            QAbstractVideoSurface::start( format );
            return true;
        }
        else  {
            return false;
        }
    }

    bool present( const QVideoFrame &frame )  {
        QVideoFrame cloneFrame( frame );

        if ( ! cloneFrame.map( QAbstractVideoBuffer::ReadOnly ))  {
            return false;
        }

        this->videoFrameActual = cloneFrame;

        return true;
    }

    QVideoFrame getVideoFrameActual() const  {
        return videoFrameActual;
    }


private:
    QVideoFrame videoFrameActual;
};


/**
 * @brief The FolderObject class Clase que permite mantener la informacion de la carpeta donde estan los
 * archivos que se desean procesar. Pueden ser imagenes o videos y cada uno de ellos requiere un trato especial.
  */
class FolderObject : public QObject
{
    Q_OBJECT
public:
    explicit FolderObject(QObject *parent = 0);

    QString getDirectorio() const;
    void setDirectorio(const QString &value);

    void clear();

    cv::Mat* getNextResource();

private:
    QString directorio;
    int cantidadImagenesEnFolder;
    int cantidadVideosEnFolder;

    QFileInfoList listaImagenes;
    QFileInfoList listaVideos;

    int indexImagenes;
    int indexVideos;

    QMediaPlayer *player;
    Grabber *grabber;

    cv::Mat * frame;


signals:
    void signal_noHayMasParaProcesar();

public slots:
};

#endif // FOLDEROBJECT_H
