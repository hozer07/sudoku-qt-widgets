#include "cell.h"
#include <QObject>
#include <QString>

Cell::Cell(uint8_t row, uint8_t column, bool isNote) :
    m_coordinates({row,column})
{
    QString styleOfThisCell{};

    if(false == isNote)
    {
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
    }
    else
    {
        styleOfThisCell.append("border: none;");
    }

    setStyleSheet(styleOfThisCell);
    setFocusPolicy(Qt::StrongFocus);
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

void Cell::setValue(uint8_t keyValue, bool isNote)
{
    if(hasValue() && keyValue == value())
    {
        setText(QString{});
    }
    else{
        QFont font;
        font.setPointSize(isNote ? 10 : 25);  // Set font size to 25
        font.setBold(true);
        setFont(font);
        setText(QString::number(keyValue));
        setAlignment(Qt::AlignCenter);
        m_cellValue = keyValue;
    }
    toggleMarked();
}

void Cell::resetValue(void)
{
    setText(QString{});
    m_cellValue = 0;
    toggleMarked();
}
