// snakeview.cpp

#include "snakeview.h"

SnakeView::SnakeView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new SnakeScene(this))
{
    // Set view properties
    setScene(m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setStyleSheet("background-color: aliceblue;");

    // Set fixed size
    setFixedSize(GRID_WIDTH * CELL_SIZE + 4, GRID_HEIGHT * CELL_SIZE + 4);

    // Center display
    centerView();
}

SnakeView::~SnakeView()
{

}

void SnakeView::wheelEvent(QWheelEvent *event)
{
    // Disable wheel zooming
    event->ignore();
}

void SnakeView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    centerView();
}

void SnakeView::centerView()
{
    // Ensure scene content is centered
    centerOn(m_scene->width() / 2, m_scene->height() / 2);
}
