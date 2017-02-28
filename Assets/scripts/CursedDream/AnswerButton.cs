using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AnswerButton : MonoBehaviour
{

    public int Index = 0;
    public int AnswerIndex = 0;

    string text = "";
    Text textComponent = null;
    Button buttonComponent = null;
    RectTransform rectComponent = null;

    public string Text
    {
        set
        {
            text = (1 + Index).ToString() + "." + value;
            if (textComponent != null)
            {
                textComponent.text = text;
            }
        }
    }

    void Awake()
    {
        textComponent = GetComponentInChildren<Text>();
        buttonComponent = GetComponent<Button>();
        rectComponent = GetComponent<RectTransform>();
        buttonComponent.onClick.AddListener(OnClick);

    }

    void OnClick()
    {
        World.Instance.OnAnswer(AnswerIndex);
    }
}
