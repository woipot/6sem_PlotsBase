#ifndef MODEL_H
#define MODEL_H

#include "qplot/qcustomplot.h"
#include "modelexception.h"

#include <QString>
#include <QRegExp>


class Model
{
public:
    Model();

    static void save(QCustomPlot *plot, QString path);

    static QVector<QPair<QVector<double>, QVector<double>>> load(QString path) noexcept(false);

private:
    static QPair<QVector<double>, QVector<double>> parse_graph(QString str) noexcept(false);
};

#endif // MODEL_H
