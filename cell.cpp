#include "cell.h"

Cell::Cell(uint8_t row, uint8_t column) {
    QString styleOfThisCell{};

    if(0 == row || 3 == row || 6 == row)
    {
        styleOfThisCell.append("border-top: 2px solid red;");
    }
    else if(8 == row)
    {
        styleOfThisCell.append("border-top: 1px solid black;");
        styleOfThisCell.append("border-bottom: 2px solid red;");
    }
    else
    {
        styleOfThisCell.append("border-top: 1px solid black;");
    }

    if(0 == column || 3 == column || 6 == column)
    {
        styleOfThisCell.append("border-left: 2px solid red;");
    }
    else if(8 == column)
    {
        styleOfThisCell.append("border-left: 1px solid black;");
        styleOfThisCell.append("border-right: 2px solid red;");
    }
    else
    {
        styleOfThisCell.append("border-left: 1px solid black;");
    }

    setStyleSheet(styleOfThisCell);
    setMinimumSize(40,40);
}

void Cell::enterEvent(QEnterEvent* event){
    // Change the background color to yellow when the cursor enters the label
    auto styleOfThisCell = this->styleSheet();
    styleOfThisCell.append("background-color: rgb(0, 255, 255)");
    setStyleSheet(styleOfThisCell);
}

void Cell::leaveEvent(QEvent* event){
    // Remove the background color when the cursor leaves the label
    auto styleOfThisCell = this->styleSheet();
    styleOfThisCell.remove("background-color: rgb(0, 255, 255)");
    setStyleSheet(styleOfThisCell);
}
