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
