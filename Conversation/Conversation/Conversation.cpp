#include "Conversation.h"
#include "InnerConversation.h"
#include "pugixml/pugixml.hpp"

extern "C"
{
	Conversation* CreateConversationFromXML(const char* filePath)
	{
		pugi::xml_document xmlFile;
		auto result = xmlFile.load_file(filePath, pugi::parse_full);
		if (result.status != pugi::status_ok)
		{
			return nullptr;
		}

		Conversation* conv = new Conversation();
		if (conv->LoadFromXML(xmlFile.child("Conversation")))
		{
			return conv;
		}
		else
		{
			delete conv;
			return nullptr;
		}
	}

	void ReleaseConversation(Conversation* conv)
	{
		if (conv != nullptr)
		{
			delete conv;
		}
	}

	int GetConversationID(Conversation* conv)
	{
		if (conv != nullptr)
		{
			return conv->GetID();
		}
		else
		{
			return 0;
		}
	}

	int IsConversationValid(Conversation* conv)
	{
		if (conv != nullptr && conv->IsValid())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	Question* StartConversation(Conversation* conv)
	{
		if (conv != nullptr)
		{
			return conv->StartConversation();
		}
		else
		{
			return nullptr;
		}
	}

	Question* GetNextQuestion(Conversation* conv, Answer* selectedAnswer)
	{
		if (conv != nullptr)
		{
			return conv->GetNextQuestion(selectedAnswer);
		}
		else
		{
			return nullptr;
		}
	}
}

bool Conversation::LoadFromXML(const pugi::xml_node& root)
{
	ID = root.attribute("ID").as_int();

	auto nodeValidCondition = root.attribute("ValidCondition");
	if (nodeValidCondition)
	{
		m_validCondition = Condition::Create(nodeValidCondition.as_string());
	}

	auto nodeEntrance = root.child("Entrances");
	for (auto nodeGoto = nodeEntrance.first_child(); nodeGoto; nodeGoto = nodeGoto.next_sibling())
	{
		Branch branch;
		if (branch.LoadFromXML(nodeGoto))
		{
			m_entranceBranches.push_back(branch);
		}
	}

	auto nodeQuestions = root.child("Questions");
	for (auto nodeQuestion = nodeQuestions.first_child(); nodeQuestion; nodeQuestion = nodeQuestion.next_sibling())
	{
		Question question;
		question.LoadFromXML(nodeQuestion);
		m_questions.insert(std::make_pair(question.GetName(), question));
	}
	return true;
}

Conversation::~Conversation()
{
	if (m_validCondition != nullptr)
	{
		delete m_validCondition;
		m_validCondition = nullptr;
	}

	for (auto& entrance : m_entranceBranches)
	{
		entrance.Destroy();
	}

	for (auto& question : m_questions)
	{
		question.second.Destroy();
	}
}

bool Conversation::IsValid()
{
	return (m_validCondition == nullptr || m_validCondition->Check());
}

Question* Conversation::StartConversation()
{
	auto save = GetContext()->Conversation;
	if (save != nullptr)
	{
		save->Clear();
		save->GetValue("count") = 0;
	}
	return GetDefaultQuestion();
}

Question* Conversation::GetDefaultQuestion()
{
	for (auto& branch : m_entranceBranches)
	{
		if (branch.Check())
		{
			OnNextQuestion();
			return &(m_questions[branch.QuestioName()]);
		}
	}
	return nullptr;
}

Question* Conversation::GetNextQuestion(Answer* selectedAnswer)
{
	auto save = GetContext()->Conversation;
	bool isQuit = (save == nullptr) || save->GetValue("quit").as<bool>();

	if (!isQuit && selectedAnswer != nullptr)
	{
		if (save != nullptr)
		{
			save->GetValue("last_id") = selectedAnswer->GetID();
		}

		auto questionName = selectedAnswer->PickBranchQuestionName();
		if (m_questions.count(questionName) > 0) //it equals to != ""
		{
			OnNextQuestion();
			return &(m_questions[questionName]);
		}
		else
		{
			return GetDefaultQuestion();
		}
	}
	else
	{
		return nullptr;
	}
}

void Conversation::OnNextQuestion()
{
	auto save = GetContext()->Conversation;
	if (save != nullptr)
	{
		variant& vcount = save->GetValue("count");
		vcount = vcount.as<int>() + 1;
	}

}