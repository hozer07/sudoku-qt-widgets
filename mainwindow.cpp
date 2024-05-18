#include "mainwindow.h"
#include <functional>
#include "box.h"

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
