#ifndef CELL_H
#define CELL_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <utility>

class Cell : public QLabel
{
    Q_OBJECT
public:
    using coordinateType = std::pair<uint8_t,uint8_t>;
    explicit Cell(uint8_t row, uint8_t column);
    void setFocus(void);
    void resetFocus(void);
    bool isSelected() const;
    coordinateType getCoordinates(void) const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    bool isFocused;
    coordinateType m_coordinates;

signals:
    void cellFocused(coordinateType);
};

#endif // CELL_H
