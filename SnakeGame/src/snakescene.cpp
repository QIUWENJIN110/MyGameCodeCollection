// snakescene.cpp

#include "snakescene.h"

SnakeScene::SnakeScene(QObject *parent)
    : QGraphicsScene(0, 0, GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, parent),
      m_direction(Right),
      m_nextDirection(Right),
      m_currentState(GameOver),
      m_score(0),
      m_speed(INITIAL_SPEED),
      //m_foodItem(nullptr),
      m_gameOverLabel(nullptr),
      m_finalScoreLabel(nullptr),
      m_gameTimer(new QTimer(this)),
      m_bgmPlaylist(new QMediaPlaylist(this)),
      m_bgm(new QMediaPlayer(this)),
      m_eatSound(new QSoundEffect(this)),
      m_crashSound(new QSoundEffect(this)),
      m_gameStarted(false),
      m_lastFoodPos(QPoint(-1,-1))
{
    // Set scene background color
    setBackgroundBrush(QColor(240, 248, 255)); // AliceBlue background

    initAudio();
    initGame();

    connect(m_gameTimer, &QTimer::timeout, this, &SnakeScene::gameLoop);    // Connect game loop
}

SnakeScene::~SnakeScene()
{
    // Clean up resources
    if (m_bgm) {
        m_bgm->stop();
    }

    // Clean up labels if they exist
    if (m_gameOverLabel) {
        delete m_gameOverLabel;
        m_gameOverLabel = nullptr;
    }

    if (m_finalScoreLabel) {
        delete m_finalScoreLabel;
        m_finalScoreLabel = nullptr;
    }

    // Clean up bomb timer and food
    for (auto& food : m_foodList) {
        if (food.bombTimer) {
            food.bombTimer->stop();
            delete food.bombTimer;
        }
        if (food.item) {
            removeItem(food.item);
            delete food.item;
        }
    }
}

void SnakeScene::initAudio()
{
    m_bgmPlaylist->addMedia(QUrl("qrc:/sounds/bgm.wav"));
    m_bgmPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    m_bgm->setPlaylist(m_bgmPlaylist);
    m_bgm->setVolume(50);
    m_bgm->setPlaybackRate(1.0);

    m_eatSound->setSource(QUrl("qrc:/sounds/eat.wav"));
    m_eatSound->setVolume(0.8f);
    m_eatSound->setLoopCount(1);

    m_crashSound->setSource(QUrl("qrc:/sounds/crash.wav"));
    m_crashSound->setVolume(0.8f);
    m_crashSound->setLoopCount(1);
}

void SnakeScene::initGame()
{
    // Clear previous graphic items
    for (auto& food : m_foodList) {
        if (food.bombTimer) {
            food.bombTimer->stop();
            delete food.bombTimer;
        }
        if (food.item) {
            removeItem(food.item);
            delete food.item;
        }
    }
    m_foodList.clear();

    for (auto item : m_snakeItems) {
        removeItem(item);
        delete item;
    }
    m_snakeItems.clear();

    // Hide game over labels
    hideGameOverLabel();

    // Reset game state
    m_snake.clear();
    m_direction = Right;
    m_nextDirection = Right;
    m_currentState = GameOver;
    m_score = 0;
    m_speed = INITIAL_SPEED;
    m_lastFoodPos = QPoint(-1, -1);

    // Initialize snake position (initial length of 3)
    m_snake.append(QPoint(5, 7));  // Head
    m_snake.append(QPoint(4, 7));  // Body
    m_snake.append(QPoint(3, 7));  // Tail

    // Spawn 3 foods
    for (int i = 0; i < FOOD_COUNT; ++i) {
        respawnFood(i);
    }

    drawSnake();    // Draw initial state
}

