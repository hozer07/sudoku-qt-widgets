#include "cell.h"
#include <QObject>
#include <QString>
#include <QRegularExpression>

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
    styleOfThisCell.append("color : black;");
    setStyleSheet(styleOfThisCell);
    setFocusPolicy(Qt::StrongFocus);
}


bool Cell::setValue(uint8_t keyValue, bool isNote)
{
    bool correctOrIncorrect{};

    if(0 == keyValue || (hasValue() && keyValue == getValue()))
    {
        resetValue();
        correctOrIncorrect = false;
    }
    else{
        m_cellMarked = true;
        QFont font;
        font.setPointSize(isNote ? 10 : 25);  // Set font size to 25
        font.setBold(true);
        setFont(font);
        setText(QString::number(keyValue));
        setAlignment(Qt::AlignCenter);
        m_cellValue = keyValue;

        static QRegularExpression regex("color\\s*:\\s*[^;]+;");
        auto style = styleSheet();
        if( auto trueValue = getTrueValue(); trueValue != keyValue && false == isNote)
        {
            style.replace(regex, "color : red;");
            correctOrIncorrect = false;
        }
        else
        {
            style.replace(regex, "color : black;");
            correctOrIncorrect = true;
        }
        setStyleSheet(style);
    }
    return correctOrIncorrect;
}

void Cell::resetValue(void)
{
    setText(QString{});
    m_cellValue = 0;
    m_cellMarked = false;
}
