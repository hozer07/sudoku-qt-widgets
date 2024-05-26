#ifndef CELL_H
#define CELL_H

#include <QLabel>

class MainWindow;

class Cell : public QLabel
{
    Q_OBJECT
public:
    using coordinateType = std::pair<uint8_t,uint8_t>;
    explicit Cell(uint8_t row, uint8_t column, bool isNote);
    void setValue(uint8_t value, bool isNote);
    void setTrueValue(uint8_t value){m_trueValue = value;}
    void resetValue(void);
    uint8_t getValue(void) const{return m_cellValue;}
    uint8_t getTrueValue(void)const{return m_trueValue;}
    bool hasValue(void) const{return m_cellMarked;}
private:
    bool isFocused{false};
    bool m_cellMarked{false};
    uint8_t m_cellValue{0};
    uint8_t m_trueValue{0};
    coordinateType m_coordinates;

signals:
    void cellFocused(coordinateType focused_box, uint8_t previous_value);
};

#endif // CELL_H
