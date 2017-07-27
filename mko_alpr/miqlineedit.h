#ifndef MIQLINEEDIT_H
#define MIQLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>

class MiQLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MiQLineEdit(QWidget * parent = 0);
    MiQLineEdit(const MiQLineEdit & linea);
    MiQLineEdit& operator=(const MiQLineEdit & linea);

    QString getDirectorio() const;

protected:
    void mousePressEvent(QMouseEvent *);

private:
    // Como solo se muestra el directorio y no la ratu completa, entonces aca almacenamos la ruta completa
    QString directorio;

};

#endif // MIQLINEEDIT_H
