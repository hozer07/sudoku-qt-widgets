#include "box.h"
#include "mainwindow.h"
#include "cell.h"
#include <QKeyEvent>
#include <QGroupBox>
#include <QLayout>


static void setupNoteBorders(QGroupBox* miniCellGroup, uint8_t row, uint8_t column);

Box::Box(uint8_t row, uint8_t column, MainWindow * mainWindow) : mainWindow(mainWindow) {
    auto cell = new Cell(row, column, false);
    cell->setMinimumSize(45,45);
    QObject::connect(cell, &Cell::cellFocused, mainWindow, &MainWindow::cleanAndHighlightBoxes);
    this->addWidget(cell);
    auto miniCellGroupForNote = new QGroupBox;
    auto miniCellLayoutForNote = new QGridLayout(miniCellGroupForNote);
    miniCellLayoutForNote->setSpacing(0);
    miniCellLayoutForNote->setContentsMargins(0, 0, 0, 0);
    setupNoteBorders(miniCellGroupForNote, row, column);
    this->addWidget(miniCellGroupForNote);

    for (size_t subRow = 0; subRow < 3; ++subRow) {
        for (size_t subColumn = 0; subColumn < 3; ++subColumn) {
            auto noteCell = new Cell(subRow, subColumn, true);
            noteCell->setFixedSize(15,15);
            noteCell->setAlignment(Qt::AlignCenter);
            miniCellLayoutForNote->addWidget(noteCell, subRow, subColumn);
        }
    }
    miniCellGroupForNote->setLayout(miniCellLayoutForNote);
    m_coordinate = {row, column};
}

void Box::cleanNote(void)
{
    auto noteGrid = qobject_cast<QGroupBox*>(this->widget(static_cast<int>(widgetTypes::NoteType)));
    auto miniCellLayout = qobject_cast<QGridLayout*>(noteGrid->layout());

    if( notesTaken.any() )
    {
        for(uint8_t note = 0; note < 9; note++)
        {
            if( auto bit = notesTaken.test(note); 0 != bit)
            {
                auto noteCoord = std::pair<uint8_t, uint8_t>{note / 3, note % 3};
                auto miniCell = qobject_cast<Cell*>(miniCellLayout->itemAtPosition(noteCoord.first, noteCoord.second)->widget());
                miniCell->resetValue();
                notesTaken.set(note, false);
            }
        }
    }
}

void Box::takeNote(uint8_t keyValue, Cell * cell)
{
    auto miniCellGroup = qobject_cast<QGroupBox*>(this->widget(static_cast<int>(widgetTypes::NoteType)));
    QGridLayout * miniCellLayout = qobject_cast<QGridLayout*>(miniCellGroup->layout());
    this->setCurrentIndex(static_cast<int>(widgetTypes::NoteType));
    cell->resetValue();
    auto row = (keyValue - 1) / 3;
    auto column = (keyValue - 1) % 3;
    auto miniCell = qobject_cast<Cell*>(miniCellLayout->itemAtPosition(row, column)->widget());
    miniCell->setValue(keyValue, true);
    notesTaken.set(keyValue - 1, true);
}

void Box::setBoxValue(uint8_t keyValue, bool isNewGame, bool is_taking_note)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    auto old_value = cell->getValue();

    if(false == is_taking_note)
    {
        this->setCurrentIndex(static_cast<int>(widgetTypes::CellType));
        bool result = cell->setValue(keyValue, false);
        getMainWindow()->setCellResult(getCoordinates(), result);
        cleanNote();
        if(0 != keyValue)
        {
            getMainWindow()->addCellToHighlight(keyValue, this);
        }
    }
    else{
        takeNote(keyValue, cell);
    }

    if(0 != old_value)
    {
        mainWindow->removeCellFromHighlight(old_value, getCoordinates());
    }

    if(false == isNewGame)
    {
        emit cell->cellFocused(getCoordinates(), old_value);
    }
}

