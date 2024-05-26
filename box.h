#ifndef BOX_H
#define BOX_H

#include <QStackedWidget>
#include <QStack>

class MainWindow;
class QMouseEvent;
class QKeyEvent;

class Box : public QStackedWidget
{
    Q_OBJECT
public:
    explicit Box(uint8_t row, uint8_t column, MainWindow*);
    MainWindow * getMainWindow(void) const {return mainWindow;}
    enum class widgetTypes
    {
        CellType,
        NoteType
    };
    using coordinateType = std::pair<uint8_t,uint8_t>;
    void setHiglighted(void){m_higlighted = true;}
    void resetHiglighted(void){m_higlighted = false;}
    void setBoxValue(uint8_t);
    void setBoxTrueValue(uint8_t);
    void resetBoxValue(void);
    void cleanBox(void);
    void highlightBox(void);
    void erase(void);
    bool isHighlighted(void)const {return m_higlighted;}
    void mousePressEvent(QMouseEvent *event) override;
    coordinateType getCoordinates(void)const {return m_coordinate;}
    uint8_t getCurrentBoxValue(void)const;
    uint8_t getBoxTrueValue(void);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    MainWindow* mainWindow;
    QStack<coordinateType> notesTaken;
    bool m_higlighted{false};
    coordinateType m_coordinate{};
    void cleanNote(void);
};

#endif // BOX_H
