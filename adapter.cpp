#include "adapter.h"
#include "modelexception.h"

Adapter::Adapter()
{
    model_ = new Model();
}

Adapter::~Adapter()
{
    delete model_;
}

void Adapter::set_main_qgraph(QCustomPlot *main_graph)
{
    main_graph_ = main_graph;

    main_graph_->xAxis->setLabel("x");
    main_graph_->yAxis->setLabel("y");
    main_graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    main_graph_->legend->setVisible(true);
    main_graph_->legend->setBrush(QBrush(QColor(255,255,255,230)));
}

void Adapter::on_load()
{
    QString str = QFileDialog::getOpenFileName(nullptr, "Load from", "", "*.txt");

    if(str != "")
    {
        try
        {
            auto plots = Model::load(str);
            main_graph_->clearGraphs();
            set_plots(&plots);
        }
        catch(ModelException m_ex)
        {
            QMessageBox msg;
            msg.setInformativeText("#Error: cannot load from this file -> " + m_ex.get_msg());
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
        catch(...)
        {
            QMessageBox msg;
            msg.setInformativeText("#Error: unknown error");
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    }
}

void Adapter::on_save()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save to", "", "*.png");

    if(str != "")
    {
        try
        {
            Model::save(main_graph_ ,str);
        }
        catch(...)
        {
            QMessageBox msg;
            msg.setInformativeText("#Error: unknown error");
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    }
}


void Adapter::on_center()
{
    auto x_cuted = main_graph_->xAxis->range().size() / 2;
    auto y_cuted = main_graph_->yAxis->range().size() / 2;

    main_graph_->xAxis->setRange(-x_cuted, x_cuted);
    main_graph_->yAxis->setRange(-y_cuted, y_cuted);
    main_graph_->replot();
}

void Adapter::on_unit_fit()
{
    main_graph_->xAxis->setRange(-1, 1);
    main_graph_->yAxis->setRange(-1, 1);
    main_graph_->replot();
}

void Adapter::on_fit()
{
    main_graph_->yAxis->rescale(true);
    main_graph_->xAxis->rescale(true);
    main_graph_->replot();
}

void Adapter::remove_all_graphs()
{
    main_graph_->clearGraphs();
    main_graph_->replot();
}

void Adapter::remove_selected_graph()
{
    if (main_graph_->selectedGraphs().size() > 0)
    {
      main_graph_->removeGraph(main_graph_->selectedGraphs().first());
      main_graph_->replot();
    }
}

void Adapter::change_line_type()
{
    if (main_graph_->selectedGraphs().size() > 0)
    {
      int style = main_graph_->selectedGraphs().first()->lineStyle();

      (style == 5)? style = 0 : style++;

      main_graph_->selectedGraphs().first()->setLineStyle((QCPGraph::LineStyle)style);

      main_graph_->replot();
    }

}



void Adapter::set_plots(QVector<QPair<QVector<double>, QVector<double>>> *plots)
{
    for (auto i = 0; i<plots->count(); i++) {
        auto x_list = (*plots)[i].first;
        auto y_list = (*plots)[i].second;
        auto graph = main_graph_->addGraph();
        graph->setData(x_list, y_list);
        graph->setPen(QPen(color_getter_.get_nextColor()));
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
        main_graph_->replot();
    }
}

QVector<QCPGraph*> Adapter::get_plots()
{
    QVector<QCPGraph*> res;

    for (auto i = 0; i< main_graph_->graphCount(); i++) {
        res.append(main_graph_->graph(i));
    }

    return res;
}