void Box::keyPressEvent(QKeyEvent *event)
{
    auto keyValue = event->key();

    if( keyValue >= Qt::Key_1 && keyValue <= Qt::Key_9)
    {
        keyValue -= Qt::Key_0;

        if( mainWindow->isTakingNote() )
        {
            mainWindow->logEvent(TakeNoteLog{getCoordinates(), (uint8_t)keyValue});
        }
        else
        {
            auto oldValue = getCurrentBoxValue();
            mainWindow->logEvent(ChangeValueLog{getCoordinates(), oldValue, notesTaken});
        }

        setBoxValue(keyValue, false, mainWindow->isTakingNote());
    }
    else if( keyValue >= Qt::Key_Left && keyValue <= Qt::Key_Down)
    {
        auto [row, column] = getCoordinates();
        auto old_value = getCurrentBoxValue();
        if( keyValue == Qt::Key_Left && column > 0 )
        {
            column--;
        }
        else if( keyValue == Qt::Key_Right && column < 8 )
        {
            column++;
        }
        else if( keyValue == Qt::Key_Up && row > 0 )
        {
            row--;
        }
        else if( keyValue == Qt::Key_Down && row < 8 )
        {
            row++;
        }
        auto next_box = getMainWindow()->getBox({row,column});
        this->clearFocus();
        next_box->setFocus(Qt::MouseFocusReason);
        auto next_cell = qobject_cast<Cell*>(next_box->widget(static_cast<int>(widgetTypes::CellType)));
        emit next_cell->cellFocused({row,column}, old_value);
    }
}

void Box::mousePressEvent(QMouseEvent *event)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    auto previousValue = mainWindow->getBox(mainWindow->getFocusedCellCoordinate())->getCurrentBoxValue();
    emit cell->cellFocused(this->getCoordinates(), previousValue);
}

static void setupNoteBorders(QGroupBox* miniCellGroup, uint8_t row, uint8_t column)
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

uint8_t Box::getCurrentBoxValue(void)const
{
    auto value = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)))->getValue();
    return value;
}


void Box::cleanBox(void){

    auto box = this;
    if(false == box->isHighlighted())
    {
        return;
    }

    box->resetHiglighted();
    auto cell = qobject_cast<Cell*>(box->widget(static_cast<int>(Box::widgetTypes::CellType)));
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.remove("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);

    auto miniGroup = qobject_cast<QGroupBox*>(box->widget(static_cast<int>(Box::widgetTypes::NoteType)));
    auto styleOfMiniGroup = miniGroup->styleSheet();
    styleOfMiniGroup.remove("background-color: rgb(0, 255, 255)");
    miniGroup->setStyleSheet(styleOfMiniGroup);
}

void Box::highlightBox(void){

    auto box = this;
    if(true == box->isHighlighted())
    {
        return;
    }

    box->setHiglighted();
    auto cell = qobject_cast<Cell*>(box->widget(static_cast<int>(Box::widgetTypes::CellType)));
    auto styleOfThisCell = cell->styleSheet();
    styleOfThisCell.append("background-color: rgb(0, 255, 255)");
    cell->setStyleSheet(styleOfThisCell);

    auto miniGroup = qobject_cast<QGroupBox*>(box->widget(static_cast<int>(Box::widgetTypes::NoteType)));
    auto styleOfMiniGroup = miniGroup->styleSheet();
    styleOfMiniGroup.append("background-color: rgb(0, 255, 255)");
    miniGroup->setStyleSheet(styleOfMiniGroup);
}

void Box::erase(bool isNewGame)
{
    cleanNote();
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    auto old_value = cell->getValue();
    if(0 != old_value)
    {
        mainWindow->removeCellFromHighlight(old_value, this->getCoordinates());
    }
    cell->resetValue();
    if(false == isNewGame)
    {
        emit cell->cellFocused(this->getCoordinates(), old_value);
    }
}

void Box::setBoxTrueValue(uint8_t value)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    cell->setTrueValue(value);
}

void Box::resetBoxValue(void)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    cell->resetValue();
}

uint8_t Box::getBoxTrueValue(void)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    return cell->getTrueValue();
}