/*
void SnakeScene::spawnFood()
{
    // Randomly generate food position ensuring it's not on the snake
    QPoint newPosition;
    bool validPosition = false;

    while (!validPosition) {
        int x = QRandomGenerator::global()->bounded(GRID_WIDTH);
        int y = QRandomGenerator::global()->bounded(GRID_HEIGHT);
        newPosition = QPoint(x, y);

        // Check if new position overlaps with snake body
        validPosition = !m_snake.contains(newPosition);
    }

    m_foodPos = newPosition;
}
*/

void SnakeScene::respawnFood(int index)
{
    // Randomly generate food position ensuring it's not on the snake
    FoodType type = generateFoodType();
    QPoint newPos;
    bool valid = false;
    while (!valid) {
        if(type == Pineapple){
            newPos = QPoint(GRID_CENTER_X,GRID_CENTER_Y);
            // Check center point
            valid = !m_snake.contains(newPos);
            for (int i = 0; i < m_foodList.size(); ++i) {
                if (i != index && m_foodList[i].pos == newPos) {
                    valid = false;
                    break;
                }
            }
            // if the center is unavailable, generate apple
            if (!valid) {
                type = Apple;
                int x = QRandomGenerator::global()->bounded(GRID_WIDTH);
                int y = QRandomGenerator::global()->bounded(GRID_HEIGHT);
                newPos = QPoint(x, y);
                valid = !m_snake.contains(newPos);
                for (int i = 0; i < m_foodList.size(); ++i) {
                    if (i != index && m_foodList[i].pos == newPos) {
                        valid = false;
                        break;
                    }
                }
            }
        }else{
            int x = QRandomGenerator::global()->bounded(GRID_WIDTH);
            int y = QRandomGenerator::global()->bounded(GRID_HEIGHT);
            newPos = QPoint(x, y);

            // Check if new position overlaps with snake body
            valid = !m_snake.contains(newPos);
            // Check if new position overlaps with other foods
            for (int i = 0; i < m_foodList.size(); ++i) {
                if (i != index && m_foodList[i].pos == newPos) {
                    valid = false;
                    break;
                }
            }
        }
    }


    if (index < m_foodList.size()) {
       auto& food = m_foodList[index];
       // clean up
       if (food.bombTimer) {
           food.bombTimer->stop();
           delete food.bombTimer;
           food.bombTimer = nullptr;
       }
       if (food.item) {
           removeItem(food.item);
           delete food.item;
           food.item = nullptr;
       }
       food.pos = newPos;
       food.type = type;
    } else {
       FoodInfo newFood;
       newFood.pos = newPos;
       newFood.type = type;
       newFood.item = nullptr;
       newFood.bombTimer = nullptr;
       m_foodList.append(newFood);
    }

    // Create SVG Item
    QString svgPath;
    if (type == Apple) {
       svgPath = ":/images/apple.svg";
    } else if (type == Bomb) {
       svgPath = ":/images/bomb.svg";
    } else {
       svgPath = ":/images/pineapple.svg";
    }

    QGraphicsSvgItem* item = new QGraphicsSvgItem(svgPath);
    item->setScale(static_cast<qreal>(CELL_SIZE) / item->renderer()->defaultSize().width());
    item->setPos(newPos.x() * CELL_SIZE, newPos.y() * CELL_SIZE);
    addItem(item);
    m_foodList[index].item = item;

    if (type == Bomb) {
       QTimer* timer = new QTimer(this);
       timer->setSingleShot(true);
       connect(timer, &QTimer::timeout, this, [this, index]() {
           onBombTimeout(index);
       });
       timer->start(BOMB_TIMEOUT);
       m_foodList[index].bombTimer = timer;
    }
}

void SnakeScene::onBombTimeout(int foodIndex)
{
    if (foodIndex >= 0 && foodIndex < m_foodList.size() && m_currentState == Playing) {
        respawnFood(foodIndex);
    }
}

