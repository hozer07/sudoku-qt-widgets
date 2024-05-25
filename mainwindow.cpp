#include "mainwindow.h"
#include <functional>
#include "box.h"
#include <QComboBox>
#include <QDialog>
#include <QProcess>
#include <QStringList>
#include <cstdlib>
#include <QDir>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createBoxesGrid();
    createMenuButtons();
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(boxesGroup);
    mainLayout->addWidget(menuButtonsGroup);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setCentralWidget(centralWidget);
    setFixedSize(sizeHint());
    for(size_t num = 0; num < 9; num++)
    {
        boxesOfSameValue.append(QSet<Box*>{});
    }
}

void MainWindow::createBoxesGrid(void)
{
    boxesGroup = new QGroupBox(this);
    boxesLayout = new QGridLayout(boxesGroup);
    boxesLayout->setSpacing(0);
    boxesLayout->setContentsMargins(0, 0, 0, 0);

    for (size_t row = 0; row < 9; row++) {
        boxes.append(QList<Box*>{});
        for (size_t column = 0; column < 9; column++) {
            auto tempStackedWidget = new Box(row, column, this);
            boxes[row].append(tempStackedWidget);
            boxesLayout->addWidget(tempStackedWidget, row, column);
        }
    }
    boxesGroup->setLayout(boxesLayout);
}

void MainWindow::createMenuButtons(void)
{
    menuButtonsGroup = new QGroupBox(this);
    menuButtonsLayout = new QVBoxLayout;

    newGameButton = new QPushButton("New Game");
    hintButton = new QPushButton("Hint");
    undoButton = new QPushButton("Undo");
    eraseButton = new QPushButton("Erase");
    takeNoteButton = new QPushButton("Note");

    menuButtonsLayout->addWidget(newGameButton);
    menuButtonsLayout->addWidget(hintButton);
    menuButtonsLayout->addWidget(undoButton);
    menuButtonsLayout->addWidget(eraseButton);
    menuButtonsLayout->addWidget(takeNoteButton);
    menuButtonsGroup->setFixedWidth(180);

    newGameButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    hintButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    undoButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    eraseButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    takeNoteButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    takeNoteButton->setCheckable(true);
    connect(takeNoteButton, &QPushButton::toggled, this, &MainWindow::takeNoteHandler);
    connect(eraseButton, &QPushButton::pressed, this, &MainWindow::eraseHandler);
    connect(newGameButton, &QPushButton::pressed, this, &MainWindow::startNewGame);
    connect(hintButton, &QPushButton::pressed, this, &MainWindow::giveHint);

    menuButtonsGroup->setLayout(menuButtonsLayout);
}

MainWindow::~MainWindow() {}

void highlightOrClean( QList<QList<Box*>> cell_grid, std::pair<uint8_t, uint8_t> coordinates, std::function<void(Box*)> func, MainWindow const * mainWindow, uint8_t cellValue)
{
    auto row_index = coordinates.first;
    auto column_index = coordinates.second;

    for(auto& box : cell_grid[row_index]){ // highlight or clean all the cells of the focused row
        func(box);
    }

    for(auto& row : cell_grid){ // highlight or clean all the cell of the focused column
        func(row[column_index]);
    }

    if(0 != cellValue)
    {
        auto cells_to_highlight = mainWindow->getCellsOfSameValue(cellValue);
        std::for_each(cells_to_highlight.begin(), cells_to_highlight.end(), func);
    }
}

void MainWindow::cleanAndHighlightBoxes(std::pair<uint8_t, uint8_t> newFocusCell, uint8_t previousValue)
{
    highlightOrClean(boxes, currentlyFocusedCell, &Box::cleanBox, this, previousValue);
    highlightOrClean(boxes, newFocusCell, &Box::highlightBox, this, getBox(newFocusCell)->getCurrentBoxValue());
    currentlyFocusedCell = newFocusCell;
}

void MainWindow::keepCellFocus(coordinateType cellCoordinate)
{
    auto [row, column] = cellCoordinate;
    auto box = boxes[row][column];
    box->setFocus(Qt::MouseFocusReason);
}

