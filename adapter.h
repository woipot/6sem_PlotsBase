#ifndef ADAPTER_H
#define ADAPTER_H

#include "model.h"
#include "qplot/qcustomplot.h"
#include "colorgetter.h"


class Adapter
{
     Model *model_;

     QCustomPlot *main_graph_;

     ColorGetter color_getter_;

public:
    Adapter();
    ~Adapter();

    void set_main_qgraph(QCustomPlot *main_graph);

    void on_save();
    void on_load();

    void on_center();
    void on_unit_fit();
    void on_fit();

private:
    void set_plots(QVector<QPair<QVector<double>, QVector<double>>> *plots);

    QVector<QCPGraph*> get_plots();
};

#endif // ADAPTER_H
