#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T14:55:10
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core gui opengl sql multimedia network

TARGET = mko_alpr
TEMPLATE = app

# Esto para que no muestre demasiado debug por consola
DEFINES += SIN_DEBUG

# Esto para

contains( DEFINES, SIN_DEBUG )  {

    message( "Definir SIN_DEBUG para evitar todo mensaje por consola, ya sea con qDebug o cout" )
    message( "Para deshabilitar debug de ALPR, ir al archivo de configuración openalpr.defaults.conf" )

}

#DEFINES += DEBUG_OSI

contains( DEFINES, DEBUG_OSI )  {

    message( Esta compilacion tendra herramientas para depuracion de Cesar )
    message( Para evitarlo comentar la linea donde se define DEBUG_OSI )


    INCLUDEPATH += /home/cosimani/Proyecto/2017/patentes/PatentesMKO/qt/openalpr/src

}

# cesar
unix:DIR_OPENCV_LIBS = /usr/local/lib

# emi
#unix:DIR_OPENCV_LIBS = /usr/lib/x86_64-linux-gnu

#nano
#unix:DIR_OPENCV_LIBS = /usr/lib/x86_64-linux-gnu


unix:INCLUDEPATH += "/usr/include/GL/"
unix:LIBS += "/usr/lib/x86_64-linux-gnu/libglut.so"

# Open CV

unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_core.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_highgui.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_imgproc.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_objdetect.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_calib3d.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ml.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_video.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_features2d.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_flann.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ml.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_photo.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_stitching.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_superres.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_video.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_videostab.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_videoio.so
unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_imgcodecs.so

# Por ahora no usadas

# unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_contrib.so
# unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_gpu.so
# unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_legacy.so
# unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ocl.so
# unix:LIBS += $$DIR_OPENCV_LIBS/libopencv_ts.so


# cesar
unix:DIR_ALPR_LIBS = /usr/local/lib

# emi
#unix:DIR_ALPR_LIBS = /usr/lib

# nano
#unix:DIR_ALPR_LIBS = /usr/lib

unix:LIBS += $$DIR_ALPR_LIBS/libopenalpr.so
#unix:LIBS += $$DIR_ALPR_LIBS/libopenalprc.so
#unix:LIBS += $$DIR_ALPR_LIBS/libopenalprgo.so
#unix:LIBS += $$DIR_ALPR_LIBS/libopenalprjni.so
#unix:LIBS += $$DIR_ALPR_LIBS/libopenalprpy.so

SOURCES += \
    main.cpp\
    principal.cpp \
    config.cpp \
    logger.cpp \
    camera.cpp \
    camerawidget.cpp \
    database.cpp \
    login.cpp \
    navegador.cpp \
    app.cpp \
    util.cpp \
    configwidget.cpp \
    miqlineedit.cpp \
    configobject.cpp \
    folderobject.cpp \
    restclient.cpp

HEADERS += \
    principal.h \
    config.h \
    logger.h \
    camera.h \
    camerawidget.h \
    texture.h \
    database.h \
    login.h \
    navegador.h \
    app.h \
    util.h \
    configwidget.h \
    miqlineedit.h \
    configobject.h \
    folderobject.h \
    restclient.h

FORMS += \
    principal.ui \
    login.ui \
    configwidget.ui

DISTFILES += \
    ../config/mko_alpr.config \
    ../config/openalpr.defaults.conf \
    ../runtime_data/cameras.yaml \
    ../runtime_data/config/ar.conf \
    ../runtime_data/config/ms.conf \
    ../runtime_data/postprocess/ar.patterns \
    ../runtime_data/postprocess/ms.patterns \
    ../runtime_data/keypoints/ar/ar2010.jpg \
    ../runtime_data/keypoints/ms/ar2016.jpg \
    ../runtime_data/ocr/lar.traineddata \
    ../runtime_data/ocr/tessdata/lar.traineddata \
    ../runtime_data/ocr/tessdata/lms.traineddata \
    ../runtime_data/config/ar.conf \
    ../runtime_data/config/ms.conf \
    ../runtime_data/region/ar.xml \
    ../runtime_data/region/ms.xml

RESOURCES += \
    images.qrc \
    fuentes.qrc
