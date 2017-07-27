#include "miqlineedit.h"
#include <QFileDialog>
#include <QDir>

MiQLineEdit::MiQLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

MiQLineEdit::MiQLineEdit(const MiQLineEdit &linea) : QLineEdit()  {
    this->setText(linea.text());
}

MiQLineEdit &MiQLineEdit::operator=(const MiQLineEdit &linea)  {
    this->setText(linea.text());
    return *this;
}

void MiQLineEdit::mousePressEvent(QMouseEvent *)
{
//    this->setText(QFileDialog::getOpenFileName(this, "Elija el recurso", "../", "Multimedia (*.jpg *.png *.mp4)"));

    directorio = QFileDialog::getExistingDirectory( this, "Elija la carpeta", "../",
                                                    QFileDialog::DontResolveSymlinks );

    QDir dir( directorio );
    this->setText( dir.dirName() );
}

QString MiQLineEdit::getDirectorio() const
{
    return directorio;
}


