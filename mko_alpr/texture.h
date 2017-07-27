#ifndef TEXTURE_H
#define TEXTURE_H

#include <QGLWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class Texture : public QObject
{
    Q_OBJECT

public:

    QString name;
    Mat mat;
    GLuint id;

    enum ImageType { RGB, BGR, GRAY };

    Texture( QString name = "", QObject *parent = 0 ) : QObject( parent ), name( name ), id( 0 )
    {
        glGenTextures( 1, &id );
    }

    void generateFromMat( ImageType type = BGR )  {
        glBindTexture( GL_TEXTURE_2D, id );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

        switch(type)  {
        case RGB:
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 1, GL_RGB, GL_UNSIGNED_BYTE, mat.data );
            break;
        case BGR:
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 1, GL_BGR, GL_UNSIGNED_BYTE, mat.data );
            break;
        case GRAY:
//            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 1, GL_BGR, GL_UNSIGNED_BYTE, mat.data );
            break;
        }
    }
};

#endif // TEXTURE_H
