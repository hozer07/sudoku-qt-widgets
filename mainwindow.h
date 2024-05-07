#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QList>
#include <utility>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using coordinateType = std::pair<uint8_t, uint8_t>;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isTakingNote(void) const{return m_takingNote;}
    coordinateType getFocusedCellCoordinate(void) const {return focusedCell;}
    void keepCellFocus(coordinateType);

public slots:
    void cleanAndHighlightRowAndColumn(coordinateType);
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

    QList<QList<QStackedWidget*>> boxes;
    coordinateType focusedCell{255,255};
    bool m_takingNote{false};

    void createBoxesGrid(void);
    void createMenuButtons(void);

private slots:
    void takeNoteHandler(void);

};
#endif // MAINWINDOW_H
