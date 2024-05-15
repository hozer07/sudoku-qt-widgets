#include "mainwindow.h"
#include <functional>
#include "cell.h"
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
}

void setupNoteBorders(QGroupBox* miniCellGroup, uint8_t row, uint8_t column)
{
    auto styleOfMiniCellGroup = miniCellGroup->styleSheet();
    if(0 == row || 3 == row || 6 == row)
    {
        styleOfMiniCellGroup.append("border-top: 4px solid red;");
    }
    else if(8 == row)
    {
        styleOfMiniCellGroup.append("border-top: 1px solid black;");
        styleOfMiniCellGroup.append("border-bottom: 4px solid red;");
    }
    else
    {
        styleOfMiniCellGroup.append("border-top: 1px solid black;");
    }

    if(0 == column || 3 == column || 6 == column)
    {
        styleOfMiniCellGroup.append("border-left: 4px solid red;");
    }
    else if(8 == column)
    {
        styleOfMiniCellGroup.append("border-left: 1px solid black;");
        styleOfMiniCellGroup.append("border-right: 4px solid red;");
    }
    else
    {
        styleOfMiniCellGroup.append("border-left: 1px solid black;");
    }
    miniCellGroup->setStyleSheet(styleOfMiniCellGroup);
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
            auto cell = new Cell(row, column, false);
            cell->setMinimumSize(45,45);
            QObject::connect(cell, &Cell::cellFocused, this, &MainWindow::cleanAndHighlightRowAndColumn);
            auto tempStackedWidget = new Box(this);
            tempStackedWidget->addWidget(cell);
            boxes[row].append(tempStackedWidget);
            boxesLayout->addWidget(tempStackedWidget, row, column);
            auto miniCellGroupForNote = new QGroupBox;
            auto miniCellLayoutForNote = new QGridLayout(miniCellGroupForNote);
            miniCellLayoutForNote->setSpacing(0);
            miniCellLayoutForNote->setContentsMargins(0, 0, 0, 0);
            tempStackedWidget->addWidget(miniCellGroupForNote);
            setupNoteBorders(miniCellGroupForNote, row, column);
            for (size_t subRow = 0; subRow < 3; ++subRow) {
                for (size_t subColumn = 0; subColumn < 3; ++subColumn) {
                    auto noteCell = new Cell(subRow, subColumn, true);
                    noteCell->setFixedSize(15,15);
                    noteCell->setAlignment(Qt::AlignCenter);
                    miniCellLayoutForNote->addWidget(noteCell, subRow, subColumn);
                }
            }
            miniCellGroupForNote->setLayout(miniCellLayoutForNote);
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

void highlightBox(Box* box){
    auto cell = qobject_cast<Cell*>(box->widget(static_cast<int>(Box::widgetTypes::CellType)));
    cell->setIsFocused();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.append("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);

    auto miniGroup = qobject_cast<QGroupBox*>(box->widget(static_cast<int>(Box::widgetTypes::NoteType)));
    auto styleOfMiniGroup = miniGroup->styleSheet();
    styleOfMiniGroup.append("background-color: rgb(0, 255, 255)");
    miniGroup->setStyleSheet(styleOfMiniGroup);
}

void cleanBox(Box* box){
    auto cell = qobject_cast<Cell*>(box->widget(static_cast<int>(Box::widgetTypes::CellType)));
    cell->resetIsFocused();
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.remove("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);

    auto miniGroup = qobject_cast<QGroupBox*>(box->widget(static_cast<int>(Box::widgetTypes::NoteType)));
    auto styleOfMiniGroup = miniGroup->styleSheet();
    styleOfMiniGroup.remove("background-color: rgb(0, 255, 255)");
    miniGroup->setStyleSheet(styleOfMiniGroup);
}


void highlightOrClean( QList<QList<Box*>> cell_grid, std::pair<uint8_t, uint8_t> coordinates, std::function<void(Box*)> func)
{
    auto row_index = coordinates.first;
    auto column_index = coordinates.second;

    auto box = cell_grid[row_index][column_index];
    func(box);

    for(auto& box : cell_grid[row_index]){ // highlight or clean all the cells of the focused row
        auto cell = qobject_cast<Cell*>(box->widget(0));
        if(cell->getCoordinates() != coordinates)
        {
            func(box);
        }
    }

    for(auto& row : cell_grid){ // highlight or clean all the cell of the focused column
        auto cell = qobject_cast<Cell*>(row[column_index]->widget(0));
        if(cell->getCoordinates() != coordinates)
        {
            func(row[column_index]);
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
        highlightOrClean(boxes, focusedCell, &cleanBox);
    }

    highlightOrClean(boxes, coordinates, &highlightBox);
    focusedCell = coordinates;
}

void MainWindow::keepCellFocus(coordinateType cellCoordinate)
{
    if( cellCoordinate == coordinateType{255,255} )
        return;

    auto [row, column] = cellCoordinate;

    auto cell = qobject_cast<Box*>(boxes[row][column]);
    cell->setFocus(Qt::MouseFocusReason);
}

void MainWindow::takeNoteHandler(void)
{
    m_takingNote ^= 1;
    keepCellFocus(focusedCell);
}
