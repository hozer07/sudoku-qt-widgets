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
            cell->setValue(keyValue, false);
            while(false == notesTaken.empty())
            {
                auto noteCoord = notesTaken.pop();
                auto miniCell = qobject_cast<Cell*>(miniCellLayout->itemAtPosition(noteCoord.first, noteCoord.second)->widget());
                miniCell->setText(QString{});
            }
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
}

void Box::mousePressEvent(QMouseEvent *event)
{
    this->setCurrentIndex(static_cast<int>(widgetTypes::CellType));
    auto cell = qobject_cast<Cell*>(this->widget(static_cast<int>(widgetTypes::CellType)));
    emit cell->cellFocused(cell->getCoordinates());
}
