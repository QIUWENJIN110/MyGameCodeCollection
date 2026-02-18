// gamewindow.cpp

#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent),
      m_gameView(new SnakeView(this)),
      m_scoreLabel(new QLabel("Score: 0", this)),
      m_statusLabel(new QLabel("Status: Game Over", this)),
      m_startButton(new QPushButton("Start", this)),
      m_pauseButton(new QPushButton("Pause", this)),
      m_restartButton(new QPushButton("Restart", this)),
      m_updateTimer(new QTimer(this))
{
    setupUI();
    setupConnections();
    
    // Set window properties
    setWindowTitle("Qt5 Snake Game");
    resize(GRID_WIDTH * CELL_SIZE + 4 , GRID_HEIGHT * CELL_SIZE + 4);
    
    // Periodicall update UI
    m_updateTimer->setInterval(100);
    m_updateTimer->start();
    connect(m_updateTimer, &QTimer::timeout, this, &GameWindow::updateScoreDisplay);
    connect(m_updateTimer, &QTimer::timeout, this, &GameWindow::updateStatusDisplay);
}

GameWindow::~GameWindow()
{

}

void GameWindow::setupUI()
{
    //  Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create info bar
    QHBoxLayout *infoLayout = new QHBoxLayout();
    
    // Score display
    m_scoreLabel->setFont(QFont("Arial", 12, QFont::Bold));
    m_scoreLabel->setStyleSheet("color: darkgreen;");
    infoLayout->addWidget(m_scoreLabel);
    
    // Status display
    m_statusLabel->setFont(QFont("Arial", 12, QFont::Bold));
    m_statusLabel->setStyleSheet("color: darkblue;");
    infoLayout->addWidget(m_statusLabel);
    
    // Add stretchable space
    infoLayout->addStretch();
    
    // Control buttons
    m_startButton->setFont(QFont("Arial", 10));
    m_pauseButton->setFont(QFont("Arial", 10));
    m_restartButton->setFont(QFont("Arial", 10));
    
    infoLayout->addWidget(m_startButton);
    infoLayout->addWidget(m_pauseButton);
    infoLayout->addWidget(m_restartButton);
    
    // Add info bar and game view to main layout
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(m_gameView);
    
    // Set margins
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Initialize button states
    updateButtons();
}

void GameWindow::setupConnections()
{
    // Connect button signals
    connect(m_startButton, &QPushButton::clicked,m_gameView->getScene(), &SnakeScene::startGame);
    connect(m_pauseButton, &QPushButton::clicked, [this]() {
        auto scene = m_gameView->getScene();
        if (scene->getCurrentState() == SnakeScene::Playing) {
            scene->pauseGame();
        } else if (scene->getCurrentState() == SnakeScene::Paused) {
            scene->resumeGame();
        }
    });
    connect(m_restartButton, &QPushButton::clicked,m_gameView->getScene(), &SnakeScene::restartGame);

    // Update button states
    connect(m_updateTimer, &QTimer::timeout, this, &GameWindow::updateButtons);
}

void GameWindow::updateScoreDisplay()
{
    if (m_gameView && m_gameView->getScene()) {
        int score = m_gameView->getScene()->getCurrentScore();
        m_scoreLabel->setText(QString("Score: %1").arg(score));
    }
}

void GameWindow::updateStatusDisplay()
{
    if (m_gameView && m_gameView->getScene()) {
       SnakeScene::GameState state = m_gameView->getScene()->getCurrentState();
       QString statusText;

       switch (state) {
           case SnakeScene::Playing:
               statusText = "Status: Playing";
               break;
           case SnakeScene::Paused:
               statusText = "Status: Paused";
               break;
           case SnakeScene::GameOver:
               statusText = "Status: Game Over";
               break;
       }

       m_statusLabel->setText(statusText);
    }

}

void GameWindow::updateButtons()
{
    if (!m_gameView || !m_gameView->getScene()) return;

    SnakeScene::GameState state = m_gameView->getScene()->getCurrentState();

    // Update button text and availability
    switch (state) {
        case SnakeScene::Playing:
            m_startButton->setEnabled(false);
            m_pauseButton->setText("Pause");
            m_pauseButton->setEnabled(true);
            break;
        case SnakeScene::Paused:
            m_startButton->setEnabled(false);
            m_pauseButton->setText("Resume");
            m_pauseButton->setEnabled(true);
            break;
        case SnakeScene::GameOver:
            m_startButton->setEnabled(true);
            m_pauseButton->setText("Pause");
            m_pauseButton->setEnabled(false);
            break;
    }

}
