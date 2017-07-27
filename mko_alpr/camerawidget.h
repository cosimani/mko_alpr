#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#define RESOLUTION_WIDTH  640
#define RESOLUTION_HEIGHT 480

#include <QGLWidget>
#include <QGLFunctions>

#include "texture.h"

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class CameraWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:
    CameraWidget( QWidget *parent = 0 );
    virtual ~CameraWidget();

private:
    QVector< Texture * > *textures;

protected:
    void initializeGL();
    void resizeGL( int width, int height );
    void paintGL();

public slots:
    void slot_setCameraTexture( cv::Mat *value );

};


#endif // CAMERAWIDGET_H
