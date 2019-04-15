#ifndef ADAPTER_H
#define ADAPTER_H

#include "model.h"
#include "qplot/qcustomplot.h"
#include "colorgetter.h"
#include <QMenu>
#include <QObject>


class Adapter: public QObject
{
    Q_OBJECT

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

    void remove_all_graphs();
    void remove_selected_graph();

    void change_line_type();

    void set_axis_colored(bool is_colored);

    void add_point_to_selected(QPoint point);

    void clear_selected();


public slots:
    void mouseMove(QMouseEvent *mev);


private:
    void set_plots(QVector<QPair<QVector<double>, QVector<double>>> *plots);

    QVector<QCPGraph*> get_plots();
};

#endif // ADAPTER_H
