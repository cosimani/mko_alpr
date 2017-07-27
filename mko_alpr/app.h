#ifndef APP_H
#define APP_H

#define APP_NAME "AR plates detection"

#include <QObject>
#include <QImage>

#include "navegador.h"
#include "login.h"
#include "principal.h"
#include "configwidget.h"

class App : public Navegador
{
    Q_OBJECT

private:

    Login *login;
    Principal *principal;
    ConfigWidget *configWidget;

    QImage imFondo;

protected:
    void paintEvent( QPaintEvent * );

    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *);
public:

    explicit App( QWidget *parent = NULL );
    virtual ~App();

    Login *getLogin() const;
    void setLogin(Login *value);

    Principal *getPrincipal() const;
    void setPrincipal(Principal *value);

private slots:

    void startSession(int sessionId, QString user);
    void closeSession();
    void slot_iniciarProcesamiento(ConfigObject *configObject);
};

#endif // APP_H
