#include "adapter.h"
#include "modelexception.h"

Adapter::Adapter()
{
    model_ = new Model();
    x_pred=0;
    y_pred=0;
    drag_graph_ = true;
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
    set_axis_colored(true);
    main_graph_->setSelectionTolerance(10);

    connect(main_graph_, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));

    connect(main_graph_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));

    main_graph_->replot();
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

void Adapter::set_axis_colored(bool is_colored)
{

    auto tick_pen = (is_colored) ? QPen(QColor("blue")) : QPen(QColor("black"));
    auto subtick_pen = (is_colored) ? QPen(QColor("red")) : QPen(QColor("black"));

    main_graph_->xAxis->setTickPen(tick_pen);
    main_graph_->yAxis->setTickPen(tick_pen);

    main_graph_->xAxis->setSubTickPen(subtick_pen);
    main_graph_->yAxis->setSubTickPen(subtick_pen);

    main_graph_->replot();
}

void Adapter::add_point_to_selected(QPoint point)
{
    if (main_graph_->selectedGraphs().size() > 0)
    {
        auto x = main_graph_->xAxis->pixelToCoord(point.x());
        auto y = main_graph_->yAxis->pixelToCoord(point.y());
        main_graph_->selectedGraphs().first()->addData(x, y);

        main_graph_->replot();
    }
}

void Adapter::clear_selected()
{
    if (main_graph_->selectedGraphs().size() > 0)
    {
        main_graph_->deselectAll();
        main_graph_->replot();
    }
}

void Adapter::switch_move(bool is_on)
{
    drag_graph_ = is_on;

    auto plots = get_plots();

    for (auto i = 0; i < plots.count(); i++) {
        plots[i]->setSelectable(is_on? QCP::stWhole : QCP::stSingleData);

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

        graph->setSelectable(drag_graph_? QCP::stWhole : QCP::stSingleData);


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

void Adapter::mousePress(QMouseEvent* mev)
{
    x_pred=main_graph_->xAxis->pixelToCoord(mev->pos().x());
    y_pred=main_graph_->yAxis->pixelToCoord(mev->pos().y());
}

void Adapter::point_move(QMouseEvent *mev)
{
    double  x = main_graph_->xAxis->pixelToCoord(mev->pos().x()),
            y = main_graph_->yAxis->pixelToCoord(mev->pos().y());


    //QCPGraph::pointDistance ( const QPointF &  pixelPoint, QCPGraphDataContainer::const_iterator &  closestData  )
        //QPointF point = mev->pos();
        //QCPGraphDataContainer::const_iterator it;
        //ui->customPlot->graph()->pointDistance(point, it);
            // --- protected

    auto graph = main_graph_->selectedGraphs().first();
    int index = graph->selection().dataRange().begin();

    if(index!=0){
        double min=main_graph_->xAxis->pixelToCoord(
                        main_graph_->xAxis->coordToPixel(
                            (graph->data()->begin() + (index-1))->key)
                        +5);
        if(x<min)x=min;
    }
    if(index != graph->data()->size()-1){
        double max=main_graph_->xAxis->pixelToCoord(
                        main_graph_->xAxis->coordToPixel(
                            (graph->data()->begin() + (index+1))->key)
                        -5);
        if(x>max)x=max;
    }

    (graph->data()->begin()+index)->key=x;
    (graph->data()->begin()+index)->value=y;
        main_graph_->replot();

    //ui->customPlot->graph()->data()[index]->value=y;

}

void Adapter::graph_move(QMouseEvent* mev)
{
    double dx, dy;
    auto graph = main_graph_->selectedGraphs().first();
    dx= - x_pred;
    x_pred=main_graph_->xAxis->pixelToCoord(mev->pos().x());
    dx+=x_pred;

    dy= - y_pred;
    y_pred=main_graph_->yAxis->pixelToCoord(mev->pos().y());
    dy+=y_pred;


    QCPGraphDataContainer::iterator it1=graph->data()->begin(),
            it2=graph->data()->end();
    for(;it1!=it2; ++it1){
        it1->key=it1->key+dx;
        it1->value=it1->value+dy;
    }
    main_graph_->replot();
}


void Adapter::mouseMove(QMouseEvent* mev)
{
    if(! QApplication::mouseButtons()) return;
    if(main_graph_->selectedGraphs().size() != 1) return;

    drag_graph_ ? graph_move(mev) : point_move(mev);

}