void SnakeScene::removeFood(int index)
{
    if (index < 0 || index >= m_foodList.size()) return;
    auto& food = m_foodList[index];
    if (food.bombTimer) {
        food.bombTimer->stop();
        delete food.bombTimer;
        food.bombTimer = nullptr;
    }
    if (food.item) {
        removeItem(food.item);
        delete food.item;
        food.item = nullptr;
    }

    respawnFood(index);
}

void SnakeScene::drawSnake()
{
    // Remove old snake body graphic items
    for (auto item : m_snakeItems) {
        removeItem(item);
        delete item;
    }
    m_snakeItems.clear();

    // Draw new snake body
    for (int i = 0; i < m_snake.size(); ++i) {
        renderSnakePart(i, m_snake[i]);
    }

    for(int i=0;i<m_foodList.size();++i){
        auto& food = m_foodList[i];
        // Remove old food graphic item
        if (food.item) {
            removeItem(food.item);
            delete food.item;
            food.item = nullptr;
        }
        QString foodSvgPath;
        if (food.type == Apple) {
            foodSvgPath = ":/images/apple.svg";
        } else if (food.type == Bomb) {
            foodSvgPath = ":/images/bomb.svg";
        } else {
            foodSvgPath = ":/images/pineapple.svg";
        }
        // Draw new food
        QGraphicsSvgItem *foodItem = new QGraphicsSvgItem(foodSvgPath);
        foodItem->setScale(static_cast<qreal>(CELL_SIZE) / foodItem->renderer()->defaultSize().width());
        foodItem->setPos(food.pos.x() * CELL_SIZE, food.pos.y() * CELL_SIZE);
        addItem(foodItem);
        food.item = foodItem;
    }
}

void SnakeScene::renderSnakePart(int index, const QPoint &position)
{
    // Determine snake part type based on index
    QString svgPath;
    if (index == 0) {
        svgPath = ":/images/head.svg";  // Snake head
    } else if (index == m_snake.size() - 1) {
        svgPath = ":/images/tail.svg";  // Snake tail
    } else {
        svgPath = ":/images/body.svg";  // Snake body
    }

    QGraphicsSvgItem *item = new QGraphicsSvgItem(svgPath);

    // Set size and position
    item->setScale(static_cast<qreal>(CELL_SIZE) / item->renderer()->defaultSize().width());
    item->setPos(position.x() * CELL_SIZE, position.y() * CELL_SIZE);

    // If it's the head, rotate based on direction
    if (index == 0) {
        qreal rotation = 0;
        switch (m_direction) {
            case Up:
                rotation = 270;
                break;
            case Down:
                rotation = 90;
                break;
            case Left:
                rotation = 180;
                break;
            case Right:
                rotation = 0;
                break;
        }
        item->setRotation(rotation);
        item->setTransformOriginPoint(15, 15); // Center point of SVG
    }

    addItem(item);
    m_snakeItems.append(item);
}

void SnakeScene::moveSnake()
{
    if (m_currentState != Playing){
        return;
    }

    m_direction = m_nextDirection;      // Update direction

    // Calculate new head position
    QPoint newHead = m_snake.first();
    switch (m_direction) {
        case Up:
            newHead.setY(newHead.y() - 1);
            break;
        case Down:
            newHead.setY(newHead.y() + 1);
            break;
        case Left:
            newHead.setX(newHead.x() - 1);
            break;
        case Right:
            newHead.setX(newHead.x() + 1);
            break;
    }

    // Check if food was eaten
    int eatenIndex = -1;
    FoodType eatenType = Apple;
    for (int i = 0; i < m_foodList.size(); ++i) {
        if (newHead == m_foodList[i].pos) {
            eatenIndex = i;
            eatenType = m_foodList[i].type;
            break;
        }
    }

    if (eatenIndex != -1) {
        if (eatenType == Apple) {
            // length +1 , score +10
            m_snake.prepend(newHead);
            m_score += 10;
            if (m_eatSound->isLoaded()) {
                m_eatSound->play();
            }
            removeFood(eatenIndex);
        } else if (eatenType == Bomb) {
            // bomb:GameOver
            handleGameOver();
            return;
        } else if (eatenType == Pineapple) {
            // length +3 , score +30
            m_snake.prepend(newHead);
            QPoint extra1 = newHead;
            QPoint extra2 = newHead;
            switch (m_direction) {
                case Up: extra1.setY(extra1.y()-1); extra2.setY(extra2.y()-2); break;
                case Down: extra1.setY(extra1.y()+1); extra2.setY(extra2.y()+2); break;
                case Left: extra1.setX(extra1.x()-1); extra2.setX(extra2.x()-2); break;
                case Right: extra1.setX(extra1.x()+1); extra2.setX(extra2.x()+2); break;
            }
            m_snake.prepend(extra1);
            m_snake.prepend(extra2);
            m_score += 30;
            if (m_eatSound->isLoaded()) {
                m_eatSound->play();
            }
            removeFood(eatenIndex);
        }
    } else {
        m_snake.prepend(newHead);
        m_snake.removeLast();
    }

    drawSnake();    // Redraw

    // Update speed (based on snake length)
    m_speed = calculateCurrentSpeed();
    m_gameTimer->setInterval(m_speed);

    updateMusicSpeed(); // Update music speed
}

