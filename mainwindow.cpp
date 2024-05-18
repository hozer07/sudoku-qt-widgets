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
        boxesToHighlight.append(QSet<Box*>{});
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

    menuButtonsGroup->setLayout(menuButtonsLayout);
}

MainWindow::~MainWindow() {}

void highlightOrClean( QList<QList<Box*>> cell_grid, std::pair<uint8_t, uint8_t> coordinates, std::function<void(Box*)> func, MainWindow * mainWindow)
{
    auto row_index = coordinates.first;
    auto column_index = coordinates.second;

    for(auto& box : cell_grid[row_index]){ // highlight or clean all the cells of the focused row
        func(box);
    }

    for(auto& row : cell_grid){ // highlight or clean all the cell of the focused column
        func(row[column_index]);
    }

    auto box = cell_grid[row_index][column_index];
    func(box);
    auto value_of_clicked_cell = box->getCurrentBoxValue();

    if(0 != value_of_clicked_cell)
    {
        auto cells_to_highlight = mainWindow->getCellsOfSameValue(value_of_clicked_cell);
        std::for_each(cells_to_highlight.begin(), cells_to_highlight.end(), func);
    }
}

void MainWindow::cleanAndHighlightBoxes(std::pair<uint8_t, uint8_t> coordinates){

    using namespace std;
    if(focusedCell == coordinates)
    {
        auto cell_value = getBox(coordinates)->getCurrentBoxValue();
        if( 0 != cell_value)
        {
            auto cells_to_highlight = getCellsOfSameValue(cell_value);
            std::for_each(cells_to_highlight.begin(), cells_to_highlight.end(), std::function<void(Box*)>{&Box::highlightBox});
        }
        return;
    }

    // Clean previously highlighted row and column
    if( focusedCell != std::pair<uint8_t,uint8_t>{255,255} ) // init state
    {
        highlightOrClean(boxes, focusedCell, &Box::cleanBox, this);
    }

    highlightOrClean(boxes, coordinates, &Box::highlightBox, this);
    focusedCell = coordinates;
}

void MainWindow::keepCellFocus(coordinateType cellCoordinate)
{
    if( cellCoordinate == coordinateType{255,255} )
        return;

    auto [row, column] = cellCoordinate;
    auto box = boxes[row][column];
    box->setFocus(Qt::MouseFocusReason);
}

void MainWindow::takeNoteHandler(void)
{
    m_takingNote ^= 1;
    keepCellFocus(focusedCell);
}

void MainWindow::removeCellFromHighlight(uint8_t value, const coordinateType coord)
{
    boxesToHighlight[value - 1].removeIf([coord](Box const * box){
        return (box->getCoordinates() == coord);
    });
}

void MainWindow::addCellToHighlight(uint8_t value, Box * box)
{
    boxesToHighlight[value - 1].insert(box);
}
