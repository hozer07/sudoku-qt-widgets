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
    setFixedSize(sizeHint());
}

void MainWindow::createBoxesGrid(void)
{
    boxesGroup = new QGroupBox(this);

    boxesLayout = new QGridLayout(boxesGroup);
    boxesLayout->setSpacing(0);
    boxesLayout->setContentsMargins(0, 0, 0, 0);

    for (size_t row = 0; row < 9; row++) {
        boxes.append(QList<QStackedWidget*>{});
        for (size_t column = 0; column < 9; column++) {
            auto cell = new Cell(row, column, this);
            QObject::connect(cell, &Cell::cellFocused, this, &MainWindow::cleanAndHighlightRowAndColumn);
            auto tempStackedWidget = new QStackedWidget;
            tempStackedWidget->addWidget(cell);
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

    menuButtonsGroup->setLayout(menuButtonsLayout);
}

MainWindow::~MainWindow() {}

void highlightCell(Cell* cell){
    cell->setIsFocused();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.append("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);
}

void cleanCell(Cell* cell){
    cell->resetIsFocused();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.remove("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);
}


void highlightOrClean( QList<QList<QStackedWidget*>> cell_grid, std::pair<uint8_t, uint8_t> coordinates, std::function<void(Cell*)> func)
{
    auto row_index = coordinates.first;
    auto column_index = coordinates.second;

    auto cell = qobject_cast<Cell*>(cell_grid[row_index][column_index]->widget(0));
    func(cell);

    for(auto& stacked_cell : cell_grid[row_index]){ // highlight or clean all the cell of the focused row
        auto cell = qobject_cast<Cell*>(stacked_cell->widget(0));
        if(cell->getCoordinates() != coordinates)
        {
            func(cell);
        }
    }

    for(auto& row : cell_grid){ // highlight or clean all the cell of the focused column
        auto cell = qobject_cast<Cell*>(row[column_index]->widget(0));
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

void MainWindow::keepCellFocus(coordinateType cellCoordinate)
{
    if( cellCoordinate == coordinateType{255,255} )
        return;

    auto [row, column] = cellCoordinate;

    auto cell = qobject_cast<Cell*>(boxes[row][column]->widget(0));
    cell->setFocus(Qt::MouseFocusReason);
}

void MainWindow::takeNoteHandler(void)
{
    m_takingNote ^= 1;
    keepCellFocus(focusedCell);
}
