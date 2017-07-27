#include "configwidget.h"
#include "ui_configwidget.h"

#include <QDebug>
#include <QCameraInfo>
#include <QMessageBox>
#include <QDir>


ConfigWidget::ConfigWidget(QWidget *parent) : QWidget(parent),
                                              ui(new Ui::ConfigWidget),
                                              configObject( new ConfigObject( this ) )
{
    ui->setupUi(this);

//    ui->leCamaraIp->setValidator(new QRegExpValidator( QRegExp( "[0-9.]+" ), this ) );

    connect(ui->pbComenzar, SIGNAL(pressed()), this, SLOT(slot_comenzar()));

    connect(ui->cbCamaraUsb, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexChanged(int)));
    connect(ui->cbFuente, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_sourceIndexChanged(int)));
    connect(ui->checkOffline, SIGNAL(toggled(bool)), this, SLOT(slot_checkBoxChanged(bool)));


    this->cargarCamaras();

    this->slot_sourceIndexChanged( 0 );

}

ConfigWidget::~ConfigWidget()
{
    delete ui;
}

void ConfigWidget::setTabFocus(bool focus)
{
    if ( focus )  {
        ui->cbFuente->setFocusPolicy( Qt::StrongFocus );

        ui->lCamaraIp->setFocusPolicy( Qt::NoFocus );
        ui->cbCamaraIp->setFocusPolicy( Qt::StrongFocus );

        ui->lCarpeta->setFocusPolicy( Qt::NoFocus );
        ui->leCarpeta->setFocusPolicy( Qt::StrongFocus );

        ui->lCamaraUsb->setFocusPolicy( Qt::NoFocus );
        ui->cbCamaraUsb->setFocusPolicy( Qt::StrongFocus );

        ui->lWebService->setFocusPolicy( Qt::NoFocus );
        ui->cbWebService->setFocusPolicy( Qt::StrongFocus );

        ui->lBaseRedLocal->setFocusPolicy( Qt::NoFocus );
        ui->cbBaseRedLocal->setFocusPolicy( Qt::StrongFocus );

        ui->checkOffline->setFocusPolicy( Qt::StrongFocus );

        ui->pbComenzar->setFocusPolicy( Qt::StrongFocus );
    }
    else  {
        ui->lFuente->setFocusPolicy( Qt::NoFocus );
        ui->cbFuente->setFocusPolicy( Qt::NoFocus );

        ui->lCamaraIp->setFocusPolicy( Qt::NoFocus );
        ui->cbCamaraIp->setFocusPolicy( Qt::NoFocus );

        ui->lCarpeta->setFocusPolicy( Qt::NoFocus );
        ui->leCarpeta->setFocusPolicy( Qt::NoFocus );

        ui->lCamaraUsb->setFocusPolicy( Qt::NoFocus );
        ui->cbCamaraUsb->setFocusPolicy( Qt::NoFocus );

        ui->lWebService->setFocusPolicy( Qt::NoFocus );
        ui->cbWebService->setFocusPolicy( Qt::NoFocus );

        ui->lBaseRedLocal->setFocusPolicy( Qt::NoFocus );
        ui->cbBaseRedLocal->setFocusPolicy( Qt::NoFocus );

        ui->checkOffline->setFocusPolicy( Qt::NoFocus );

        ui->pbComenzar->setFocusPolicy( Qt::NoFocus );
    }
}

void ConfigWidget::showEvent(QShowEvent *)
{
    this->dimensionarWidgets();
}

void ConfigWidget::hideEvent(QHideEvent *)
{
}

