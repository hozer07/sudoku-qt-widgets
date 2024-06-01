#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QVector>
#include <QSet>
#include <QStack>
#include "logelement.h"
#include <bitset>

class Box;
class QDialog;
class QProcess;
class QGridLayout;
class QVBoxLayout;
class QGroupBox;
class QPushButton;
class QLabel;
class QTimer;

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
    void logEvent(LogElement);
    void setCellResult(coordinateType, bool);

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
    QLabel *timerDisplay;
    QTimer *timer;

    uint64_t timeElapsed{};
    QList<QList<Box*>> boxes;
    coordinateType currentlyFocusedCell{};
    bool m_takingNote{false};

    uint8_t focusedValue{};
    QVector<QSet<Box*>> boxesOfSameValue;
    int m_difficulty_index{};
    QDialog* difficultySetting;
    QProcess* puzzleGenerator;
    QGridLayout* newGameLayout;
    QStack<LogElement> logsToUndo;
    bool isPuzzleComplete(void) const {return correctOrIncorrectBoxes.all();}
    void createBoxesGrid(void);
    void createMenuButtons(void);
    void finishGame(void);
    std::bitset<81> correctOrIncorrectBoxes{};

    friend class UndoProcessor;


private slots:
    void takeNoteHandler(bool);
    void eraseHandler(void);
    void startNewGame(void);
    void generateNewPuzzle(void);
    void setValuesOnPuzzle(void);
    void giveHint(void);
    void undoHandler(void);
    void updateTimer(void);
};
#endif // MAINWINDOW_H
