#include "game2048.h"

Game2048::Game2048()
{
    m_board.resize(16);
}

void Game2048::reset()
{
    m_board.fill(Empty);       // Clear board
    spawnRandomTile();         // Spawn first random tile
    spawnRandomTile();         // Spawn second random tile
}

bool Game2048::move(Game2048::Direction dir)
{
    bool moved = false;
    switch (dir) {
        case Up: moved = moveUp(); break;
        case Down: moved = moveDown(); break;
        case Left: moved = moveLeft(); break;
        case Right: moved = moveRight(); break;
    }

    // Spawn new tile if movement succeeded
    if (moved) {
        spawnRandomTile();
    }
    return moved;
}

bool Game2048::isGameOver() const
{
    // Check for empty tiles
    if(m_board.contains(Empty)){
        return false;
    }

    // Check for mergeable tiles
    return !hasMergeableTiles();
}

QString Game2048::getStyleName(int value)
{
    switch (value) {
       case 2: return "tile_2";
       case 4: return "tile_4";
       case 8: return "tile_8";
       case 16: return "tile_16";
       case 32: return "tile_32";
       case 64: return "tile_64";
       case 128: return "tile_128";
       case 256: return "tile_256";
       case 512: return "tile_512";
       case 1024: return "tile_1024";
       case 2048: return "tile_2048";
       case 4096: return "tile_4096";
       default: return "tile_empty";
    }
}

void Game2048::spawnRandomTile()
{
    // Collect empty tile indices
    QVector<int> emptyIndices;
    for (int i = 0; i < m_board.size(); ++i) {
        if (m_board[i] == Empty) {
            emptyIndices.append(i);
        }
    }
    if (emptyIndices.isEmpty()) {
        return;
    }

    // Select random empty position
    int idx = emptyIndices[QRandomGenerator::global()->bounded(emptyIndices.size())];
    // 60% chance for 2, 40% chance for 4
    m_board[idx] = QRandomGenerator::global()->bounded(10) < 6 ? 2 : 4;
}

bool Game2048::moveUp()
{
    bool moved = false;
    // Process each column (4 columns total)
    for (int col = 0; col < 4; ++col) {
        QVector<int> column;
        // Extract column data
        for (int row = 0; row < 4; ++row) {
            column.append(m_board[row * 4 + col]);
        }
        bool merged = false;
        QVector<int> newColumn = mergeRow(collapseRow(column), merged);
        // Pad with zeros to 4 elements
        while (newColumn.size() < 4) {
            newColumn.append(Empty);
        }
        // Update column data
        for (int row = 0; row < 4; ++row) {
            if (m_board[row * 4 + col] != newColumn[row]) {
                moved = true;
                m_board[row * 4 + col] = newColumn[row];
            }
        }
    }
    return moved;
}

bool Game2048::moveDown()
{
    bool moved = false;
    // Process each column (4 columns total)
    for (int col = 0; col < 4; ++col) {
        QVector<int> column;
        // Extract column data (reverse order)
        for (int row = 3; row >= 0; --row) {
            column.append(m_board[row * 4 + col]);
        }
        bool merged = false;
        QVector<int> newColumn = mergeRow(collapseRow(column), merged);
        // Pad with zeros and reverse
        while (newColumn.size() < 4) {
            newColumn.append(Empty);
        }
        std::reverse(newColumn.begin(), newColumn.end());
        // Update column data
        for (int row = 0; row < 4; ++row) {
            if (m_board[row * 4 + col] != newColumn[row]) {
                moved = true;
                m_board[row * 4 + col] = newColumn[row];
            }
        }
    }
    return moved;
}

bool Game2048::moveLeft()
{
    bool moved = false;
    // Process each row (4 rows total)
    for (int row = 0; row < 4; ++row) {
        QVector<int> currentRow;
        // Extract row data
        for (int col = 0; col < 4; ++col) {
            currentRow.append(m_board[row * 4 + col]);
        }
        bool merged = false;
        QVector<int> newRow = mergeRow(collapseRow(currentRow), merged);
        // Pad with zeros to 4 elements
        while (newRow.size() < 4) {
            newRow.append(Empty);
        }
        // Update row data
        for (int col = 0; col < 4; ++col) {
            if (m_board[row * 4 + col] != newRow[col]) {
                moved = true;
                m_board[row * 4 + col] = newRow[col];
            }
        }
    }
    return moved;
}

bool Game2048::moveRight()
{
    bool moved = false;
    // Process each row (4 rows total)
    for (int row = 0; row < 4; ++row) {
        QVector<int> currentRow;
        // Extract row data (reverse order)
        for (int col = 3; col >= 0; --col) {
            currentRow.append(m_board[row * 4 + col]);
        }
        bool merged = false;
        QVector<int> newRow = mergeRow(collapseRow(currentRow), merged);
        // Pad with zeros and reverse
        while (newRow.size() < 4) {
            newRow.append(Empty);
        }
        std::reverse(newRow.begin(), newRow.end());
        // Update row data
        for (int col = 0; col < 4; ++col) {
            if (m_board[row * 4 + col] != newRow[col]) {
                moved = true;
                m_board[row * 4 + col] = newRow[col];
            }
        }
    }
    return moved;
}

QVector<int> Game2048::mergeRow(const QVector<int> &row, bool &merged)
{
    QVector<int> result;
    for (int i = 0; i < row.size(); ++i) {
        if (i + 1 < row.size() && row[i] == row[i+1] && row[i] != Empty) {
            // Merge same values
            result.append(row[i] * 2);
            merged = true;
            i++; // Skip next element
        } else {
            result.append(row[i]);
        }
    }
    return result;
}

QVector<int> Game2048::collapseRow(const QVector<int> &row)
{
    QVector<int> collapsed;
    for (int val : row) {
        if (val != Empty) {
            collapsed.append(val);
        }
    }
    return collapsed;
}

bool Game2048::hasMergeableTiles() const
{
    // Check horizontal adjacency
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 3; ++col) {
            int idx = row * 4 + col;
            if (m_board[idx] == m_board[idx + 1] && m_board[idx] != Empty) {
                return true;
            }
        }
    }
    // Check vertical adjacency
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 3; ++row) {
            int idx = row * 4 + col;
            if (m_board[idx] == m_board[idx + 4] && m_board[idx] != Empty) {
                return true;
            }
        }
    }
    return false;
}
