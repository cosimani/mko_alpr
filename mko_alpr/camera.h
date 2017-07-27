#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QTimer>
#include <QFileInfoList>
#include "folderobject.h"

#include <vector>

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Camera : public QObject
{
    Q_OBJECT

public:

    enum TipoCaptura  {  IPCAM, USBCAM, FOLDER, VIDEO  };

    explicit Camera( QObject *parent = NULL );
    VideoCapture *videoCapture;
    QTimer *timer;

    void initCamaraUSB( int indexCamera );
    void initFolder( QString directorio );
    void initCamaraIP(QString url );
    void release();
    virtual ~Camera();

    QTimer *getTimer() const;

    void procesarVideo( QString archivo );

private:

    Mat *frame;
    TipoCaptura tipoCaptura;

    FolderObject * folderObject;


private slots:

    void process();

public slots:


signals:

    void plateDetected( QString plate );
    void newCameraFrame( cv::Mat * frame );

};


#endif // CAMERA_H
