#include "model.h"

Model::Model()
{

}

void Model::save(QCustomPlot *plot, QString path)
{
    plot->savePng(path);
}

QVector<QPair<QVector<double>, QVector<double> > > Model::load(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        throw ModelException(file.errorString());
    }

    QTextStream in(&file);

    QVector<QPair<QVector<double>, QVector<double>>> result_vector;
    QString all = in.readAll();
    QStringList fields = all.split(":");
    for (int i = 1;i < fields.count(); i++) {
        QString simplified = fields[i].simplified();

        try
        {
            QPair<QVector<double>, QVector<double>> graph = parse_graph(simplified);
            result_vector.append(graph);
        }catch(...)
        {
            throw ModelException("erro of parse [" + simplified +"]");
        }
    }

    file.close();

    return result_vector;
}


QPair<QVector<double>, QVector<double> > Model::parse_graph(QString str)
{
    QVector<double> x_list, y_list;

    QStringList points_list = str.split(' ');

    for (auto i = 0; i < points_list.count(); i++) {
        auto points = points_list[i].split(',');

        bool parse_result = false;
        auto x = points[0].toDouble(&parse_result);

        if(!parse_result)
            throw ModelException(points[0] + " is not double");

        auto y = points[1].toDouble(&parse_result);

        if(!parse_result)
            throw ModelException(points[1] + " is not double");

        x_list.append(x);
        y_list.append(y);

    }
    return QPair<QVector<double>, QVector<double> >(x_list, y_list);
}
