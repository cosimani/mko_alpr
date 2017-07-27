#ifndef NAVEGADOR_H
#define NAVEGADOR_H

#define ANIMATION_DURATION 2000
#define ANIMATION_EASING_CURVE QEasingCurve::OutExpo

#include <QWidget>
#include <QVector>
#include <QRect>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QPropertyAnimation>

class Navegador : public QWidget
{
    Q_OBJECT

protected:
    QVector< QVector< QWidget * > * > *widgets; // Vector de vectores de punteros a Widget

    int filaWidgetActual;       // Fila del widget actual
    int columnaWidgetActual;    // Columna del widget actual

    void init( int filas, int columnas );                   // Auxiliar del constructor para crear los objetos
    void setPositions( int filaActual, int columnaActual);  // Setea las posiciones de los widgets

    int getFilas();     // Devuelve cantidad de filas del navegador
    int getColumnas();  // Devuelve cantidad de columnas del navegador

    void animacionUpKey( int filaWidgetDesplazado, int columnaWidgetDesplazado );       // Desplaza widgets hacia abajo
    void animacionDownKey( int filaWidgetDesplazado, int columnaWidgetDesplazado );     // Desplaza widgets hacia arriba
    void animacionLeftKey(int filaWidgetDesplazado, int columnaWidgetDesplazado , int cantidadDesplazos = 1 );     // Desplaza widgets hacia derecha
    void animacionRightKey( int filaWidgetDesplazado, int columnaWidgetDesplazado );    // Desplaza widgets hacia izquierda

public:
    explicit Navegador( QWidget *parent = NULL );                           // Crea un navegador con 1 widget
    explicit Navegador( int filas, int columnas, QWidget *parent = NULL );  // Crea un navegador de tamano personalizado
    virtual ~Navegador();

    void setWidgetActual( int fila, int columna );              // Setea el widget central
    void setWidget( QWidget *widget, int fila, int columna );   // Setea un widget en una posicion

protected:
    void resizeEvent( QResizeEvent * );     // Acomoda los widgets al redimensionar la ventana

    void closeEvent( QCloseEvent * );
};

#endif // NAVEGADOR_H
