using UnityEngine;

// Detect if the ball falls into the miss zone and trigger life deduction logic
public class MissZone : MonoBehaviour
{
    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Ball") {
            FindObjectOfType<GameManager>().Miss();
        }
    }
}