void MainWindow::takeNoteHandler(void)
{
    m_takingNote ^= 1;
    keepCellFocus(currentlyFocusedCell);
}

void MainWindow::removeCellFromHighlight(uint8_t value, const coordinateType coord)
{
    boxesOfSameValue[value - 1].removeIf([coord](Box const * box){
        return (box->getCoordinates() == coord);
    });
}

void MainWindow::addCellToHighlight(uint8_t value, Box * box)
{
    boxesOfSameValue[value - 1].insert(box);
}

void MainWindow::eraseHandler(void)
{
    auto box = getBox(currentlyFocusedCell);
    box->erase();
}

void MainWindow::startNewGame(void)
{
    QDialog newGameSetting{this};
    difficultySetting = &newGameSetting;
    newGameSetting.setWindowTitle("Difficulty");
    newGameSetting.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout layout;
    QLabel label{"Choose Difficulty:"};
    QComboBox levels{};
    levels.addItem("Easy");
    levels.addItem("Medium");
    levels.addItem("Hard");
    levels.addItem("Insane");
    connect(&levels, &QComboBox::currentIndexChanged, this, &MainWindow::setDifficulty);

    layout.addWidget(&label, 0, 0);
    layout.addWidget(&levels, 0, 1);

    QPushButton okButton{"Start Game"}, cancelButton{"Cancel"};
    connect(&okButton, &QPushButton::pressed, this, &MainWindow::generateNewPuzzle);
    connect(&cancelButton, &QPushButton::pressed, &newGameSetting, &QDialog::close);
    layout.addWidget(&okButton, 1, 0);
    layout.addWidget(&cancelButton, 1, 1);

    newGameSetting.setLayout(&layout);

    newGameSetting.exec();
}

void MainWindow::setDifficulty(int difficulty)
{
    m_difficulty_index = difficulty;
}

void MainWindow::setValuesOnPuzzle(void)
{
    using namespace std;
    auto stdout = puzzleGenerator->readAllStandardOutput();
    auto marker = '\n';
    auto answerKeyStart = next(std::find(begin(stdout), end(stdout), marker));
    auto puzzleStart = next(std::find(next(answerKeyStart), end(stdout), marker));
    vector<int> answers, puzzleValues;

    copy_if(answerKeyStart, puzzleStart, back_inserter(answers), [](char c){
        return (c >= '0' && c <= '9');
    });

    copy_if(puzzleStart, end(stdout), back_inserter(puzzleValues), [](char c){
        return (c >= '0' && c <= '9');
    });

    for (size_t i = 0; i < 81; i++)
    {
        auto row = i / 9;
        auto column = i % 9;
        auto box = getBox({row, column});
        box->mousePressEvent({});
        box->setBoxTrueValue(answers[i] - '0');
        if('0' != puzzleValues[i])
        {
            box->setBoxValue(puzzleValues[i] - '0');
        }
    }
    getBox({3, 3})->mousePressEvent({});
    keepCellFocus({3, 3});
}

void MainWindow::generateNewPuzzle(void)
{
    auto puzzleGeneratorProcess = new QProcess{this};
    puzzleGenerator = puzzleGeneratorProcess;
    const QString difficulties[] = {"Easy", "Medium", "Hard", "Insane"};
    // Get the path to this source file
    QString sourceFilePath = __FILE__;
    QFileInfo fileInfo(sourceFilePath);
    QString scriptPath = fileInfo.absolutePath() + "/puzzle_generator.py";
    const QString command = "python3";
    QStringList arguments{};
    arguments << scriptPath << difficulties[m_difficulty_index];

    connect(puzzleGeneratorProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::setValuesOnPuzzle);
    connect(puzzleGeneratorProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), difficultySetting, &QDialog::close);
    puzzleGeneratorProcess->start(command, arguments);
}

void MainWindow::giveHint(void)
{
    auto focusedBox = getBox(currentlyFocusedCell);
    const auto trueValue = focusedBox->getBoxTrueValue();
    const auto currentValue = focusedBox->getCurrentBoxValue();
    if( trueValue != currentValue )
    {
        focusedBox->setBoxValue(trueValue);
    }
}
