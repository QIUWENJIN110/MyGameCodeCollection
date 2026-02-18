#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QMap>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSoundEffect>

#include "game2048.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Main window class for 2048 game UI (handles rendering and interaction)
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void onRestartClicked();    // Restart button click event handler
    void updateGameUI();        // Update game UI (refresh tiles)
    void updateScoreDisplay();  // Update score display

private:
    Ui::MainWindow *ui;
    Game2048 m_game2048;                    // Game logic instance
    QVector<QLabel*> m_tileLabels;          // Tile label array
    QMap<QString, QString> m_styleMap;      // Style name to CSS map
    QString m_emptyTileStyle;               // Empty tile style
    QFont m_tileFont;                       // Tile font

    QLabel *m_gameOverLabel;
    bool m_isGameOver;

    QMediaPlaylist *m_bgmPlaylist;
    QMediaPlayer *m_bgm;
    QSoundEffect *m_mergeSound;
    QSoundEffect *m_gameOverSound;

    void initStyleMap();           // Initialize style map for tiles
    void initUI();                 // Initialize game board UI
    void checkGameOver();          // Check game over and show message box
    int calculateScore() const;    // Calculate current game score

    void initGameOverLabel();
    void initAudio();
    void showGameOverLabel(bool show);

};
#endif // MAINWINDOW_H
