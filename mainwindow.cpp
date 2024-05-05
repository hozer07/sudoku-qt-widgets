#include "mainwindow.h"
#include <functional>
#include "cell.h"

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
    this->setFixedSize(this->sizeHint());
}

void MainWindow::createBoxesGrid(void)
{
    boxesGroup = new QGroupBox(this);

    boxesLayout = new QGridLayout(boxesGroup);
    boxesLayout->setSpacing(0);
    boxesLayout->setContentsMargins(0, 0, 0, 0);

    for (size_t row = 0; row < 9; row++) {
        boxes.append(QList<Cell*>{});
        for (size_t column = 0; column < 9; column++) {
            auto cell = new Cell(row, column);
            QObject::connect(cell, &Cell::cellFocused, this, &MainWindow::cleanAndHighlightRowAndColumn);
            boxes[row].append(cell);
            boxesLayout->addWidget(cell, row, column);
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

    menuButtonsGroup->setLayout(menuButtonsLayout);
}

MainWindow::~MainWindow() {}

void highlightCell(Cell* cell){
    cell->setFocus();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.append("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);
}

void cleanCell(Cell* cell){
    cell->resetFocus();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.remove("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);
}


void highlightOrClean( QList<QList<Cell*>> cell_grid, std::pair<uint8_t, uint8_t> coordinates, std::function<void(Cell*)> func)
{
    auto row_index = coordinates.first;
    auto column_index = coordinates.second;

    func(cell_grid[row_index][column_index]);

    for(auto& cell : cell_grid[row_index]){ // highlight or clean all the cell of the focused row
        if(cell->getCoordinates() != coordinates)
        {
            func(cell);
        }
    }

    for(auto& row : cell_grid){ // highlight or clean all the cell of the focused column
        auto cell = row[column_index];
        if(cell->getCoordinates() != coordinates)
        {
            func(cell);
        }
    }
}

void MainWindow::cleanAndHighlightRowAndColumn(std::pair<uint8_t, uint8_t> coordinates){

    using namespace std;
    if(focusedCell == coordinates)
    {
        return;
    }

    // Clean previously highlighted row and column
    if( focusedCell != std::pair<uint8_t,uint8_t>{255,255} ) // init state
    {
        highlightOrClean(boxes, focusedCell, &cleanCell);
    }

    highlightOrClean(boxes, coordinates, &highlightCell);
    focusedCell = coordinates;
}


