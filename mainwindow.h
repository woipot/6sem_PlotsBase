#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "adapter.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Adapter *adapter_;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_save();
    void on_load();
    void on_center();
    void on_unit_fit();
    void on_fit();
};

#endif // MAINWINDOW_H
