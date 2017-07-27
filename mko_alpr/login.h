#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>

#include "database.h"

namespace Ui
{
    class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:

    explicit Login(QWidget *parent = NULL);
    void setInterface(bool enable);
    void clearFields();
    void setTabFocus( bool focus );
    ~Login();

private:

    Ui::Login *ui;

    void dimensionarWidgets();



protected:
    void showEvent( QShowEvent * );
    void hideEvent( QHideEvent * );
    void closeEvent( QCloseEvent * );

    void resizeEvent(QResizeEvent *);

private slots:

    void slot_focusPassword();
    void slot_login();

signals:

    void successfulSessionStarted(int adminId, QString user);
};

#endif // LOGIN_H
