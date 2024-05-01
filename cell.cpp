#include "cell.h"

Cell::Cell() {

}

void Cell::enterEvent(QEnterEvent* event){
    // Change the background color to yellow when the cursor enters the label
    setStyleSheet("background-color: yellow");
}

void Cell::leaveEvent(QEvent* event){
    // Remove the background color when the cursor leaves the label
    setStyleSheet("");
}
