#include "cell.h"
#include <QObject>
#include <QString>

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
    setFocusPolicy(Qt::StrongFocus);
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

Cell::coordinateType Cell::getCoordinates(void) const
{
    return m_coordinates;
}

void Cell::keyPressEvent(QKeyEvent *event)
{
    auto keyValue = event->key();
    if(keyValue >= Qt::Key_1 || keyValue <= Qt::Key_9)
    {
        QFont font;
        font.setPointSize(20);  // Set font size to 20
        font.setBold(true);
        setFont(font);
        setText(QString::number(keyValue - Qt::Key_0));
        setAlignment(Qt::AlignHCenter);
    }
}
