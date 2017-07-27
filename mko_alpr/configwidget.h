#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <QHostAddress>
#include <QResizeEvent>
#include "configobject.h"

namespace Ui {
class ConfigWidget;
}

class ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWidget(QWidget *parent = 0);
    ~ConfigWidget();
    void setTabFocus( bool focus );

private:
    Ui::ConfigWidget *ui;

    void dimensionarWidgets();

    void cargarCamaras();
    ConfigObject * configObject;

protected:
    void showEvent( QShowEvent * );
    void hideEvent( QHideEvent * );
//    void resizeEvent( QResizeEvent * e );

    void resizeEvent(QResizeEvent *);

private slots:
    void slot_comenzar();

    void slot_currentIndexChanged(int index);
    void slot_sourceIndexChanged(int index);
    void slot_checkBoxChanged( bool activated );

signals:
    void signal_configReady( ConfigObject * configObject );

};

#endif // CONFIGWIDGET_H
