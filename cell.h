#ifndef CELL_H
#define CELL_H

#include <QLabel>
#include <QEvent>

class Cell : public QLabel
{
public:
    explicit Cell(uint8_t row, uint8_t column);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

#endif // CELL_H
