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
    QString str = QFileDialog::getSaveFileName(nullptr, "Save to", "", "*.txt");

    if(str != "")
    {
        try
        {
            Model::save(main_graph_, str);
        }
        catch(ModelException m_ex)
        {
            QMessageBox msg;
            msg.setInformativeText("#Error: cannot save to this file");
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

void Adapter::set_plots(QVector<QPair<QVector<double>, QVector<double>>> *plots)
{
    for (auto i = 0; i<plots->count(); i++) {
        auto x_list = (*plots)[i].first;
        auto y_list = (*plots)[i].second;
        auto graph = main_graph_->addGraph();
        graph->setData(x_list, y_list);
        graph->setPen(QPen(color_getter_.get_nextColor()));
        main_graph_->replot();
    }
}