void SnakeScene::checkCollisions()
{
    if (m_currentState != Playing){
        return;
    }

    QPoint head = m_snake.first();

    // Check wall collision
    if (head.x() < 0 || head.x() >= GRID_WIDTH ||
        head.y() < 0 || head.y() >= GRID_HEIGHT) {
        handleGameOver();
        return;
    }

    // Check self collision (starting from second node)
    for (int i = 1; i < m_snake.size(); ++i) {
        if (head == m_snake[i]) {
            handleGameOver();
            return;
        }
    }
}

void SnakeScene::handleGameOver()
{
    m_currentState = GameOver;
    m_gameTimer->stop();

    // Play crash sound effect
    if (m_crashSound->isLoaded()) {
        m_crashSound->play();
    }

    // Output game over information
    qDebug() << "Game Over! Final Score:" << m_score
             << "Snake Length:" << m_snake.size();

    // Show game over information on canvas
    showGameOverLabel();

    if (m_bgm->state() == QMediaPlayer::PlayingState) {
        m_bgm->stop();
    }
}

int SnakeScene::calculateCurrentSpeed() const
{
    // Calculate speed based on snake length
    // Longer snake moves faster, but cannot exceed minimum value
    int accelerateStep = (m_snake.size() -3) / 3;
    int lengthBasedSpeed = INITIAL_SPEED - accelerateStep * SPEED_INCREMENT;
    return qMax(lengthBasedSpeed, MIN_SPEED);
}

void SnakeScene::updateMusicSpeed()
{
    // Adjust background music playback speed based on game speed
    qreal musicSpeed = 1.0 + (INITIAL_SPEED - m_speed) / 500.0;
    m_bgm->setPlaybackRate(qBound(1.0, musicSpeed, 2.0));
}

void SnakeScene::showGameOverLabel()
{
    // Create game over label if it doesn't exist
    if (!m_gameOverLabel) {
        m_gameOverLabel = new QGraphicsTextItem();
        m_gameOverLabel->setDefaultTextColor(QColor(255, 0, 0)); // Red color
        m_gameOverLabel->setFont(QFont("Arial", 24, QFont::Bold));
        m_gameOverLabel->setPlainText("GAME OVER");
    }

    // Create final score label if it doesn't exist
    if (!m_finalScoreLabel) {
        m_finalScoreLabel = new QGraphicsTextItem();
        m_finalScoreLabel->setDefaultTextColor(QColor(0, 0, 255)); // Blue color
        m_finalScoreLabel->setFont(QFont("Arial", 16, QFont::Normal));
        m_finalScoreLabel->setPlainText(QString("Final Score: %1\nSnake Length: %2").arg(m_score).arg(m_snake.size()));
    }

    // Position the labels in the center of the scene
    qreal sceneWidth = GRID_WIDTH * CELL_SIZE;
    qreal sceneHeight = GRID_HEIGHT * CELL_SIZE;

    // Position game over label
    m_gameOverLabel->setPos(sceneWidth / 2 - m_gameOverLabel->boundingRect().width() / 2,sceneHeight / 2 - 50);

    // Position final score label below the game over label
    m_finalScoreLabel->setPos(sceneWidth / 2 - m_finalScoreLabel->boundingRect().width() / 2,sceneHeight / 2 + 10);

    // Add labels to scene if they're not already added
    if (!m_gameOverLabel->scene()) {
        addItem(m_gameOverLabel);
    }

    if (!m_finalScoreLabel->scene()) {
        addItem(m_finalScoreLabel);
    }

    // Update the text in case score changed during collision detection
    m_finalScoreLabel->setPlainText(QString("Final Score: %1\nSnake Length: %2").arg(m_score).arg(m_snake.size()));
}

