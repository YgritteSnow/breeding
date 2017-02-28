using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody2D))]
public class Charactor : MonoBehaviour
{
    private Rigidbody2D body = null;

    private void Awake()
    {
        body = GetComponent<Rigidbody2D>();
    }

    public void MoveTo(Vector2 position)
    {
        body.MovePosition(position);
    }

    public void Move(Vector2 offset)
    {
        MoveTo(body.position + offset);
    }

    public void Facing(Vector2 direction)
    {
        transform.right = direction;
    }

    public Vector2 position
    {
        get { return body.position; }
    }


}
