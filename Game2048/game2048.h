// game2048.h
#ifndef GAME2048_H
#define GAME2048_H

#include <QVector>
#include <QRandomGenerator>

class Game2048
{

public:
    Game2048();

    enum Direction {
        Up,
        Down,
        Left,
        Right
    };

    static const int Empty = 0;

    void reset();   // Reset game (initialize with two random tiles)
    bool move(Direction dir);   // Whether movement/merge succeeded
    bool isGameOver() const;    // Check if game is over (no empty tiles and no merge possible)
    QVector<int> getBoard() const { return m_board; }   // Get current board data (16 tiles in 4x4 grid)
    static QString getStyleName(int value); // Get style name for specific tile value (for UI styling)

private:
    QVector<int> m_board;   // 4x4 board stored as 1D array (index 0-15)
    void spawnRandomTile(); // Spawn random tile (2 or 4, 60% chance for 2)

    // Internal movement logic
    bool moveUp();    // Move tiles up
    bool moveDown();  // Move tiles down
    bool moveLeft();  // Move tiles left
    bool moveRight(); // Move tiles right

    QVector<int> mergeRow(const QVector<int>& row, bool& merged);   // Merge same values in a row/column

    QVector<int> collapseRow(const QVector<int>& row);  // Remove empty tiles and pad with zeros (clean up row/column)

    bool hasMergeableTiles() const; // Check if there are mergeable adjacent tiles

};



#endif // GAME2048_H

