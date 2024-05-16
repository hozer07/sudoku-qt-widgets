#ifndef BOX_H
#define BOX_H

#include <QStackedWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QStack>

class MainWindow;
class Box : public QStackedWidget
{
    Q_OBJECT
public:
    explicit Box(MainWindow*);
    MainWindow * getMainWindow(void) const {return mainWindow;}
    enum class widgetTypes
    {
        CellType,
        NoteType
    };
    using coordinateType = std::pair<uint8_t,uint8_t>;
    void setHiglighted(void){m_higlighted = true;}
    void resetHiglighted(void){m_higlighted = false;}
    bool isHighlighted(void){return m_higlighted;}

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    MainWindow* mainWindow;
    QStack<coordinateType> notesTaken;
    bool m_higlighted{false};
};

#endif // BOX_H
