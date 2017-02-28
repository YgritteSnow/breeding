using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CV;
using UnityEngine.UI;


class ConvContext
{
    public Conversation Conversation;
    public Question Question;
    public int AnswerCount;
    public List<int> AnswerIndex = new List<int>();
}
public class World : MonoBehaviour
{
    public static World Instance = null;

    [SerializeField]
    SpriteRenderer hoverCircle = null;

    [SerializeField]
    GameObject UI = null;

    [SerializeField]
    Charactor charactor = null;

    [SerializeField]
    Transform mainCamera = null;

    [SerializeField]
    float movingSpeed = 2.0f;

    NPC hoverNPC = null;
    bool inConversation = false;
    ConvContext cctx = new ConvContext();

    void Awake()
    {
        Instance = this;
    }

    void Update()
    {
        if (InConversation)
        {
            ConversationInput();
        }
        else
        {
            MovementUpdate();
        }
        mainCamera.position = new Vector3(
            charactor.position.x,
            charactor.position.y,
            mainCamera.position.z);
    }

    void ConversationInput()
    {
        for (int i = 0; i < cctx.AnswerIndex.Count; i++)
        {
            if (Input.GetKeyUp(KeyCode.Alpha1 + i))
            {
                OnAnswer(cctx.AnswerIndex[i]);
            }
        }
    }

    void MovementUpdate()
    {
        if (Input.GetButton("Horizontal") || Input.GetButton("Vertical"))
        {
            Vector2 direction = new Vector2(
                                    Input.GetAxis("Horizontal"),
                                    Input.GetAxis("Vertical")).normalized;

            Vector2 offset = direction * Time.deltaTime * movingSpeed;
            charactor.Move(offset);
            charactor.Facing(offset.normalized);
        }

        if (Input.GetButton("Interact"))
        {
            if (HoverNPC != null)
            {
                StartConversation();
            }
        }

    }

    public void SetHoverNPC(NPC npc)
    {
        hoverNPC = npc;
        if (hoverNPC != null)
        {
            hoverCircle.transform.position = hoverNPC.transform.position;
            hoverCircle.enabled = true;
        }
        else
        {
            hoverCircle.enabled = false;

        }
    }

    public NPC HoverNPC { get { return hoverNPC; } }

    public bool InConversation { get { return inConversation; } }

    public void StartConversation()
    {
        UI.SetActive(true);
        SaveContext.Instance.ConversationSave = HoverNPC.convSave;
        SaveContext.Instance.NPCSave = HoverNPC.npcSave;
        inConversation = true;



        cctx.Conversation = HoverNPC.conv;
        cctx.Question = HoverNPC.conv.StartConversation();
        OnConversationShow();
    }

    public void OnAnswer(int index)
    {
        if (index >= 0)
        {
            Answer a = cctx.Question.GetAnswerByIndex(index);
            if (a != null)
            {
                Question q = cctx.Conversation.GetNextQuestion(a);
                if (q != null)
                {
                    cctx.Question = q;
                    OnConversationShow();
                    return;
                }
            }
        }
        UI.SetActive(false);
        inConversation = false;
    }

    void OnConversationShow()
    {
        cctx.AnswerCount = cctx.Question.AnswerCount;
        cctx.AnswerIndex.Clear();
        QuestionUI.text = cctx.Question.Text.Replace("\\n", "\n");
        Save s = SaveContext.Instance.ConversationSave;
        Value v = s.GetValue("count");
        if (v != null)
        {
            QuestionUI.text += "\n\n\n从Save中读取的\"conversation.count\"=" + s.GetValue("count").Int;
        }

        List<GameObject> removedButtons = new List<GameObject>();
        foreach (Transform child in ButtonList)
        {
            removedButtons.Add(child.gameObject);
        }

        removedButtons.ForEach(c => Destroy(c));


        int index = 0;
        for (int i = 0; i < cctx.AnswerCount; i++)
        {
            Answer answer = cctx.Question.GetAnswerByIndex(i);
            if (answer == null || !answer.IsShown)
                continue;

            GameObject btnObject = Instantiate(ButtonPrefab, ButtonList);
            AnswerButton answerButton = btnObject.GetComponent<AnswerButton>();
            answerButton.AnswerIndex = i;
            answerButton.Index = index;
            answerButton.Text = answer.Text;

            cctx.AnswerIndex.Add(index);
            index++;
        }

        {
            GameObject btnObject = Instantiate(ButtonPrefab, ButtonList);
            AnswerButton answerButton = btnObject.GetComponent<AnswerButton>();

            answerButton.AnswerIndex = -1;
            answerButton.Index = index;
            answerButton.Text = "默默地离开……";
            cctx.AnswerIndex.Add(-1);
        }
    }
    public Text QuestionUI = null;
    public Transform ButtonList = null;
    public GameObject ButtonPrefab = null;
}
