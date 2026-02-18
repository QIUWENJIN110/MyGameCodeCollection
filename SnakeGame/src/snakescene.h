// snakescene.h
#ifndef SNAKESCENE_H
#define SNAKESCENE_H

#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSoundEffect>
#include <QList>
#include <QPoint>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QSvgRenderer>
#include <QDebug>

#include "snakegame_global.h"

class SnakeScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit SnakeScene(QObject *parent = nullptr);
    ~SnakeScene();

    // Direction enumeration
    enum Direction {
        Up,
        Down,
        Left,
        Right
    };

    // Game state enumeration
    enum GameState {
        Playing,
        Paused,
        GameOver
    };

    enum FoodType {
        Apple,      //60%
        Bomb,       //30%
        Pineapple   //10%
    };

    // Get current game state
    GameState getCurrentState() const { return m_currentState; }
    // Get current score
    int getCurrentScore() const { return m_score; }
    // Get snake length
    int getSnakeLength() const { return m_snake.size(); }

public slots:
    void startGame();
    void pauseGame();
    void resumeGame();
    void restartGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();

private:
    void initAudio();   // Initialize audio
    void initGame();    // Initialize the game
    //void spawnFood();   // Spawn food
    void moveSnake();   // Move the snake
    void drawSnake();   // Draw the snake
    void renderSnakePart(int index, const QPoint &position);    // Render a single snake body part
    void checkCollisions(); // Check for collisions
    void handleGameOver();  // Handle game over
    int calculateCurrentSpeed() const;  // Calculate current speed based on snake length
    void updateMusicSpeed();    // Update background music playback speed
    void showGameOverLabel();
    void hideGameOverLabel();

    FoodType generateFoodType();    //Generate food based on probability
    void removeFood(int index);
    void respawnFood(int index);
    void onBombTimeout(int index);

    // Game elements
    Direction m_direction;            // Current movement direction
    Direction m_nextDirection;        // Next movement direction
    GameState m_currentState;         // Current game state
    int m_score;                      // Current score
    int m_speed;                      // Current movement speed
    QList<QPoint> m_snake;            // List of snake body positions 
    //QPoint m_foodPos;                 // Position of the food

    struct FoodInfo {
        QPoint pos;               // Position of the food
        FoodType type;
        QGraphicsSvgItem *item;       // Food SVG item
        QTimer *bombTimer;
    };
    QList<FoodInfo> m_foodList;

    // Graphics elements
    //QGraphicsSvgItem *m_foodItem;     // Food SVG item
    QList<QGraphicsSvgItem*> m_snakeItems; // List of snake body SVG items
    QGraphicsTextItem *m_gameOverLabel;
    QGraphicsTextItem *m_finalScoreLabel;

    // Game logic
    QTimer *m_gameTimer;              // Game loop timer
    QMediaPlaylist *m_bgmPlaylist;    // Music Playlist
    QMediaPlayer *m_bgm;              // Background music player
    QSoundEffect *m_eatSound;         // Eat food sound effect
    QSoundEffect *m_crashSound;       // Wall collision sound effect

    // Game state tracking
    bool m_gameStarted;               // Whether the game has started
    QPoint m_lastFoodPos;             // Last food position (for optimization)

};

#endif // SNAKESCENE_H
