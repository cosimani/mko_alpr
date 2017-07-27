#include "principal.h"
#include "ui_principal.h"
#include "configwidget.h"

#include "util.h"

#include <QMessageBox>

#include <QFileDialog>
#include <QFile>
#include <QDateTime>

Principal::Principal(QWidget *parent) : QWidget(parent),
                                        ui(new Ui::Principal),
                                        contadorEfectoEstado( 1 ),
                                        camera( new Camera( this ) ),
                                        timerEstado( new QTimer( this ) ),
                                        timerPatentes( new QTimer( this ) ),
                                        sessionId( 0 ),
                                        restClient( new RestClient( this ) )
{
    ui->setupUi(this);

    connect( ui->pbCerrarSesion, SIGNAL( pressed() ), this, SLOT( slot_cerrarSesion() ) );

    connect( ui->pbProcesarVideo, SIGNAL( pressed() ), this, SLOT( slot_procesarVideo() ) );

    connect( camera, SIGNAL( newCameraFrame( cv::Mat* ) ),
             ui->cameraWidget, SLOT( slot_setCameraTexture( cv::Mat* ) ) );

    connect( camera, SIGNAL( newCameraFrame( cv::Mat* ) ),
             this, SLOT( process( cv::Mat* ) ) );

    connect(timerEstado, SIGNAL(timeout()), this, SLOT(slot_estado()));
//    connect(timerPatentes, SIGNAL(timeout()), this, SLOT(slot_prueba()));
//    connect(ui->pbTestSuite, SIGNAL(pressed()), this, SLOT(slot_testsuite()));

    connect( restClient, SIGNAL( sig_responsePatente( QString, bool ) ),
             this, SLOT( slot_dominioConsultado( QString, bool ) ) );
}

void Principal::setSessionId(int sessionId)
{
    sessionDomains.clear();
    this->sessionId = sessionId;
}

void Principal::setUser(QString user)
{
    Q_UNUSED(user)
    //ui->lbUsuario->setText( "Hola, " + Util::getInstance()->toCamelCase( user ) + "!" );
}

void Principal::setInterface(bool enable)
{
    ui->pbCerrarSesion->setEnabled(enable);
}

void Principal::initCameraAndDetection( ConfigObject * configObject )
{
    this->configObject = configObject;

    if ( configObject->getFuente() == ConfigObject::USBCAM )  {
        camera->initCamaraUSB( configObject->getIndexCamUSB() );
        ui->pbProcesarVideo->setEnabled( false );
    }
    else  if ( configObject->getFuente() == ConfigObject::IPCAM )  {
        camera->initCamaraIP( configObject->getIpCam() );
        ui->pbProcesarVideo->setEnabled( false );
    }
    if ( configObject->getFuente() == ConfigObject::FOLDER )  {
        camera->initFolder( configObject->getFolder() );
    }

//    timerPatentes->start(3000);
    timerEstado->start(200);
}

void Principal::releaseCameraAndDetection()
{
    timerEstado->stop();
    timerPatentes->stop();
    camera->release();
}

void Principal::slot_prueba()
{
    // initialize the library using ar style license plates
    alpr::Alpr openalpr(
            Config::getInstance()->getStdString("country"),
            Config::getInstance()->getStdString("config_file"),
            Config::getInstance()->getStdString("runtime_dir"));

    openalpr.setDefaultRegion("arg");

    // top n possible plates
    openalpr.setTopN(
            Config::getInstance()->getString("max_count").toInt());

    // make sure the library loaded before continuing
    if(openalpr.isLoaded() == false)
    {
        LOG_ERR("Principal: Error loading OpenALPR");
        return;
    }

    std::string imageFile =
            Config::getInstance()->getStdString("example_image");

    // Recognize an image file.  You could alternatively provide the image bytes in-memory.
    // Castear imagen de la camara en array de char o lo que sirva...
    alpr::AlprResults results =
            openalpr.recognize(imageFile);

    // Iterate through the results.  There may be multiple plates in an image,
    // and each plate return sthe top N candidates.
    for (unsigned int i = 0; i < results.plates.size(); i++)
    {
        alpr::AlprPlateResult plate = results.plates[i];

        LOG_INF("Principal: Plate [" + QString::number(i+1) + "]: " +
                QString::number(plate.topNPlates.size()) + " results");

        for (unsigned int k = 0; k < plate.topNPlates.size(); k++)
        {
            alpr::AlprPlate candidate = plate.topNPlates[k];

            QString matches = candidate.matches_template ? "True" : "False";

            LOG_INF("Principal: " +
                    QString::fromUtf8(candidate.characters.c_str()) +
                    " - Confidence: " +
                    QString::number(candidate.overall_confidence) +
                    " - Pattern_match: " + matches);
        }
    }
}

