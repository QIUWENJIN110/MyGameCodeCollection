using UnityEngine;
using UnityEngine.SceneManagement;
using TMPro;

// Manages global game state (score, lives, level), scene loading and game flow
public class GameManager : MonoBehaviour
{
    public int level = 1;
    public int score = 0;
    public int lives = 3;

    public Ball ball { get; private set; }
    public Paddle paddle { get; private set; }
    public Brick[] bricks { get; private set; }

    public static GameManager Instance { get; private set; }

    public TextMeshProUGUI levelText;
    public TextMeshProUGUI livesText;
    public TextMeshProUGUI scoreText;

    private GameObject globalUICanvas;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(this.gameObject);
            return;
        }
        Instance = this;
        DontDestroyOnLoad(this.gameObject);

        // Core modification: Keep GlobalUICanvas persistent
        globalUICanvas = GameObject.Find("GlobalUICanvas");
        if (globalUICanvas != null)
        {
            DontDestroyOnLoad(globalUICanvas);
            Debug.Log("GlobalUICanvas set to DontDestroyOnLoad");
        }
        else
        {
            Debug.LogError("GlobalUICanvas not found! Check object name");
        }

        InitGlobalUI();

        SceneManager.sceneLoaded += OnLevelLoaded;

    }

    private void Start()
    {
        NewGame();
    }

    private void NewGame()
    {
        this.score = 0;
        this.lives = 3;

        LoadLevel(1);
    }

    private void InitGlobalUI() {

        if (globalUICanvas == null) return;

        // Auto-find UI components (if not assigned)
        if (levelText == null)
            levelText = globalUICanvas.transform.Find("LevelText")?.GetComponent<TextMeshProUGUI>();
        if (livesText == null)
            livesText = globalUICanvas.transform.Find("LivesText")?.GetComponent<TextMeshProUGUI>();
        if (scoreText == null)
            scoreText = globalUICanvas.transform.Find("ScoreText")?.GetComponent<TextMeshProUGUI>();

        // Debug: Check UI component status
        if (levelText == null) Debug.LogWarning("LevelText not found");
        if (livesText == null) Debug.LogWarning("LivesText not found");
        if (scoreText == null) Debug.LogWarning("ScoreText not found");

    }

    private void LoadLevel(int level)
    {
        this.level = level;

        SceneManager.LoadScene("Level " + level);
    }

    private void OnLevelLoaded(Scene scene, LoadSceneMode mode)
    {
        this.ball = FindObjectOfType<Ball>();
        this.paddle = FindObjectOfType<Paddle>();
        this.bricks = FindObjectsOfType<Brick>();

        InitGlobalUI();
        // Force update UI after scene load (sync UI on level change)
        UpdateUI();

        // Debug: Confirm level load and UI update
        Debug.Log($"Level {level} loaded,Lives:{lives},Score:{score}");
    }

    private void ResetLevel()
    {
        this.ball?.ResetBall();
        this.paddle?.ResetPaddle();
        /*
        for (int i = 0; i < this.bricks.Length; i++) {
            this.bricks[i].ResetBrick();
        }
        */
    }

    private bool Cleared()
    {
        // Check if all breakable bricks are destroyed
        for (int i = 0; i < this.bricks.Length; i++) {
            if (this.bricks[i].gameObject.activeInHierarchy && !this.bricks[i].unbreakable) {
                return false;
            }
        }
        return true;
    }



    private void GameOver()
    {
        //SceneManager.LoadScene("GameOver");
        NewGame();
    }

    public void Hit(Brick brick)
    {
        this.score += brick.points;

        UpdateUI();

        if (Cleared()){
            if (this.level < 5){
                LoadLevel(this.level + 1);
            }else{
                NewGame();
            }
        }

    }

    public void Miss()
    {
        this.lives--;

        UpdateUI();

        if (this.lives > 0){
            ResetLevel();
        }else {
            GameOver();
        }
    }

    public void UpdateUI()
    {
        if (levelText != null) {
            levelText.text = "Level: " + level;
        }
        if (livesText != null) {
            livesText.text = "Lives: " + lives;
        }
        if (scoreText != null) {
            scoreText.text = "Score: " + score;
        }
    }

    public void AddScore(int points)
    {
        score += points;
        UpdateUI();

    }
}
