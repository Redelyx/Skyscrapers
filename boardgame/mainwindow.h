#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "boardgame.h"
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void new_game();
    void auto_hint();
    void open();
    void save();
    void exit();

private:
    BoardGame* game_ = nullptr;
    QTextEdit* text_edit = new QTextEdit;
    QPushButton* open_button = new QPushButton{tr("&Open")};
    QPushButton* save_button = new QPushButton{tr("&Save")};
    QPushButton* exit_button = new QPushButton{tr("E&xit")};
};

#endif // MAINWINDOW_H
