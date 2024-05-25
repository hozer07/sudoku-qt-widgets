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
class QDialog;
class QProcess;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using coordinateType = std::pair<uint8_t, uint8_t>;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isTakingNote(void) const{return m_takingNote;}
    coordinateType getFocusedCellCoordinate(void) const {return currentlyFocusedCell;}
    Box * getBox(const coordinateType coord) const {return boxes[coord.first][coord.second];}
    void removeCellFromHighlight(uint8_t value, const coordinateType coord);
    void addCellToHighlight(uint8_t value, Box*);
    QSet<Box*> getCellsOfSameValue(uint8_t value) const{return boxesOfSameValue[value - 1];}
    void keepCellFocus(coordinateType cellCoordinate);
public slots:
    void cleanAndHighlightBoxes(coordinateType, uint8_t previousValue);
    void setDifficulty(int);
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
    int m_difficulty_index{};
    QDialog* difficultySetting;
    QProcess* puzzleGenerator;


private slots:
    void takeNoteHandler(void);
    void eraseHandler(void);
    void startNewGame(void);
    void generateNewPuzzle(void);
    void setValuesOnPuzzle(void);
};
#endif // MAINWINDOW_H
