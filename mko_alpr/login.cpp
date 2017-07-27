#include "login.h"
#include "ui_login.h"

#include <QPainter>
#include <QDesktopWidget>
#include <QRect>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect( ui->userLineEdit, SIGNAL( returnPressed() ), SLOT( slot_focusPassword() ) );
    connect( ui->passwordLineEdit, SIGNAL( returnPressed() ), SLOT( slot_login() ) );
    connect( ui->pbIngresar, SIGNAL( pressed() ), SLOT( slot_login() ) );
}

void Login::setInterface(bool enable)
{
    ui->userLineEdit->setEnabled(enable);
    ui->passwordLineEdit->setEnabled(enable);
}

void Login::clearFields()
{
    ui->userLineEdit->clear();
    ui->passwordLineEdit->clear();
}

void Login::setTabFocus( bool focus )
{
    if ( focus )  {
        ui->welcomeLabel->setFocusPolicy( Qt::NoFocus );
        ui->userLineEdit->setFocusPolicy( Qt::StrongFocus );
        ui->passwordLineEdit->setFocusPolicy( Qt::StrongFocus );
        ui->pbIngresar->setFocusPolicy( Qt::NoFocus );
        ui->userLineEdit->setFocus();
    }
    else  {
        ui->welcomeLabel->setFocusPolicy( Qt::NoFocus );
        ui->userLineEdit->setFocusPolicy( Qt::NoFocus );
        ui->passwordLineEdit->setFocusPolicy( Qt::NoFocus );
        ui->pbIngresar->setFocusPolicy( Qt::NoFocus );

    }
}

void Login::slot_focusPassword()
{
    ui->passwordLineEdit->setFocus();
}

void Login::slot_login()
{
    QString user = ui->userLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    bool validate = user != "" && password != "";

    if( ! validate )
    {
        ui->userLineEdit->setFocus();
        return;
    }

    int adminId = Database::getInstance()->login(user, password);
    switch(adminId)
    {
    case -1:
        ui->userLineEdit->setFocus();
        ui->userLineEdit->selectAll();
        QMessageBox::critical(
                    this,
                    "Error",
                    "Error al ejecutar la consulta a la base de datos");
        break;

    case 0:
        ui->userLineEdit->setFocus();
        ui->userLineEdit->selectAll();
        QMessageBox::critical(
                    this,
                    "Error",
                    "Nombre de usuario o contraseña inválidos");
        break;

    default:

        if(adminId > 0)
        {
            int sessionId = Database::getInstance()->startSession(adminId);

            if(sessionId == -1)
            {
                ui->userLineEdit->setFocus();
                ui->userLineEdit->selectAll();
                QMessageBox::critical(
                            this,
                            "Error",
                            "Error al ejecutar la consulta a la base de datos");
                break;
            }

            emit successfulSessionStarted(sessionId, user);
        }
        break;
    }
}

Login::~Login()
{
    delete ui;
}

void Login::dimensionarWidgets()
{    
    int anchoPantalla = this->width();
    int altoPantalla = this->height();

    int cantidadLineasDeWidgets = 8;
    int cantidadBordesEntreWidgets = cantidadLineasDeWidgets + 1;

    int altoWidget = altoPantalla / 15;
    int borde = ( altoPantalla - cantidadLineasDeWidgets * altoWidget ) / cantidadBordesEntreWidgets;
    int anchoWidgetsCol0 = anchoPantalla / 3;
    int anchoWidgetsCol1 = anchoPantalla - 3 * borde - anchoWidgetsCol0;

    QFont fontBethHand( "BethHand", altoWidget / 1.7f, QFont::Normal );
//    QFont fontAngelina( "Angelina", 15, QFont::Normal );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );

    this->setFont( fontBethHand );
    this->setPalette( palette );

//    ui->welcomeLabel->setGeometry( QRect( borde, borde, anchoWidgetsCol0, altoWidget ) );
//    ui->userLineEdit->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, borde, anchoWidgetsCol1, altoWidget ) );

//    ui->passwordLineEdit->setGeometry( QRect( borde, altoWidget + 2 * borde, anchoWidgetsCol0, altoWidget ) );
//    ui->pbIngresar->setGeometry( QRect( 2 * borde + anchoWidgetsCol0, altoWidget + 2 * borde, anchoWidgetsCol1, altoWidget ) );


//    QRect rec = QApplication::desktop()->screenGeometry();
//    int anchoScreen = rec.width();
//    int altoScreen = rec.height();

//    this->resize( anchoScreen / 3, altoScreen / 3 );

//    QFontMetrics fm( QFont( "BethHand" ) );
//    int altoFuente = fm.height();

//    QFont fontBethHand( "BethHand", 25, QFont::Normal );
//    ui->userLineEdit->setFont( fontBethHand );
//    ui->passwordLineEdit->setFont( fontBethHand );


//    QFont fontAngelina( "Angelina", 35, QFont::Normal );
////    QFont font( "Angelina" );

//    ui->welcomeLabel->setFont( fontAngelina );
//    ui->pbIngresar->setFont( fontAngelina );

//    QPalette palette = this->palette();
//    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );

//    ui->welcomeLabel->setPalette(palette);
}

void Login::showEvent(QShowEvent *)
{
    this->dimensionarWidgets();
}

void Login::resizeEvent(QResizeEvent *)
{
    this->dimensionarWidgets();
}


void Login::hideEvent(QHideEvent *)
{
    //    this->setFocusPolicy(Qt::NoFocus);
}

void Login::closeEvent(QCloseEvent *)
{
    LOG_INF( "Login: closeEvent" );
}
