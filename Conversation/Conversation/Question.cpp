#include "Conversation.h"
#include "InnerConversation.h"
#include "pugixml/pugixml.hpp"

extern "C"
{
	const char* GetQuestionText(Question* question)
	{
		if (question != nullptr)
		{
			return question->GetText();
		}
		else
		{
			return "";
		}
	}

	int GetAnswerCount(Question* question)
	{
		if (question != nullptr)
		{
			return question->GetAnswerCount();
		}
		else
		{
			return 0;
		}
	}

	Answer* GetAnswerByIndex(Question* question, int index)
	{
		if (question != nullptr)
		{
			return question->GetAnswerByIndex(index);
		}
		else
		{
			return nullptr;
		}
	}
}

bool Question::LoadFromXML(const pugi::xml_node& nodeQuestion)
{
	m_name = nodeQuestion.attribute("Name").as_string();
	m_text = nodeQuestion.attribute("Text").as_string();

	for (auto nodeAnswer = nodeQuestion.first_child(); nodeAnswer; nodeAnswer = nodeAnswer.next_sibling())
	{
		Answer answer;
		if (answer.LoadFromXML(nodeAnswer))
		{
			m_answers.push_back(answer);
		}
	}
	return true;
}

Answer* Question::GetAnswerByIndex(int index)
{
	if (index >= 0 && index < static_cast<int>(m_answers.size()))
		return &(m_answers[index]);
	else
		return nullptr;
}

void Question::Destroy()
{
	for (auto& answer : m_answers)
	{
		answer.Destroy();
	}
}
