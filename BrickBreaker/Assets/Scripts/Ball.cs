using UnityEngine;

// Responsible for ball's physical movement, position reset and random initial trajectory setup
public class Ball : MonoBehaviour
{
    public new Rigidbody2D rigidbody { get; private set; }
    public AudioSource audioSource;
    public float speed = 500f;

    private void Awake()
    {
        this.rigidbody = GetComponent<Rigidbody2D>();
        this.audioSource = GetComponent<AudioSource>();

        rigidbody.drag = 0f;
        rigidbody.angularDrag = 0f;
        rigidbody.gravityScale = 0f;

    }

    private void Start()
    {
        ResetBall();
    }

    // Set random initial movement trajectory
    // Ensure the ball moves downward (negative Y-axis) with random left/right offset initially
    private void SetRandomTrajectory()
    {
        Vector2 force = Vector2.zero;
        force.x = Random.Range(-1f, 1f);    // Random X force (-1 to 1)
        force.y = -1f;                      // Fixed downward Y force

        // Add normalized force (consistent speed magnitude)
        this.rigidbody.AddForce(force.normalized * this.speed);
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Brick") {
            if (audioSource != null && audioSource.clip != null){
                audioSource.Play();
            }
        }
    }

    public void ResetBall()
    {
        this.transform.position = Vector2.zero; // Reset to world origin
        this.rigidbody.velocity = Vector2.zero; // Clear rigidbody velocity (prevent residual movement)

        Invoke(nameof(SetRandomTrajectory), 1f);
    }

}
