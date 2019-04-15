#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    adapter_ = new Adapter();

    adapter_->set_main_qgraph(ui->plotWidget);
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_save()));
    QObject::connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(on_load()));

    QObject::connect(ui->action_to_center, SIGNAL(triggered()), this, SLOT(on_center()));
    QObject::connect(ui->action_unit_scale, SIGNAL(triggered()), this, SLOT(on_unit_fit()));
    QObject::connect(ui->action_fit, SIGNAL(triggered()), this, SLOT(on_fit()));
    QObject::connect(ui->actionswitch_axis_colored, SIGNAL(triggered(bool)), this, SLOT(colored(bool)));

    //QObject::connect(ui->plotWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_mouse_click(QMouseEvent*)));

    ui->plotWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->plotWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(context_menu_request(QPoint)));
}

MainWindow::~MainWindow()
{
    delete adapter_;
    delete ui;
}

void MainWindow::context_menu_request(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->plotWidget->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(move_legend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(move_legend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(move_legend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(move_legend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(move_legend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    if (ui->plotWidget->selectedGraphs().size() > 0)
    {
        auto action = menu->addAction("Add Point", this, SLOT(on_mouse_click()));
        QVariant v = qVariantFromValue(pos);
        action->setData(v);

s        menu->addAction("Change line type of selected", this, SLOT(change_line_type()));
        menu->addAction("Clear select", this, SLOT(clear_select()));
        menu->addAction("Remove selected graph", this, SLOT(remove_selected_graph()));
    }
    if (ui->plotWidget->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(remove_all_graphs()));
  }

  menu->popup(ui->plotWidget->mapToGlobal(pos));
}

void MainWindow::move_legend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->plotWidget->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->plotWidget->replot();
    }
  }
}

void MainWindow::remove_selected_graph()
{
    adapter_->remove_selected_graph();
}

void MainWindow::change_line_type()
{
    adapter_->change_line_type();
}

void MainWindow::colored(bool is_on)
{
    adapter_->set_axis_colored(is_on);
}

void MainWindow::clear_select()
{
    adapter_->clear_selected();
}

void MainWindow::on_mouse_click()
{
    QAction *act = qobject_cast<QAction *>(sender());
    QVariant v = act->data();
    auto pos = v.value<QPoint>();
    adapter_->add_point_to_selected(pos);
}

void MainWindow::remove_all_graphs()
{
  adapter_->remove_all_graphs();
}

void MainWindow::on_save()
{
    adapter_->on_save();
}

void MainWindow::on_load()
{
    adapter_->on_load();
}


void MainWindow::on_center()
{
    adapter_->on_center();
}

void MainWindow::on_unit_fit()
{
    adapter_->on_unit_fit();
}

void MainWindow::on_fit()
{
    adapter_->on_fit();
}
