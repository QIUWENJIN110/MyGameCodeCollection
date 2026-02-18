// gamewindow.h
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>

#include "snakeview.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void updateScoreDisplay();
    void updateStatusDisplay();

private:
    SnakeView *m_gameView;
    QLabel *m_scoreLabel;
    QLabel *m_statusLabel;
    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_restartButton;
    
    QTimer *m_updateTimer;
    
    void setupUI();
    void setupConnections();
    void updateButtons();
};

#endif // GAMEWINDOW_H