void Principal::slot_estado()
{
    switch(contadorEfectoEstado)
    {
    case 1:
        ui->lbEstado->setText(ESTADO_1); break;
    case 2:
        ui->lbEstado->setText(ESTADO_2); break;
    case 3:
        ui->lbEstado->setText(ESTADO_3); break;
    case 4:
        ui->lbEstado->setText(ESTADO_4); break;
    case 5:
        ui->lbEstado->setText(ESTADO_5); break;
    case 6:
        ui->lbEstado->setText(ESTADO_6); break;
    case 7:
        ui->lbEstado->setText(ESTADO_7); break;
    case 8:
        ui->lbEstado->setText(ESTADO_8); break;
    case 9:
        ui->lbEstado->setText(ESTADO_9); break;
    case 10:
        ui->lbEstado->setText(ESTADO_10); break;

    default:
        break;
    }
    if(contadorEfectoEstado < 10) contadorEfectoEstado++;
    else contadorEfectoEstado = 1;
}

void Principal::slot_testsuite()
{
    /*Estadistica*/
    int cantidadimagenes=0;
    int cantidadcorrectas=0;

    try
    {

        /*Configuracion de openalpr*/
        // initialize the library using ar style license plates
        alpr::Alpr openalpr(/*"us","/usr/share/openalpr/config/openalpr.defaults.conf"*/

                            Config::getInstance()->getStdString("country"),
                            Config::getInstance()->getStdString("config_file"),
                            Config::getInstance()->getStdString("runtime_dir")

            );
       openalpr.setDefaultRegion( Config::getInstance()->getStdString( "region" ) );

        // top n possible plates
        openalpr.setTopN(
                Config::getInstance()->getString("max_count").toInt());

        // make sure the library loaded before continuing
        if(openalpr.isLoaded() == false)
        {
            throw new runtime_error("Error loading OpenALPR");
            return;
        }

        /*Leer linea por linea el archivo lista.txt nombreimagen:resultado esperado */
        LOG_INF("Principal: Inicio de pruebas");

        QFile inputFile("/home/patentes/bdpatentes/lista.txt");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
              cantidadimagenes++;
              QString line = in.readLine();
              QStringList imagenxresult = line.split(':');

              QString imageFile = imagenxresult[0];
              QString resultadoesperado = imagenxresult[1];

              // Recognize an image file.  You could alternatively provide the image bytes in-memory.
              alpr::AlprResults results =
                      openalpr.recognize("/home/patentes/bdpatentes/"+imageFile.toStdString());

              LOG_INF("Principal: ANALIZANDO ARCHIVO: " + imageFile);
              if(results.plates.size()==0 && resultadoesperado==""){
                  cantidadcorrectas++;
              }

              // Iterate through the results.  There may be multiple plates in an image,
              // and each plate return sthe top N candidates.


              for (unsigned int i = 0; i < results.plates.size(); i++)
              {
                  alpr::AlprPlateResult plate = results.plates[i];

                  LOG_INF("Principal: Plate [" + QString::number(i+1) + "]: " +
                          QString::number(plate.topNPlates.size()) + " results");

                  bool flag=true;
                  for (unsigned int k = 0; k < plate.topNPlates.size(); k++)
                  {
                      alpr::AlprPlate candidate = plate.topNPlates[k];

                      QString matches = candidate.matches_template ? "True" : "False";

                      if(matches=="True"){
                          LOG_INF("Principal: " +
                                  QString::fromUtf8(candidate.characters.c_str()) +
                                  " - Confidence: " +
                                  QString::number(candidate.overall_confidence) +
                                  " - Resultado Esperado: " + resultadoesperado);
                          if(QString::fromUtf8(candidate.characters.c_str()) == resultadoesperado ){
                              cantidadcorrectas++;
                              flag=false;
                          }


                      }

                  }
                  if(flag && resultadoesperado==""){
                      cantidadcorrectas++;
                  }
              }


           }
           /*Calculo Estadistica*/
           LOG_INF("Principal: Procesadas: " + QString::number(cantidadimagenes));
           LOG_INF("Principal: Correctas: " + QString::number(cantidadcorrectas));

           double porcentaje = cantidadcorrectas * 100.0 / cantidadimagenes;
           LOG_INF("Principal: Porcentaje: " + QString::number(porcentaje));

           inputFile.close();
        }
    }
    catch(runtime_error *e)
    {
        Logger::getInstance()->err(e->what());
        return;
    }
}

