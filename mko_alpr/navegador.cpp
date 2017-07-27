#include "navegador.h"
#include <QDebug>
#include "logger.h"

Navegador::Navegador( QWidget *parent ) :
    QWidget(parent)
{
    init( 1, 1 );
}

Navegador::Navegador( int filas, int columnas, QWidget *parent ) :
    QWidget(parent)
{
    if( filas <= 0 ) filas = 1;
    if( columnas <= 0 ) columnas = 1;
    init( filas, columnas );
}

Navegador::~Navegador()
{
    LOG_INF( "Navegador: destructor" );
}

void Navegador::init( int filas, int columnas )
{
    // Creamos un widget en cada sector del navegador

    widgets = new QVector< QVector< QWidget * > * >( filas );
    for( int i = 0; i < filas; i++ )
    {
        widgets->operator []( i ) = new QVector< QWidget * >( columnas );
        for( int j = 0; j < columnas; j++ )
        {
            widgets->operator []( i )->operator []( j ) = new QWidget( this );
        }
    }

    setWidgetActual( 0, 0 );
}

void Navegador::setWidgetActual( int fila, int columna )
{
    // Actualiza las variables del widget actual

    if( fila < 0 ) fila = widgets->size() - 1;
    if( fila >= widgets->size() ) fila = 0;
    if( columna < 0 ) columna = widgets->operator []( fila )->size() - 1;
    if( columna >= widgets->operator []( fila )->size() ) columna = 0;

    this->filaWidgetActual = fila;
    this->columnaWidgetActual = columna;

    // Lo posiciona en pantalla y acomoda a los demas

    setPositions( filaWidgetActual, columnaWidgetActual );
}

void Navegador::setPositions( int filaActual, int columnaActual )
{
    // Posiciona al widget que se encuentre en filaActual - columnaActual al frente

    for( int i = 0; i < widgets->size(); i++ )
    {
        for( int j = 0; j < widgets->operator []( i )->size(); j++ )
        {
            QWidget *widgetActual = widgets->operator []( i )->operator []( j );
            widgetActual->resize( this->width(), this->height() );

            int xWidgetActual = i * this->width() - filaActual * this->width();
            int yWidgetActual = j * this->height() - columnaActual * this->height();

            widgetActual->setGeometry( xWidgetActual, yWidgetActual, widgetActual->width(), widgetActual->height() );
        }
    }
}

void Navegador::animacionUpKey(int filaWidgetDesplazado, int columnaWidgetDesplazado)
{
    // Genera la animacion para el widget desplazado y el widget superior

    QWidget *widgetDesplazado = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *desplazarAnimation = new QPropertyAnimation( widgetDesplazado, "pos" );
    desplazarAnimation->setDuration( ANIMATION_DURATION );
    desplazarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    desplazarAnimation->setStartValue( QPoint( 0, 0 ) );
    desplazarAnimation->setEndValue( QPoint( 0, widgetDesplazado->height() ) );
    desplazarAnimation->start();

    QWidget *widgetEntrante;
    if( filaWidgetDesplazado > 0 )
        widgetEntrante = widgets->operator []( filaWidgetDesplazado - 1 )->operator []( columnaWidgetDesplazado );
    else
        widgetEntrante = widgets->operator []( this->getFilas() - 1 )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *ingresarAnimation = new QPropertyAnimation( widgetEntrante, "pos" );
    ingresarAnimation->setDuration( ANIMATION_DURATION );
    ingresarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    ingresarAnimation->setStartValue( QPoint( 0, - widgetEntrante->height() ) );
    ingresarAnimation->setEndValue( QPoint( 0, 0 ));
    ingresarAnimation->start();
}

void Navegador::animacionDownKey(int filaWidgetDesplazado, int columnaWidgetDesplazado)
{
    // Genera la animacion para el widget desplazado y el widget inferior

    QWidget *widgetDesplazado = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *desplazarAnimation = new QPropertyAnimation( widgetDesplazado, "pos" );
    desplazarAnimation->setDuration( ANIMATION_DURATION );
    desplazarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    desplazarAnimation->setStartValue( QPoint( 0, 0 ) );
    desplazarAnimation->setEndValue( QPoint( 0, - widgetDesplazado->height() ) );
    desplazarAnimation->start();

    QWidget *widgetEntrante;
    if( filaWidgetDesplazado < this->getFilas() - 1 )
        widgetEntrante = widgets->operator []( filaWidgetDesplazado + 1 )->operator []( columnaWidgetDesplazado );
    else
        widgetEntrante = widgets->operator []( 0 )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *ingresarAnimation = new QPropertyAnimation( widgetEntrante, "pos" );
    ingresarAnimation->setDuration( ANIMATION_DURATION );
    ingresarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    ingresarAnimation->setStartValue( QPoint( 0, widgetDesplazado->height() ) );
    ingresarAnimation->setEndValue( QPoint( 0, 0 ));
    ingresarAnimation->start();
}

