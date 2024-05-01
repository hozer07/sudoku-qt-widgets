#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include "cell.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QGroupBox *boxesGroup;
    QGridLayout *boxesLayout;

    QGroupBox *menuButtonsGroup;
    QVBoxLayout *menuButtonsLayout;

    QPushButton *newGameButton;
    QPushButton *hintButton;
    QPushButton *undoButton;
    QPushButton *eraseButton;
    QPushButton *takeNoteButton;

    QList<QList<Cell*>> boxes;

    void createBoxesGrid(void);
    void createMenuButtons(void);
};
#endif // MAINWINDOW_H
