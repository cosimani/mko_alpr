#include "camerawidget.h"

CameraWidget::CameraWidget( QWidget *parent ) : QGLWidget( parent ),
                                                textures( new QVector< Texture * > )
{

}

CameraWidget::~CameraWidget()  {

}

/**
 * @brief CameraWidget::slot_setCameraTexture La clonacion del MAT deberia evitarse para mejorar los tiempos
 * @param value
 */
void CameraWidget::slot_setCameraTexture(cv::Mat * value)
{
    if ( this->isVisible() )  {
        textures->operator []( 0 )->mat = value->clone();
        textures->operator []( 0 )->generateFromMat();  // Webcam entrega en BGR
        this->updateGL();
    }
}

void CameraWidget::initializeGL()  {
    initializeGLFunctions();

    glClearColor( 0, 0, 0, 0 );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );

    GLfloat lightAmbient[4]; lightAmbient[0] = 0.5f;  lightAmbient[1] = 0.5f;  lightAmbient[2] = 0.5f;  lightAmbient[3] = 1.0f;
    GLfloat lightDiffuse[4]; lightDiffuse[0] = 1.0f;  lightDiffuse[1] = 1.0f;  lightDiffuse[2] = 1.0f;  lightDiffuse[3] = 1.0f;
    GLfloat lightPosition[4];lightPosition[0]= 0.0f;  lightPosition[1]= 0.0f;  lightPosition[2]= 2.0f;  lightPosition[3]= 1.0f;

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );  glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION,lightPosition ); glEnable( GL_LIGHT1 );

    textures->append( new Texture( "CameraTexture" ) );
}

void CameraWidget::resizeGL( int width, int height )
{
    float spaceProp = width / (float)height;
    float cameraProp = 4 / (float)3; // replace with camera proportion
    if( spaceProp > cameraProp )
    {
        glViewport( 0, 0, height * cameraProp, height );
    }
    else
    {
        glViewport( 0, 0, width, width / (float)cameraProp);
    }
}

void CameraWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, RESOLUTION_WIDTH, 0, RESOLUTION_HEIGHT, 1, 1000 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Inicio: Gráfico de cámara

    glEnable( GL_TEXTURE_2D );
    glColor3f( 1, 1, 1 );
    glBindTexture( GL_TEXTURE_2D, textures->at( 0 )->id );
    glBegin( GL_QUADS );

//        glTexCoord2f( 1, 0 ); glVertex3f( 0, RESOLUTION_HEIGHT, -999 );
//        glTexCoord2f( 0, 0 ); glVertex3f( RESOLUTION_WIDTH, RESOLUTION_HEIGHT, -999 );
//        glTexCoord2f( 0, 1 ); glVertex3f( RESOLUTION_WIDTH, 0, -999 );
//        glTexCoord2f( 1, 1 ); glVertex3f( 0, 0, -999 );

        glTexCoord2f( 0, 1 ); glVertex3f( 0, 0, -999 );
        glTexCoord2f( 1, 1 ); glVertex3f( RESOLUTION_WIDTH, 0, -999 );
        glTexCoord2f( 1, 0 ); glVertex3f( RESOLUTION_WIDTH, RESOLUTION_HEIGHT, -999 );
        glTexCoord2f( 0, 0 ); glVertex3f( 0, RESOLUTION_HEIGHT, -999 );




    glEnd();
    glDisable( GL_TEXTURE_2D );

    // Fin: Gráfico de cámara

    glFlush();
}
