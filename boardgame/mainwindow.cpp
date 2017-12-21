#include "mainwindow.h"
#include "boardgamegui.h"
#include "skyscrapers.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVariant>
#include <fstream>

MainWindow::MainWindow()
{
    auto menu = menuBar()->addMenu(tr("Game"));
    auto game = menu->addAction(tr("New game"));
    menu->addSeparator();
    auto hint = menu->addAction(tr("&Hint"));
    menu->addSeparator();
    QAction* open = menu->addAction(tr("&Open"));
    QAction* save = menu->addAction(tr("&Save"));
    menu->addSeparator();
    QAction* exit = menu->addAction(tr("E&xit"));

    setWindowTitle(tr("Skyscrapers"));

    connect(game, &QAction::triggered, this, &MainWindow::new_game);
    connect(hint, &QAction::triggered, this, &MainWindow::auto_hint);
    connect(open, &QAction::triggered, this, &MainWindow::open);
    connect(save, &QAction::triggered, this, &MainWindow::save);
    connect(exit, &QAction::triggered, this, &MainWindow::exit);
    new_game();
}

void MainWindow::new_game()
{
    if (game_ != nullptr) delete game_;
    if (centralWidget() != nullptr) delete centralWidget();

    auto filename_ = QFileDialog::getOpenFileName(this).toStdString();
    game_ = new Skyscrapers{filename_};
    setCentralWidget(new BoardGameGui{game_});

    // fix appearance
    adjustSize();
    setFixedSize(sizeHint());
}

void MainWindow::auto_hint() {
    auto msg = game_->message_1().c_str();
    QMessageBox::information(this, tr("!!!"), tr(msg));
    return game_->auto_hint();}

void MainWindow::open() {
    // choose the input file
    auto filename = QFileDialog::getOpenFileName(this).toStdString();
    if (filename != "") {
        if (game_ != nullptr) delete game_;
        if (centralWidget() != nullptr) delete centralWidget();
        game_ = new Skyscrapers{filename};
        setCentralWidget(new BoardGameGui{game_});

        // fix appearance
        adjustSize();
        setFixedSize(sizeHint());
    }else {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));

    }
}

void MainWindow::save() {
    // choose the output file
    auto filename = QFileDialog::getSaveFileName(this).toStdString();
    if (filename != "") {
           game_->print_matrix(filename);
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Could not save file"));
    }
}

void MainWindow::exit() {
    auto button = QMessageBox::question(
        this,
        tr("Quit"),
        tr("Do you really want to quit?"),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes) {
        window()->close();
    }
}

