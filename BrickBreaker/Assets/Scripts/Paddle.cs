using UnityEngine;

// Manages paddle movement and ball bounce angle calculation on collision
public class Paddle : MonoBehaviour
{
    public new Rigidbody2D rigidbody { get; private set; }
    public Vector2 direction { get; private set; }
    public AudioSource audioSource;
    public float speed = 30f;
    public float maxBounceAngle = 75f;  // Limit ball bounce angle range


    private void Awake()
    {
        this.rigidbody = GetComponent<Rigidbody2D>();
        this.audioSource = GetComponent<AudioSource>();
    }

    // Detect input every frame (Update is for input/logic, not physics)
    private void Update()
    {
        if (Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) {
            this.direction = Vector2.left;
        }else if(Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) {
            this.direction = Vector2.right;
        }else {
            this.direction = Vector2.zero;
        }
    }

    private void FixedUpdate()
    {
        // Only apply force when there is movement direction
        if (this.direction != Vector2.zero) {
            this.rigidbody.AddForce(this.direction * this.speed );
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        Ball ball = collision.gameObject.GetComponent<Ball>();    
        if (audioSource != null) {
            audioSource.Play();
        }

        if (ball != null){
            Vector3 paddlePosition = this.transform.position;   // Get current position of the paddle
            Vector2 contactPoint = collision.GetContact(0).point;   // Get collision contact point

            // Calculate X-axis offset between contact point and paddle center
            float offset = paddlePosition.x - contactPoint.x;
            // Calculate half of the paddle width (for normalizing offset)
            float width = collision.otherCollider.bounds.size.x / 2;

            // Calculate current ball angle relative to up direction
            float currentAngle = Vector2.SignedAngle(Vector2.up, ball.rigidbody.velocity);
            // Calculate bounce angle based on offset
            float bounceAngle = (offset / width) * this.maxBounceAngle;
            // Limit bounce angle within maximum range
            float newAngle = Mathf.Clamp(currentAngle + bounceAngle, -this.maxBounceAngle, this.maxBounceAngle);

            // Generate rotation from new angle
            Quaternion rotation = Quaternion.AngleAxis(newAngle, Vector3.forward);

            // Apply new velocity direction (keep speed magnitude unchanged)
            //ball.rigidbody.velocity = rotation * Vector2.up * ball.rigidbody.velocity.magnitude;

            // Apply new velocity (maintain speed magnitude)
            ball.rigidbody.velocity = rotation * Vector2.up * ball.speed * Time.fixedDeltaTime;

        }
    }

    public void ResetPaddle()
    {
        this.transform.position = new Vector2(0f, this.transform.position.y);   // Reset X position to 0, keep Y position
        this.rigidbody.velocity = Vector2.zero; // Clear rigidbody velocity
    }
}
