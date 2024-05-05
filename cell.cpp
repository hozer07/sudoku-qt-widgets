#include "cell.h"
#include <QObject>

Cell::Cell(uint8_t row, uint8_t column) :
    m_coordinates({row,column })
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
    setMinimumSize(40,40);
}

void Cell::mousePressEvent(QMouseEvent *event){
    emit cellFocused(m_coordinates);
}

void Cell::setFocus()
{
    isFocused = true;
}

void Cell::resetFocus()
{
    isFocused = false;
}

bool Cell::isSelected() const
{
    return isFocused;
}

Cell::coordinateType Cell::getCoordinates(void)
{
    return m_coordinates;
}
