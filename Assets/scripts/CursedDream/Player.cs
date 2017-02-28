using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
	void OnTriggerStay2D (Collider2D other)
	{
		if (!other.isTrigger)
		{
			NPC npc = other.GetComponent<NPC> ();
			if (npc != null)
			{
				World.Instance.SetHoverNPC (npc);
			}
		}
	}

	void OnTriggerExit2D (Collider2D other)
	{
		if (!other.isTrigger)
		{
			NPC npc = other.GetComponent<NPC> ();
			if (npc == World.Instance.HoverNPC)
			{
				World.Instance.SetHoverNPC (null);
			}
		}
	}

}
