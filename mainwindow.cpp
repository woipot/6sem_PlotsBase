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

    QObject::connect(ui->actionTo_center, SIGNAL(triggered()), this, SLOT(on_center()));
    QObject::connect(ui->actionUnit_scale, SIGNAL(triggered()), this, SLOT(on_unit_fit()));
    QObject::connect(ui->actionFit, SIGNAL(triggered()), this, SLOT(on_fit()));
}

MainWindow::~MainWindow()
{
    delete adapter_;
    delete ui;
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
