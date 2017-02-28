using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using CV;
using System.IO;
public class ConversationUI : MonoBehaviour
{

    Conversation conversation = null;
    Question currentQuestion = null;
    Save conversationSave = null;

    void Awake()
    {
        conversation = Conversation.CreateFromXML(Application.streamingAssetsPath + "/example.xml");
        if (conversation == null)
        {
            Application.Quit();
            DestroyObject(gameObject);
        }
        else
        {
            //set save/
            conversationSave = new Save();
            SaveContext.Instance.ConversationSave = conversationSave;
            currentQuestion = conversation.StartConversation();
        }
    }

    void OnGUI()
    {
        GUILayout.Label(currentQuestion.Text);

        int answerCount = currentQuestion.AnswerCount;
        GUILayout.Label("回答选项：" + answerCount);

        for (int i = 0; i < answerCount; i++)
        {
            Answer answer = currentQuestion.GetAnswerByIndex(i);
            if (answer != null && answer.IsShown)
            {
                if (GUILayout.Button(answer.Text))
                {
                    currentQuestion = conversation.GetNextQuestion(answer);
                    if (currentQuestion == null)
                    {
                        DestroyObject(gameObject);
                    }
                }
            }
        }

    }

    void OnDestry()
    {
        conversation.Dispose();
    }
}
