#include "app.h"
#include <QPainter>
#include <QKeyEvent>


App::App( QWidget *parent ) : Navegador( parent )
{
    this->init( 1, 3 );

    login = new Login( NULL );
    configWidget = new ConfigWidget( NULL );
    principal = new Principal( NULL );

    this->setWidget( login, 0, 0 );
    this->setWidget( configWidget, 0, 1 );
    this->setWidget( principal, 0, 2 );

    this->setWindowTitle( APP_NAME );
    this->setWidgetActual( 0, 0 );

    this->login->setTabFocus( true );
    this->configWidget->setTabFocus( false );
    this->principal->setTabFocus( false );


    connect( login, SIGNAL(successfulSessionStarted( int, QString ) ), this, SLOT( startSession( int, QString ) ) );
    connect( principal, SIGNAL( closeSession() ), this, SLOT( closeSession() ) );
    connect( configWidget, SIGNAL( signal_configReady( ConfigObject * ) ),
             this, SLOT( slot_iniciarProcesamiento( ConfigObject * ) ) );

    imFondo.load( ":/images/fondo_azul.jpg" );
}

App::~App()
{
    Database::getInstance()->disconnect();
    LOG_INF( "App: Destructor" );
}

Login *App::getLogin() const
{
    return login;
}

void App::setLogin(Login *value)
{
    login = value;
}

Principal *App::getPrincipal() const
{
    return principal;
}

void App::setPrincipal(Principal *value)
{
    principal = value;
}

void App::startSession(int sessionId, QString user )
{
    login->setInterface( false );
    login->clearFields();

    principal->setSessionId(sessionId);
    principal->setUser( user );
//    principal->setInterface(true);
//    principal->initCameraAndDetection();

    animacionRightKey( filaWidgetActual, columnaWidgetActual );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->login->setTabFocus( false );
    this->configWidget->setTabFocus( true );
    this->principal->setTabFocus( false );
}

void App::paintEvent(QPaintEvent *)
{
    QPainter painter( this );

    painter.drawImage( 0, 0, imFondo );

}


void App::closeSession()
{
    login->setInterface(true);

    principal->setInterface(false);
    principal->releaseCameraAndDetection();

    int cantidadDesplazos = 2;

    this->resize( this->width() / 2, this->height() );

    animacionLeftKey( filaWidgetActual, columnaWidgetActual, cantidadDesplazos );
    setWidgetActual( filaWidgetActual, columnaWidgetActual - cantidadDesplazos );

    login->setTabFocus( true );
    configWidget->setTabFocus( false );
    principal->setTabFocus( false );
}

void App::slot_iniciarProcesamiento( ConfigObject * configObject )
{
    //    principal->setUser( user );
    principal->setInterface( true );

    this->resize( this->width() * 2, this->height() );

    animacionRightKey( filaWidgetActual, columnaWidgetActual );
    setWidgetActual( filaWidgetActual, columnaWidgetActual + 1 );

    this->login->setTabFocus( false );
    this->configWidget->setTabFocus( false );
    this->principal->setTabFocus( true );

    principal->initCameraAndDetection( configObject );
}

void App::keyPressEvent( QKeyEvent *e )
{
    qDebug() << "keyPressEvent";
    switch ( e->key() )  {
    case Qt::Key_Enter:
    case Qt::Key_Space:
    case Qt::Key_Return:
        break;
    default:;
    }
}

void App::closeEvent(QCloseEvent *)
{
    LOG_INF( "App: closeEvent" );
}