void SnakeScene::hideGameOverLabel()
{
    // Remove and hide the game over labels
    if (m_gameOverLabel && m_gameOverLabel->scene()) {
       removeItem(m_gameOverLabel);
    }

    if (m_finalScoreLabel && m_finalScoreLabel->scene()) {
       removeItem(m_finalScoreLabel);
    }
}

SnakeScene::FoodType SnakeScene::generateFoodType()
{
    int randVal = QRandomGenerator::global()->bounded(100); // 0-99
    if (randVal < 60) {
        return Apple; // 60%
    } else if (randVal < 90) {
        return Bomb;  // 30%
    } else {
        return Pineapple; // 10%
    }
}

void SnakeScene::gameLoop()
{
    if (m_currentState == Playing) {
        moveSnake();
        checkCollisions();
    }
}

void SnakeScene::keyPressEvent(QKeyEvent *event)
{
    if (m_currentState == GameOver) {
        // On game over state, space bar restarts the game
        if (event->key() == Qt::Key_Space) {
            restartGame();
        }
        return;
    }

    // P key for pause/resume
    if (event->key() == Qt::Key_P) {
        if (m_currentState == Playing) {
            pauseGame();
        } else if (m_currentState == Paused) {
            resumeGame();
        }
        return;
    }

    // Space key for restart
    if (event->key() == Qt::Key_Space) {
        restartGame();
        return;
    }

    // Arrow keys for control (prevent 180-degree turns)
    switch (event->key()) {
        case Qt::Key_Up:
            if (m_direction != Down) m_nextDirection = Up;
            break;
        case Qt::Key_Down:
            if (m_direction != Up) m_nextDirection = Down;
            break;
        case Qt::Key_Left:
            if (m_direction != Right) m_nextDirection = Left;
            break;
        case Qt::Key_Right:
            if (m_direction != Left) m_nextDirection = Right;
            break;
        default:
            QGraphicsScene::keyPressEvent(event);
            return;
    }
}

void SnakeScene::startGame()
{
    if (m_currentState == GameOver) {
        initGame();
    }

    m_currentState = Playing;
    m_gameTimer->start(m_speed);

    // Start playing background music
    if (m_bgm->state() != QMediaPlayer::PlayingState) {
        m_bgm->play();
    }

    m_gameStarted = true;
}

void SnakeScene::pauseGame()
{
    if (m_currentState == Playing) {
        m_currentState = Paused;
        m_gameTimer->stop();

        // Pause background music
        if(m_bgm->state() == QMediaPlayer::PlayingState){
            m_bgm->pause();
        }
    }
}

void SnakeScene::resumeGame()
{
    if (m_currentState == Paused) {
        m_currentState = Playing;
        m_gameTimer->start(m_speed);

        // Resume background music
        if(m_bgm->state() == QMediaPlayer::PausedState) {
            m_bgm->play();
        }
    }
}

void SnakeScene::restartGame()
{
    initGame();
    startGame();
}
