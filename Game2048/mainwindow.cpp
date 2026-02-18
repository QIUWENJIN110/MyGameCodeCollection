#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_isGameOver(false),
      m_bgmPlaylist(new QMediaPlaylist(this)),
      m_bgm(new QMediaPlayer(this)),
      m_mergeSound(new QSoundEffect(this)),
      m_gameOverSound(new QSoundEffect(this))
{
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow { background-color: #000000; }");  //Global setting: main window black background

    initStyleMap();          // Initialize tile styles
    initUI();                // Initialize board UI
    initAudio();
    initGameOverLabel();
    m_game2048.reset();      // Reset game logic
    updateGameUI();          // Initial UI render
    updateScoreDisplay();    // Initial score display

    connect(ui->restartButton, &QPushButton::clicked, this, &MainWindow::onRestartClicked); // Connect restart button

    // Play background audio
    if (m_bgm->state() != QMediaPlayer::PlayingState) {
        m_bgm->play();
    }
}

MainWindow::~MainWindow()
{
    if (m_bgm) {
        m_bgm->stop();
    }
    delete m_bgm;
    delete m_mergeSound;
    delete m_gameOverSound;
    delete m_gameOverLabel;
    delete ui;
}

void MainWindow::initStyleMap()
{
    // Empty tile style (dark gray + white border)
    m_emptyTileStyle = QStringLiteral("border:2px solid rgba(255,255,255,0.6); background-color: #222222; color: #FFFFFF;");

    // Tile style map for different values
    m_styleMap["tile_2"] = QStringLiteral("background-color: rgb(170, 255, 85); border: 2px solid rgba(255,255,255,0.6); color: #000000; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_4"] = QStringLiteral("background-color: rgb(85, 85, 255); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_8"] = QStringLiteral("background-color: rgb(255, 160, 183); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_16"] = QStringLiteral("background-color: rgb(255, 255, 85); border: 2px solid rgba(255,255,255,0.6); color: #000000; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_32"] = QStringLiteral("background-color: rgb(255, 85, 85); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_64"] = QStringLiteral("background-color: rgb(170, 255, 255); border: 2px solid rgba(255,255,255,0.6); color: #000000; font-size: 24px; font-weight: bold;");
    m_styleMap["tile_128"] = QStringLiteral("background-color: rgb(255, 140, 123); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 20px; font-weight: bold;");
    m_styleMap["tile_256"] = QStringLiteral("background-color: rgb(140, 0, 255); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 20px; font-weight: bold;");
    m_styleMap["tile_512"] = QStringLiteral("background-color: rgb(85, 0, 0); border: 2px solid rgba(255,255,255,0.6); color: #FFFFFF; font-size: 20px; font-weight: bold;");
    m_styleMap["tile_1024"] = QStringLiteral("background-color: rgb(0, 255, 191); border: 2px solid rgba(255,255,255,0.6); color: #000000; font-size: 16px; font-weight: bold;");
    m_styleMap["tile_2048"] = QStringLiteral("background-color: rgb(255, 255, 100); border: 2px solid rgba(255,255,255,0.6); color: #000000; font-size: 16px; font-weight: bold;");

    // Set tile font
    m_tileFont.setPointSize(30);
    m_tileFont.setBold(true);
}

void MainWindow::initGameOverLabel()
{
    m_gameOverLabel = new QLabel(this);
    QFont gameOverFont;
    gameOverFont.setPointSize(50);
    gameOverFont.setBold(true);
    m_gameOverLabel->setFont(gameOverFont);
    m_gameOverLabel->setStyleSheet(R"(
        QLabel {
            color: #FFFFFF;
        }
    )");
    m_gameOverLabel->setText("GAME OVER");
    m_gameOverLabel->setAlignment(Qt::AlignCenter);
    m_gameOverLabel->hide();

    m_gameOverLabel->setGeometry((this->width() - 400) / 2, (this->height() - 100) / 2 , 400 , 100 );
}

void MainWindow::showGameOverLabel(bool show)
{
    m_isGameOver = show;
    if (show) {
        m_gameOverLabel->setGeometry((this->width() - 400) / 2, (this->height() - 100) / 2 , 400 , 100 );
        m_gameOverLabel->show();
        m_gameOverSound->play();
        // bgm stop
        m_bgm->pause();
    } else {
        m_gameOverLabel->hide();
        // bgm start
        if (m_bgm->state() != QMediaPlayer::PlayingState) {
            m_bgm->play();
        }
    }
}

void MainWindow::initAudio()
{
    m_bgmPlaylist->addMedia(QUrl("qrc:/sounds/2048_bgm.wav"));
    m_bgmPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    m_bgm->setPlaylist(m_bgmPlaylist);
    m_bgm->setVolume(50);
    m_bgm->setPlaybackRate(1.0);

    m_mergeSound->setSource(QUrl("qrc:/sounds/2048.wav"));
    m_mergeSound->setVolume(0.8f);
    m_mergeSound->setLoopCount(1);

    m_gameOverSound->setSource(QUrl("qrc:/sounds/gg.wav"));
    m_gameOverSound->setVolume(0.8f);
    m_gameOverSound->setLoopCount(1);
}

void MainWindow::initUI()
{
    // Set focus policy (capture key events)
    setFocusPolicy(Qt::ClickFocus);

    // Create 16 tile labels
    m_tileLabels.resize(16);
    for (int i = 0; i < 16; ++i) {
        QString frameName = QString("frame_%1").arg(i + 1);
        QFrame *frame = ui->widget->findChild<QFrame*>(frameName);
        if (!frame) continue;

        QGridLayout *layout = new QGridLayout(frame);
        layout->setContentsMargins(0, 0, 0, 0);

        QLabel *label = new QLabel(frame);
        label->setStyleSheet(m_emptyTileStyle);
        label->setFixedSize(116, 116);               // Fixed tile size
        label->setAlignment(Qt::AlignCenter);        // Center text
        label->setFont(m_tileFont);                  // Set font

        layout->addWidget(label);
        m_tileLabels[i] = label;
    }

    // Restart button style
    ui->restartButton->setStyleSheet(R"(
        QPushButton {
            color: rgb(255, 255, 255);
            background-color: #444444;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #555555;
        }
    )");

}

int MainWindow::calculateScore() const
{
    QVector<int> board = m_game2048.getBoard();
    int totalScore = 0;
    for (int val : board) {
        totalScore += val; // Sum all tile values
    }
    return totalScore;
}

void MainWindow::updateScoreDisplay()
{
    int score = calculateScore();
    ui->scoreLabel->setText(QString::number(score)); // Update score text
}

void MainWindow::updateGameUI()
{
    // Get board data
    QVector<int> board = m_game2048.getBoard();
    for (int i = 0; i < board.size(); ++i) {
        int value = board[i];
        QLabel *label = m_tileLabels[i];

        // Update tile style and text
        if (value == Game2048::Empty) {
            label->setStyleSheet(m_emptyTileStyle);
            label->setText("");
        } else {
            QString styleName = Game2048::getStyleName(value);
            label->setStyleSheet(m_styleMap[styleName]);
            label->setText(QString::number(value));
        }
    }
    updateScoreDisplay(); // Sync score display
}


void MainWindow::checkGameOver()
{
    if (m_game2048.isGameOver()) {
        showGameOverLabel(true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // Close the buttons when the game ends
    if (m_isGameOver){
        return;
    }

    // Ignore if window not focused
    if (!hasFocus()) {
           QMainWindow::keyReleaseEvent(event);
           return;
    }

    bool moved = false;
    // Handle movement based on key press
    switch (event->key()) {
    case Qt::Key_Up:
       moved = m_game2048.move(Game2048::Up);
       break;
    case Qt::Key_Down:
       moved = m_game2048.move(Game2048::Down);
       break;
    case Qt::Key_Left:
       moved = m_game2048.move(Game2048::Left);
       break;
    case Qt::Key_Right:
       moved = m_game2048.move(Game2048::Right);
       break;
    default:
       QMainWindow::keyReleaseEvent(event);
       return;
    }

    // Update UI if movement succeeded
    if (moved) {
       m_mergeSound->play();
       QTimer::singleShot(150, this, &MainWindow::updateGameUI);
       QTimer::singleShot(200, this, &MainWindow::checkGameOver);
    }
}

void MainWindow::onRestartClicked()
{
    m_game2048.reset();     // Reset game logic
    updateGameUI();         // Update UI
    updateScoreDisplay();   // Reset score
    showGameOverLabel(false);
}
