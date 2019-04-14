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
};

#endif // MAINWINDOW_H
