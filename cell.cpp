#include "cell.h"
#include <QObject>
#include <QString>
#include "mainwindow.h"

Cell::Cell(uint8_t row, uint8_t column, MainWindow * mainWindow) :
    m_coordinates({row,column }),
    mainWindow(mainWindow)
{
    QString styleOfThisCell{};

    if(0 == row || 3 == row || 6 == row)
    {
        styleOfThisCell.append("border-top: 4px solid red;");
    }
    else if(8 == row)
    {
        styleOfThisCell.append("border-top: 1px solid black;");
        styleOfThisCell.append("border-bottom: 4px solid red;");
    }
    else
    {
        styleOfThisCell.append("border-top: 1px solid black;");
    }

    if(0 == column || 3 == column || 6 == column)
    {
        styleOfThisCell.append("border-left: 4px solid red;");
    }
    else if(8 == column)
    {
        styleOfThisCell.append("border-left: 1px solid black;");
        styleOfThisCell.append("border-right: 4px solid red;");
    }
    else
    {
        styleOfThisCell.append("border-left: 1px solid black;");
    }

    setStyleSheet(styleOfThisCell);
    setMinimumSize(45,45);
    setFocusPolicy(Qt::StrongFocus);
}

void Cell::mousePressEvent(QMouseEvent *event){
    emit cellFocused(m_coordinates);
}

void Cell::setIsFocused()
{
    isFocused = true;
}

void Cell::resetIsFocused()
{
    isFocused = false;
}

void Cell::toggleMarked()
{
    cellMarked ^= 1;
}

void setCellValue(Cell* cell, uint8_t keyValue)
{
    if(cell->hasValue() && keyValue == cell->value())
    {
        cell->setText(QString{});
    }
    else{
        QFont font;
        font.setPointSize(25);  // Set font size to 25
        font.setBold(true);
        cell->setFont(font);
        cell->setText(QString::number(keyValue - Qt::Key_0));
        cell->setAlignment(Qt::AlignHCenter);
        cell->setValue(keyValue);
    }

    cell->toggleMarked();
}

void takeNoteOnCell(Cell* cell, uint8_t keyValue)
{

}

void Cell::keyPressEvent(QKeyEvent *event)
{
    auto keyValue = event->key();
    if( keyValue >= Qt::Key_1 && keyValue <= Qt::Key_9)
    {
        if(false == mainWindow->isTakingNote())
        {
            setCellValue(this, keyValue);
        }
        else{
            takeNoteOnCell(this, keyValue);
        }
    }
}
