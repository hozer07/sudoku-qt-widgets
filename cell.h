#ifndef CELL_H
#define CELL_H

#include <QLabel>
#include <QEvent>

class Cell : public QLabel
{
public:
    Cell();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

#endif // CELL_H
