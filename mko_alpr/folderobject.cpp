#include "folderobject.h"
#include <QDir>

FolderObject::FolderObject(QObject *parent) : QObject(parent),
                                              cantidadImagenesEnFolder( 0 ),
                                              cantidadVideosEnFolder( 0 ),
                                              indexImagenes( 0 ),
                                              indexVideos( 0 ),
                                              player( new QMediaPlayer( this ) ),
                                              grabber( new Grabber( this ) ),
                                              frame( new cv::Mat() )

{

}

QString FolderObject::getDirectorio() const
{
    return directorio;
}

/**
 * @brief FolderObject::setDirectorio Metodo que setea el directorio y tambien buscar los archivos y deja todo listo
 * para poder navegar tal directorio.
 * @param value
 */
void FolderObject::setDirectorio(const QString &value)
{
    this->clear();

    directorio = value;

    QDir dir( directorio );
    QStringList filtersImagenes;
    filtersImagenes << "*.jpg" << "*.png";

    listaImagenes = dir.entryInfoList( filtersImagenes );

    cantidadImagenesEnFolder = listaImagenes.size();

#ifndef SIN_DEBUG

    for ( int i = 0 ; i < listaImagenes.size() ; i++ )  {
        qDebug() << listaImagenes.at( i ).fileName();
    }

#endif

    QStringList filtersVideos;
    filtersVideos << "*.mp4";

    listaVideos = dir.entryInfoList( filtersVideos );

    cantidadVideosEnFolder = listaVideos.size();

    for ( int i = 0 ; i < listaVideos.size() ; i++ )  {
        qDebug() << listaVideos.at( i ).fileName();
    }

    indexImagenes = 0;
    indexVideos = 0;
}

void FolderObject::clear()
{
    this->directorio = "";
    cantidadImagenesEnFolder = 0;
    cantidadVideosEnFolder = 0;
    listaImagenes.clear();
    listaVideos.clear();
    indexImagenes = 0;
    indexVideos = 0;
}

/**
 * @brief FolderObject::getNextResource Devuleve el proximo frame para procesar. De todos las imagenes y videos
 * que estan en la carpeta, arma toda una bola de imagenes para devolverlas a traves de esta metodos.
 * @return
 */
cv::Mat* FolderObject::getNextResource()
{
    // Esta es una validacion simple para asegurarnos que no se intente acceder a un index fuera de rango
    if ( listaImagenes.size() > this->indexImagenes )  {
//        qDebug() << "procesar imagen" << listaImagenes.at( this->indexImagenes ).absoluteFilePath();

        cv::Mat imagen = cv::imread( listaImagenes.at( this->indexImagenes ).absoluteFilePath().toStdString().c_str(),
                                     CV_LOAD_IMAGE_COLOR );

        delete frame;
        frame = new cv::Mat( imagen );

        this->indexImagenes++;

    }
    else if ( listaVideos.size() > this->indexVideos )  {
        qDebug() << "procesar video" << listaVideos.at( this->indexVideos ).absoluteFilePath();

        delete frame;
        frame = new cv::Mat( 640, 480, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
        cv::putText( *frame, "Videos no disponibles", cv::Point2f( 20, 400 ),
                     cv::FONT_HERSHEY_PLAIN, 2,  cv::Scalar( 0, 0, 0, 255 ), 2 );

        this->indexVideos++;
    }
    else  {
        delete frame;
        frame = new cv::Mat( 640, 480, CV_8UC3, cv::Scalar( 0, 0, 0 ) );
        cv::putText( *frame, "No hay mas imagenes", cv::Point2f( 20, 400 ),
                     cv::FONT_HERSHEY_PLAIN, 2,  cv::Scalar( 255, 255, 255, 255 ), 2 );

        emit signal_noHayMasParaProcesar();
    }


    return frame;
}

