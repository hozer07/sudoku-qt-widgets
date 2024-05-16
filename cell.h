#ifndef CELL_H
#define CELL_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <utility>

class MainWindow;

class Cell : public QLabel
{
    Q_OBJECT
public:
    using coordinateType = std::pair<uint8_t,uint8_t>;
    explicit Cell(uint8_t row, uint8_t column, bool isNote);
    bool hasValue(void) const{return cellMarked;}
    void toggleMarked(void);
    uint8_t getValue(void) const{return m_cellValue;}
    void setValue(uint8_t value, bool isNote);
    void resetValue(void);
    coordinateType getCoordinates(void) const{return m_coordinates;}
private:
    bool isFocused{false};
    bool cellMarked{false};
    uint8_t m_cellValue{0};
    coordinateType m_coordinates;

signals:
    void cellFocused(coordinateType);
};

#endif // CELL_H