void Principal::slot_cerrarSesion()
{
    emit closeSession();
}



Principal::~Principal()
{
    delete ui;
}

void Principal::setTabFocus(bool focus)
{
    if ( focus )  {
        ui->cameraWidget->setFocusPolicy( Qt::NoFocus );
        ui->pbCerrarSesion->setFocusPolicy( Qt::NoFocus );
        ui->pbProcesarVideo->setFocusPolicy( Qt::NoFocus );
    }
    else  {
        ui->cameraWidget->setFocusPolicy( Qt::NoFocus );
        ui->pbCerrarSesion->setFocusPolicy( Qt::NoFocus );
        ui->pbProcesarVideo->setFocusPolicy( Qt::NoFocus );
    }
}

void Principal::dimensionarWidgets()
{
    int anchoPantalla = this->width();
    int altoPantalla = this->height();

    int cantidadLineasDeWidgets = 8;
    int cantidadBordesEntreWidgets = cantidadLineasDeWidgets + 1;

    int altoWidget = altoPantalla / 15;
    int borde = ( altoPantalla - cantidadLineasDeWidgets * altoWidget ) / cantidadBordesEntreWidgets;
    int anchoWidgetsCol0 = anchoPantalla / 3;
    int anchoWidgetsCol1 = anchoPantalla - 3 * borde - anchoWidgetsCol0;
    int anchoWidgetsCamara = anchoPantalla - 3 * borde - anchoWidgetsCol0;

    Q_UNUSED(anchoWidgetsCol1);
    Q_UNUSED(anchoWidgetsCamara);

    QFont fontBethHand( "BethHand", altoWidget / 1.7f, QFont::Normal );
//    QFont fontAngelina( "Angelina", 15, QFont::Normal );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );

    this->setFont( fontBethHand );
    this->setPalette( palette );

    ui->lbEstado->setGeometry( QRect( borde, borde, anchoWidgetsCol0, altoWidget ) );

    ui->pbProcesarVideo->setGeometry( QRect( borde, altoPantalla - borde - altoWidget,
                                             anchoWidgetsCol0, altoWidget ) );

    ui->pbCerrarSesion->setGeometry( QRect( anchoPantalla - borde - anchoWidgetsCol0,
                                            altoPantalla - borde - altoWidget,
                                            anchoWidgetsCol0, altoWidget ) );

    int altoCamara = altoPantalla - 4 * borde - 2 * altoWidget;
    int anchoCamara = 4 * altoCamara / 3;

    ui->cameraWidget->setGeometry( QRect( anchoPantalla / 2 - anchoCamara / 2,
                                          altoWidget + 2 * borde,
                                          anchoCamara, altoCamara ) );

//    QRect rec = QApplication::desktop()->screenGeometry();
//    int anchoScreen = rec.width();
//    int altoScreen = rec.height();

//    this->resize( anchoScreen / 3, altoScreen / 3 );

//    QFontMetrics fm( QFont( "BethHand" ) );
//    int altoFuente = fm.height();

//    QFont fontBethHand( "BethHand", 25, QFont::Normal );
//    ui->userLineEdit->setFont( fontBethHand );
//    ui->passwordLineEdit->setFont( fontBethHand );


//    QFont fontAngelina( "Angelina", 35, QFont::Normal );
////    QFont font( "Angelina" );

//    ui->welcomeLabel->setFont( fontAngelina );
//    ui->pbIngresar->setFont( fontAngelina );

//    QPalette palette = this->palette();
//    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );

//    ui->welcomeLabel->setPalette(palette);
}

void Principal::showEvent(QShowEvent *)
{
    this->dimensionarWidgets();
}

void Principal::hideEvent(QHideEvent *)
{
//    this->setFocusPolicy(Qt::NoFocus);
}

