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
#include <QVector>
#include <QSet>

class Box;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using coordinateType = std::pair<uint8_t, uint8_t>;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isTakingNote(void) const{return m_takingNote;}
    coordinateType getFocusedCellCoordinate(void) const {return currentlyFocusedCell;}
    void keepCellFocus(coordinateType);
    Box * getBox(const coordinateType coord) const {return boxes[coord.first][coord.second];}
    void removeCellFromHighlight(uint8_t value, const coordinateType coord);
    void addCellToHighlight(uint8_t value, Box*);
    QSet<Box*> getCellsOfSameValue(uint8_t value) const{return boxesOfSameValue[value - 1];}
public slots:
    void cleanAndHighlightBoxes(coordinateType, uint8_t previousValue);
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

    QList<QList<Box*>> boxes;
    coordinateType currentlyFocusedCell{};
    bool m_takingNote{false};

    void createBoxesGrid(void);
    void createMenuButtons(void);
    uint8_t focusedValue{};
    QVector<QSet<Box*>> boxesOfSameValue;

private slots:
    void takeNoteHandler(void);
    void eraseHandler(void);

};
#endif // MAINWINDOW_H
