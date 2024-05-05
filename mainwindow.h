#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <utility>

class Cell;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void cleanAndHighlightRowAndColumn(std::pair<uint8_t, uint8_t>);
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
    std::pair<uint8_t, uint8_t> focusedCell{255,255};

};
#endif // MAINWINDOW_H