void ConfigWidget::slot_comenzar()
{
    configObject->clear();

    switch ( ui->cbFuente->currentIndex() )  {
    case 0:  // No seleccionado

        break;

    case 1:  // IP
        configObject->setFuente( ConfigObject::IPCAM );
        configObject->setIpCam( ui->cbCamaraIp->currentText() );

        break;

    case 2:  // Disco
        configObject->setFuente( ConfigObject::FOLDER );
        configObject->setFolder( ui->leCarpeta->getDirectorio() );

        break;

    case 3:  // USB
        configObject->setFuente( ConfigObject::USBCAM );
        configObject->setIndexCamUSB( ui->cbCamaraUsb->currentIndex() - 1 );

        break;

    default:;
    }

    ///////////////// Aca colocamos distintas validaciones

    // Si elige USBCAM, entonces se debe indicar cual camara USB quiere usar.
    if ( configObject->getFuente() == ConfigObject::USBCAM && configObject->getIndexCamUSB() == -1 )  {
        QMessageBox::critical( this, "No hay camara USB seleccionada",
                               "Debe seleccionar una camara USB entre las disponibles en la lista desplegable.");
        QPalette paletteComboBox = this->palette();
        paletteComboBox.setColor( QPalette::ButtonText, QColor( 255, 0, 0 ) );
        ui->cbCamaraUsb->setPalette( paletteComboBox );

        return;
    }

    // Si eligio IPCAM, entonces debe seleccionar una del combob box
    if ( configObject->getFuente() == ConfigObject::IPCAM && configObject->getIpCam() == "No seleccionado" )  {
        QMessageBox::critical( this, "No hay camara",
                               "Debe seleccionar una camara IP.");
        QPalette paletteLineEdit = this->palette();
        paletteLineEdit.setColor( QPalette::ButtonText, QColor( 255, 0, 0 ) );
        ui->cbCamaraIp->setPalette( paletteLineEdit );

        return;
    }

    // Si elige FOLDER, entonces se debe indicar un directorio que exista.
    if ( configObject->getFuente() == ConfigObject::FOLDER && configObject->getFolder().isEmpty() )  {
        QMessageBox::critical( this, "No hay directorio seleccionado",
                               "Debe seleccionar un directorio con las imagenes y videos para procesar.");
        QPalette paletteLineEdit = this->palette();
        paletteLineEdit.setColor( QPalette::ButtonText, QColor( 255, 0, 0 ) );
        ui->leCarpeta->setPalette( paletteLineEdit );

        return;
    }


    if ( ! ui->checkOffline->isChecked() )  {

        // Entra cuando se elegio algun webservice, ya sea el interno como el de internet
        if ( ui->cbWebService->currentText() != "No seleccionado" )  {

            configObject->setUrlWebService( "http://" + ui->cbWebService->currentText() + "/xml_demo/xml_api.get_objeto_3" );

        }
        else  {

            int respuesta = QMessageBox::question( this, "No hay WebService seleccionado",
                                                   "¿Seguro que desea trabajar en modo Off-line?");

            QPalette paletteWebService = this->palette();
            paletteWebService.setColor( QPalette::ButtonText, QColor( 255, 0, 0 ) );

            switch(respuesta)
            {
            case QMessageBox::Yes:
                break;

            case QMessageBox::No:

                ui->cbWebService->setPalette( paletteWebService );
                return;
                break;
            }
        }
    }


    emit signal_configReady( configObject );
}

void ConfigWidget::dimensionarWidgets()
{
    int anchoPantalla = this->width();
    int altoPantalla = this->height();

    int cantidadLineasDeWidgets = 8;
    int cantidadBordesEntreWidgets = cantidadLineasDeWidgets + 1;

    int altoWidget = altoPantalla / 15;
    int borde = ( altoPantalla - cantidadLineasDeWidgets * altoWidget ) / cantidadBordesEntreWidgets;
    int anchoWidgetsCol0 = anchoPantalla / 3;
    int anchoWidgetsCol1 = anchoPantalla - 3 * borde - anchoWidgetsCol0;

    QFont fontBethHand( "BethHand", altoWidget / 1.7f, QFont::Normal );
    QFont fontHelveticaNeue( "Helvetica Neue", altoWidget / 3, QFont::Normal );
//    QFont fontAngelina( "Angelina", 15, QFont::Normal );

    QPalette paletteComboBox = this->palette();
    paletteComboBox.setColor( QPalette::ButtonText, QColor( 42, 25, 122 ) );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );

    this->setFont( fontBethHand );
    this->setPalette( palette );
    ui->leCarpeta->setFont( fontHelveticaNeue );
    ui->cbCamaraIp->setFont( fontHelveticaNeue );
    ui->cbWebService->setFont( fontHelveticaNeue );
    ui->cbFuente->setPalette( paletteComboBox );

    ui->lFuente->setGeometry( QRect( borde, borde, anchoWidgetsCol0, altoWidget ) );
    ui->cbFuente->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, borde, anchoWidgetsCol1, altoWidget ) );

    ui->lCamaraIp->setGeometry( QRect( borde, altoWidget + 2 * borde, anchoWidgetsCol0, altoWidget ) );
    ui->cbCamaraIp->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, altoWidget + 2 * borde, anchoWidgetsCol1, altoWidget ) );

    ui->lCarpeta->setGeometry( QRect( borde, 2 * altoWidget + 3 * borde, anchoWidgetsCol0, altoWidget ) );
    ui->leCarpeta->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, 2 * altoWidget + 3 * borde, anchoWidgetsCol1, altoWidget ) );

    ui->lCamaraUsb->setGeometry( QRect( borde, 3 * altoWidget + 4 * borde, anchoWidgetsCol0, altoWidget ) );
    ui->cbCamaraUsb->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, 3 * altoWidget + 4 * borde, anchoWidgetsCol1, altoWidget ) );

    ui->lWebService->setGeometry( QRect( borde, 4 * altoWidget + 5 * borde, anchoWidgetsCol0, altoWidget ) );
    ui->cbWebService->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, 4 * altoWidget + 5 * borde, anchoWidgetsCol1, altoWidget ) );

    ui->lBaseRedLocal->setGeometry( QRect( borde, 5 * altoWidget + 6 * borde, anchoWidgetsCol0, altoWidget ) );
    ui->cbBaseRedLocal->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, 5 * altoWidget + 6 * borde, anchoWidgetsCol1, altoWidget ) );

    ui->checkOffline->setGeometry( QRect( borde, 6 * altoWidget + 7 * borde, anchoWidgetsCol0, altoWidget ) );

    ui->pbComenzar->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, 7 * altoWidget + 8 * borde, anchoWidgetsCol1, altoWidget ) );
}

