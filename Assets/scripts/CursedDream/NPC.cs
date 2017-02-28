using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CV;

[RequireComponent(typeof(Charactor))]
public class NPC : MonoBehaviour
{
    Charactor charactor = null;

    [SerializeField]
    CircleCollider2D viewRange = null;

    [SerializeField]
    string conversationFileName = "example.xml";

    public Save convSave = new Save();
    public Save npcSave = new Save();
    public Conversation conv;

    void Awake()
    {
        conv = Conversation.CreateFromXML(Application.streamingAssetsPath + "/" + conversationFileName);
        if(conv == null)
        {
            Destroy(gameObject);
        }
        charactor = GetComponent<Charactor>();
    }

    void OnTriggerStay2D(Collider2D other)
    {

        if (!other.isTrigger)
        {
            if (other.GetComponent<Player>() != null)
            {
                Vector2 targetPos = new Vector2(other.transform.position.x,
                                        other.transform.position.y);
                Vector2 dir = targetPos - charactor.position;
                charactor.Facing(dir.normalized);
            }
        }
    }
}
