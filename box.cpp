#include "box.h"
#include "mainwindow.h"
#include "cell.h"

Box::Box(MainWindow * mainWindow) : mainWindow(mainWindow) {}

void Box::keyPressEvent(QKeyEvent *event)
{
    auto is_taking_note = mainWindow->isTakingNote();
    auto keyValue = event->key();
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    auto miniCellGroup = qobject_cast<QGroupBox*>(this->widget(static_cast<int>(widgetTypes::NoteType)));
    QGridLayout * miniCellLayout = qobject_cast<QGridLayout*>(miniCellGroup->layout());

    if( keyValue >= Qt::Key_1 && keyValue <= Qt::Key_9)
    {
        keyValue -= Qt::Key_0;
        if(false == is_taking_note)
        {
            this->setCurrentIndex(static_cast<int>(widgetTypes::CellType));
            auto old_value = cell->getValue();
            cell->setValue(keyValue, false);
            while(false == notesTaken.empty())
            {
                auto noteCoord = notesTaken.pop();
                auto miniCell = qobject_cast<Cell*>(miniCellLayout->itemAtPosition(noteCoord.first, noteCoord.second)->widget());
                miniCell->resetValue();
            }
            auto mainWindow = getMainWindow();
            if(old_value != keyValue)
            {
                if(0 != old_value)
                {
                    mainWindow->removeCellFromHighlight(old_value, cell->getCoordinates());
                }
                mainWindow->addCellToHighlight(keyValue, this);
            }
            emit cell->cellFocused(cell->getCoordinates());
        }
        else{
            this->setCurrentIndex(static_cast<int>(widgetTypes::NoteType));
            cell->resetValue();
            auto row = (keyValue - 1) / 3;
            auto column = (keyValue - 1) % 3;
            auto miniCell = qobject_cast<Cell*>(miniCellLayout->itemAtPosition(row, column)->widget());
            miniCell->setValue(keyValue, true);
            notesTaken.push({row,column});
        }
    }
    else if( keyValue >= Qt::Key_Left && keyValue <= Qt::Key_Down)
    {
        auto [row, column] = cell->getCoordinates();
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
        emit next_cell->cellFocused({row,column});
    }
}

void Box::mousePressEvent(QMouseEvent *event)
{
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    emit cell->cellFocused(cell->getCoordinates());
}