void ConfigWidget::cargarCamaras()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    for (int i=0 ; i<cameras.size() ; i++)
        ui->cbCamaraUsb->addItem(cameras.at(i).description());

    if ( cameras.size() > 0 )  {
        ui->cbCamaraUsb->setItemText( 0, "No seleccionado" );
        ui->cbCamaraUsb->setCurrentIndex( 0 );
    }

    if ( cameras.size() <= 0 )  {
        ui->cbCamaraUsb->setItemText( 0, "No se detectan camaras" );
        ui->cbCamaraUsb->setCurrentIndex( 0 );
    }
}

void ConfigWidget::slot_currentIndexChanged( int index )  {
    if ( index == 0 )  {
        ui->cbCamaraIp->setEnabled( true );
        ui->leCarpeta->setEnabled( true );
    }
    else  {
        ui->cbCamaraIp->setEnabled( false );
        ui->leCarpeta->setEnabled( false );
    }
}

void ConfigWidget::slot_sourceIndexChanged( int index )
{
    switch ( index )  {
    case 0:  // No seleccionado
        ui->leCarpeta->setEnabled( false );
        ui->cbCamaraIp->setEnabled( false );
        ui->cbCamaraUsb->setEnabled( false );
        ui->cbWebService->setEnabled( false );
        ui->cbBaseRedLocal->setEnabled( false );
        ui->checkOffline->setEnabled( false );
        ui->pbComenzar->setEnabled( false );

        break;
    case 1:  // IP
        ui->leCarpeta->setEnabled( false );
        ui->cbCamaraIp->setEnabled( true );
        ui->cbCamaraUsb->setEnabled( false );
        ui->cbBaseRedLocal->setEnabled( true );
        ui->checkOffline->setEnabled( true );
        ui->pbComenzar->setEnabled( true );

        if ( ! ui->checkOffline->isChecked() )  {
            ui->cbWebService->setEnabled( true );
        }


        break;
    case 2:  // Disco
        ui->leCarpeta->setEnabled( true );
        ui->cbCamaraIp->setEnabled( false );
        ui->cbCamaraUsb->setEnabled( false );
        ui->cbBaseRedLocal->setEnabled( true );
        ui->checkOffline->setEnabled( true );
        ui->pbComenzar->setEnabled( true );

        if ( ! ui->checkOffline->isChecked() )  {
            ui->cbWebService->setEnabled( true );
        }


        break;
    case 3:  // USB
        ui->leCarpeta->setEnabled( false );
        ui->cbCamaraIp->setEnabled( false );
        ui->cbCamaraUsb->setEnabled( true );
        ui->cbBaseRedLocal->setEnabled( true );
        ui->checkOffline->setEnabled( true );
        ui->pbComenzar->setEnabled( true );

        if ( ! ui->checkOffline->isChecked() )  {
            ui->cbWebService->setEnabled( true );
        }

        break;

    default:;
    }
}

void ConfigWidget::slot_checkBoxChanged( bool activated )
{
    if ( activated )  {
        ui->cbWebService->setEnabled( false );
//        ui->cbBaseRedLocal->setEnabled( false );
    }
    else  {
        ui->cbWebService->setEnabled( true );
        ui->cbBaseRedLocal->setEnabled( true );
    }
}

void ConfigWidget::resizeEvent(QResizeEvent *)
{
    this->dimensionarWidgets();
}

