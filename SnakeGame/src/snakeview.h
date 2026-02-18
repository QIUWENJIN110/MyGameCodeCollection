// snakeview.h
#ifndef SNAKEVIEW_H
#define SNAKEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QResizeEvent>

#include "snakescene.h"

class SnakeView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit SnakeView(QWidget *parent = nullptr);
    ~SnakeView();

    SnakeScene* getScene() const { return m_scene; }

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    SnakeScene *m_scene;
    void centerView();
};

#endif // SNAKEVIEW_H