void Navegador::animacionLeftKey(int filaWidgetDesplazado, int columnaWidgetDesplazado, int cantidadDesplazos )
{
    // Genera la animacion para el widget desplazado y el widget izquierdo

    QWidget *widgetDesplazado = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *desplazarAnimation = new QPropertyAnimation( widgetDesplazado, "pos" );
    desplazarAnimation->setDuration( ANIMATION_DURATION );
    desplazarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    desplazarAnimation->setStartValue( QPoint( 0, 0 ) );
    desplazarAnimation->setEndValue( QPoint( widgetDesplazado->width(), 0 ) );
    desplazarAnimation->start();

    QWidget *widgetEntrante;
    if( columnaWidgetDesplazado > 0 )  {
//        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado - 1 );
        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->
                operator []( columnaWidgetDesplazado - cantidadDesplazos );
    }
    else  {
//        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->operator []( this->getColumnas() - 1 );
        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->
                operator []( this->getColumnas() - cantidadDesplazos );
    }

    QPropertyAnimation *ingresarAnimation = new QPropertyAnimation( widgetEntrante, "pos" );
    ingresarAnimation->setDuration( ANIMATION_DURATION );
    ingresarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    ingresarAnimation->setStartValue( QPoint( - widgetEntrante->width(), 0 ) );
    ingresarAnimation->setEndValue( QPoint( 0, 0 ));
    ingresarAnimation->start();
}

void Navegador::animacionRightKey(int filaWidgetDesplazado, int columnaWidgetDesplazado)
{
    // Genera la animacion para el widget desplazado y el widget derecho

    QWidget *widgetDesplazado = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado );

    QPropertyAnimation *desplazarAnimation = new QPropertyAnimation( widgetDesplazado, "pos" );
    desplazarAnimation->setDuration( ANIMATION_DURATION );
    desplazarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    desplazarAnimation->setStartValue( QPoint( 0, 0 ) );
    desplazarAnimation->setEndValue( QPoint( - widgetDesplazado->width(), 0 ) );
    desplazarAnimation->start();

    QWidget *widgetEntrante;
    if( columnaWidgetDesplazado < this->getColumnas() - 1 )
        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->operator []( columnaWidgetDesplazado + 1 );
    else
        widgetEntrante = widgets->operator []( filaWidgetDesplazado )->operator []( 0 );

    QPropertyAnimation *ingresarAnimation = new QPropertyAnimation( widgetEntrante, "pos" );
    ingresarAnimation->setDuration( ANIMATION_DURATION );
    ingresarAnimation->setEasingCurve( ANIMATION_EASING_CURVE );
    ingresarAnimation->setStartValue( QPoint( widgetDesplazado->width(), 0 ) );
    ingresarAnimation->setEndValue( QPoint( 0, 0 ));
    ingresarAnimation->start();
}

void Navegador::setWidget( QWidget *widget, int fila, int columna )
{
    // Ocupa un lugar especificado con un widget

    if( ! widget ) return;
    widget->setParent( this );
    widgets->operator []( fila )->operator []( columna ) = widget;
}

int Navegador::getFilas()
{
    // Retorna cantidad de filas de la matriz

    return widgets->size();
}

int Navegador::getColumnas()
{
    // Retorna cantidad de columnas de la matriz

    if( getFilas() > 0 )
    {
        return widgets->operator []( 0 )->size();
    }

    return 0;
}

void Navegador::resizeEvent( QResizeEvent * )
{
    // Acomoda la geometria de los widgets si se redimensiona la ventana

    setPositions( filaWidgetActual, columnaWidgetActual );
}

void Navegador::closeEvent(QCloseEvent *)
{
    LOG_INF( "Navegador: closeEvent" );
}