void Principal::process( cv::Mat * frame )
{
    // Si el timer de Camera esta detenido entonces no hace nada.
    // Se puede detener porque no hay mas imagenes en la carpeta o porque el video termino.
    if ( ! camera->getTimer()->isActive() )  {
        this->timerEstado->stop();
        ui->lbEstado->setText( "Sin imagenes para procesar");
        return;
    }

    // Si el frame esta vacio entonces sale
    if ( frame->empty() )  {
         this->timerEstado->stop();
         ui->lbEstado->setText( "Sin imagenes para procesar");
         return;
     }

    // qDebug() << "process inicio";

    // initialize the library using ar style license plates
    alpr::Alpr openalpr(
            Config::getInstance()->getStdString( "country" ),
            Config::getInstance()->getStdString( "config_file" ),
            Config::getInstance()->getStdString( "runtime_dir" ) );

    openalpr.setDefaultRegion( Config::getInstance()->getStdString( "region" ) );



#ifdef DEBUG_OSI

    openalpr.getConfig()->setDebug( true );

    alpr::Config * config = openalpr.getConfig();

    Util::getInstance()->publicarInfoAlprConfig( config );

#endif

    // top n possible plates
    openalpr.setTopN( Config::getInstance()->getString( "max_count" ).toInt() );

    // make sure the library loaded before continuing
    if( openalpr.isLoaded() == false )
    {
        LOG_ERR( "Principal: Error loading OpenALPR" );
        return;
    }

//    std::string imageFile =
//            Config::getInstance()->getStdString( "example_image" );

    // Recognize an image file.  You could alternatively provide the image bytes in-memory.
    // Castear imagen de la camara en array de char o lo que sirva...
//    alpr::AlprResults results =
//            openalpr.recognize( imageFile );

    std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;
    alpr::AlprRegionOfInterest region( 0, 0, frame->cols, frame->rows );
    regionsOfInterest.push_back( region );


    alpr::AlprResults results = openalpr.recognize( frame->data,
                                                    frame->channels(),
                                                    frame->cols,
                                                    frame->rows,
                                                    regionsOfInterest );



#ifdef DEBUG_OSI

    qDebug() << "results" << results.epoch_time;
    qDebug() << "frame_number" << results.frame_number;
    qDebug() << "img_width" << results.img_width;
    qDebug() << "img_height" << results.img_height;
    qDebug() << "total_processing_time_ms" << results.total_processing_time_ms;
    qDebug() << "cantidad de alpr::AlprPlateResult" << results.plates.size();

    for ( unsigned int i=0 ; i<results.plates.size() ; i++ )  {

        alpr::AlprPlateResult alprPlateResult = results.plates.at( i );
        // The number requested is always >= the topNPlates count
        qDebug() << "requested_topn" << alprPlateResult.requested_topn;

        // The country (training data code) that was used to recognize the plate
        qDebug() << "country" << alprPlateResult.country.c_str();

        // the best plate is the topNPlate with the highest confidence
//        qDebug() << "requested_topn" << plateResult.requested_topn;
//        AlprPlate bestPlate;

        // A list of possible plate number permutations
        qDebug() << "Cantidad de alpr::AlprPlate" << alprPlateResult.topNPlates.size();

        for ( unsigned int j=0 ; j<alprPlateResult.topNPlates.size() ; j++ )  {

            alpr::AlprPlate alprPlate = alprPlateResult.topNPlates.at( j );
            qDebug() << "characters" << alprPlate.characters.c_str();
            qDebug() << "overall_confidence" << alprPlate.overall_confidence;
            qDebug() << "matches_template" << alprPlate.matches_template;

            qDebug() << "Cantidad de alpr::AlprChar" << alprPlate.character_details.size();

            for ( unsigned int k=0 ; k<alprPlate.character_details.size() ; k++ )  {
                alpr::AlprChar alprChar = alprPlate.character_details.at( k );

                qDebug() << "character" << alprChar.character.c_str();
                qDebug() << "confidence" << alprChar.confidence;
//                alpr::AlprCoordinate cornersLocal[4];
                qDebug() << "AlprCoordinate" << alprChar.corners[0].x << alprChar.corners[0].y
                            << alprChar.corners[1].x << alprChar.corners[1].y
                               << alprChar.corners[2].x << alprChar.corners[2].y
                                  << alprChar.corners[3].x << alprChar.corners[3].y;
            }

        }

        // The processing time for this plate
        qDebug() << "processing_time_ms" << alprPlateResult.processing_time_ms;

        // the X/Y coordinates of the corners of the plate (clock-wise from top-left)
//        qDebug() << "requested_topn" << plateResult.requested_topn;
//        AlprCoordinate plate_points[4];

        // The index of the plate if there were multiple plates returned
        qDebug() << "plate_index" << alprPlateResult.plate_index;

        // When region detection is enabled, this returns the region.  Region detection is experimental
        qDebug() << "regionConfidence" << alprPlateResult.regionConfidence;
        qDebug() << "region" << alprPlateResult.region.c_str();

    }

    qDebug() << "cantidad de alpr::AlprRegionOfInterest" << results.regionsOfInterest.size();

    for ( unsigned int i=0 ; i<results.regionsOfInterest.size() ; i++ )  {
        alpr::AlprRegionOfInterest alprRegionOfInterest = results.regionsOfInterest.at( i );
        qDebug() << alprRegionOfInterest.x
                 << alprRegionOfInterest.y
                 << alprRegionOfInterest.width
                 << alprRegionOfInterest.height;
    }

#endif



    // Iterate through the results.  There may be multiple plates in an image,
    // and each plate return sthe top N candidates.
    for ( unsigned int i = 0 ; i < results.plates.size() ; i++ )
    {
        bool pdidReg = false;
        int pdid = -1;
        // si este for da mas de una vuelta, significa que se detecto
        // mas de una patente en la misma imagen

        alpr::AlprPlateResult plate = results.plates[ i ];




        //qDebug() << "Plate [" << QString::number( i+1 ) << "]: " << plate.topNPlates.size() << " results";

        for ( unsigned int k = 0 ; k < plate.topNPlates.size() ; k++ )
        {
            alpr::AlprPlate candidate = plate.topNPlates[ k ];
            bool m = candidate.matches_template;

            // Procesa los que no hayan sido procesados en la sesion
            if ( m )  {
                QString domain = QString::fromUtf8( candidate.characters.c_str() );
                if(sessionDomains.contains( domain ) )
                {
                    continue;
                }
                sessionDomains.append( domain );
                if( sessionDomains.size() == 500 )
                {
                    sessionDomains.removeFirst();
                }
                if( pdidReg == false )
                {
                    pdid = Database::getInstance()->plateDetection( sessionId );
                    pdidReg = true;

                    // the X/Y coordinates of the corners of the plate (clock-wise from top-left)
                    alpr::AlprCoordinate plate_points[4] = { plate.plate_points[0],
                                                             plate.plate_points[1],
                                                             plate.plate_points[2],
                                                             plate.plate_points[3] };

//                    qDebug() << "alpr::AlprCoordinate" << i;
//                    qDebug() << plate_points[0].x << plate_points[0].y;
//                    qDebug() << plate_points[1].x << plate_points[1].y;
//                    qDebug() << plate_points[2].x << plate_points[2].y;
//                    qDebug() << plate_points[3].x << plate_points[3].y;

                    int borde = 30;  // Pixeles para ampliar el borde de la patente en la imagen a guardar en disco

                    int xPlate = qMin( plate_points[0].x, plate_points[3].x );
                    int yPlate = qMin( plate_points[0].y, plate_points[1].y );

                    if ( ( xPlate - borde ) > 0 )  // agrega borde simpre que no se exceda
                        xPlate -= borde;

                    if ( ( yPlate - borde ) > 0 )  // agrega borde simpre que no se exceda
                        yPlate -= borde;

                    // Validaciones
                    xPlate = qMax( xPlate, 0 );
                    yPlate = qMax( yPlate, 0 );

                    int wPlate = qMax( plate_points[1].x, plate_points[2].x ) - xPlate;
                    int hPlate = qMax( plate_points[2].y, plate_points[3].y ) - yPlate;

                    if ( ( xPlate + wPlate + borde ) < frame->cols )  // agrega borde simpre que no se exceda
                        wPlate += borde;

                    if ( ( yPlate + hPlate + borde ) < frame->rows )  // agrega borde simpre que no se exceda
                        hPlate += borde;

                    // Validaciones
//                    wPlate = qMin( wPlate, frame->cols - xPlate );
//                    hPlate = qMin( hPlate, frame->rows - yPlate );

                    // fecha y hora para el nombre del archivo para guardar en /reg y en /evidencias
                    QDateTime dateTime = QDateTime::currentDateTime();
                    QString sDateTime = dateTime.toString("yyyyMMdd_hh-mm-ss_");

                    // Esta es la ultima validacion que hacemos para que no se clave opencv
                    // Si no se cumple no grabamos en disco el recorte sino la imagen completa
                    if ( 0 <= xPlate &&
                         0 <= wPlate &&
                         ( xPlate + wPlate ) <= frame->cols &&
                         0 <= yPlate &&
                         0 <= hPlate &&
                         ( yPlate + hPlate ) <= frame->rows )  {

                        cv::Rect roi( xPlate, yPlate, wPlate, hPlate );
                        cv::Mat image_roi = frame->operator ()( roi );

                        QString nombreArchivo = "../reg/" + sDateTime + domain + ".png";

                        // Almacena el roi donde esta la patente y como nombre de archivo lo que se detecto.
                        // Posiblemente no coincida el nombre del archivo con la patente detectada
                        cv::imwrite( nombreArchivo.toStdString().c_str(), image_roi );
                    }
                    else  {  // Aca almacena la imagen completa cuando no se puede recortar correctamente el dominio

                        QString nombreArchivo = "../reg/" + sDateTime + domain + ".png";
                        cv::imwrite( nombreArchivo.toStdString().c_str(), *frame );
                    }

                    // Aqui se almacena la imagen completa, como evidencia.
                    QString nombreArchivoImCompleta = "../evidencias/" + sDateTime + domain + ".png";
                    cv::imwrite( nombreArchivoImCompleta.toStdString().c_str(), *frame );


                    if( pdid != -1 )
                    {
                        QString matches = m ? "s":"n";
                        float c = candidate.overall_confidence;

                        // Sera registrado siempre que la confianza sea mayor a este porcentaje
                        if ( c > 75.0f )  {

                            Database::getInstance()->candidate( domain, matches, c, pdid );

                            qDebug() << "Principal: "
                                     + QString( domain + " detected and matched ("
                                     + QString::number( candidate.overall_confidence ) + ")" );


                            if ( configObject->getUrlWebService() != "" )  {

                                qDebug() << "Entro para usar webservice" << configObject->getUrlWebService();

                                restClient->HTTPGet( configObject->getUrlWebService(),
                                                     "p_objeto",
                                                     domain,
                                                     "TEST",
                                                     "test" );
                            }
                            else  {
                                qDebug() << "NO   Entro para usar webservice";
                            }
                        }
                    }
                }

                // Este log se realizara en la clase RestClient con la respuesta del webservice
//                LOG_INF( "Principal: "
//                         + QString( domain + " detected and matched ("
//                         + QString::number( candidate.overall_confidence ) + ")" ) );
            }
        }
    }

    //qDebug() << "process fin";
}

