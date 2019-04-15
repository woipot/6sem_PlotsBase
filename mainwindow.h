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

    void context_menu_request(QPoint pos);
    void move_legend();

    void remove_all_graphs();
    void remove_selected_graph();

    void change_line_type();

    void colored(bool is_on);

    void clear_select();
    void on_mouse_click();


};

#endif // MAINWINDOW_H
