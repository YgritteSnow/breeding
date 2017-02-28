#include "Conversation.h"
#include "InnerConversation.h"
#include "pugixml/pugixml.hpp"

extern "C"
{
	const char* GetAnswerText(Answer* answer)
	{
		if (answer != nullptr)
		{
			return answer->GetText();
		}
		else
		{
			return "";
		}
	}

	int IsAnswerShown(Answer* answer)
	{
		if (answer != nullptr && answer->IsShown())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void Answer::Destroy()
{
	if (m_showCondition != nullptr)
	{
		delete m_showCondition;
		m_showCondition = nullptr;
	}

	for (auto& action : m_actions)
	{
		action.Destroy();
	}
	m_actions.clear();

	for (auto& branch : m_jumpBranches)
	{
		branch.Destroy();
	}
	m_jumpBranches.clear();
}

bool Answer::LoadFromXML(const pugi::xml_node& nodeAnswer)
{
	ID = nodeAnswer.attribute("ID").as_int();
	m_text = nodeAnswer.attribute("Text").as_string();

	auto attribCondition = nodeAnswer.attribute("ShowCondition");
	if (attribCondition)
	{
		m_showCondition = Condition::Create(attribCondition.as_string());
	}
	else
	{
		m_showCondition = nullptr;
	}

	for (auto nodeChild = nodeAnswer.first_child(); nodeChild; nodeChild = nodeChild.next_sibling())
	{
		std::string nodeName = nodeChild.name();
		if (nodeName == "Action")
		{
			Action action;
			if (action.LoadFromXML(nodeChild))
			{
				m_actions.push_back(action);
			}
		}
		else if (nodeName == "Branch")
		{
			Branch branch;
			if (branch.LoadFromXML(nodeChild))
			{
				m_jumpBranches.push_back(branch);
			}
		}
	}
	return true;
}

std::string Answer::PickBranchQuestionName()
{
	for (auto& branch : m_jumpBranches)
	{
		if (branch.Check())
		{
			return branch.QuestioName();
		}
	}
	return "";
}

void Answer::DoActions()
{
	for (auto& action : m_actions)
	{
		action.Do();
	}
}

bool Answer::IsShown()
{
	return m_showCondition == nullptr || m_showCondition->Check();
}