void Principal::slot_dominioConsultado( QString dominio, bool registrado )
{
    if ( registrado )  {
        qDebug() << QString( dominio + " detectado - EXISTE" );
        LOG_INF( QString( dominio + " detectado - EXISTE" ) );
    }
    else  {
        qDebug() << QString( dominio + " detectado - NO EXISTE ---" );
        LOG_INF( QString( dominio + " detectado - NO EXISTE ---" ) );
    }
}

void Principal::slot_procesarVideo()
{
    if ( this->camera->getTimer()->isActive() )  {
        QMessageBox::critical( this, "Aun esta procesando imagenes",
                               "Si desea procesar un video debera esperar que se finalice con las imagenes "
                               "de la carpeta elegida. O cierre la sesion e comience de nuevo.");
        return;
    }
    else {
        QString archivo = QFileDialog::getOpenFileName(this, "Elija el recurso", "../", "Multimedia (*.mp4 *.dav)");

        QFile file( archivo );
        if ( ! file.exists() )  {
            QMessageBox::critical( this, "El archivo no se puede abrir",
                                   "Por favor, intente de nuevo o seleccione otro archivo de video.");
            return;
        }
        else  {
            this->camera->procesarVideo( archivo );
            this->timerEstado->start();
        }
    }
}

void Principal::resizeEvent(QResizeEvent *)
{
    this->dimensionarWidgets();
}


