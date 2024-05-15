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

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    MainWindow* mainWindow;
    QStack<coordinateType> notesTaken;
};

#endif // BOX_H
