#include "Conversation.h"
#include "InnerConversation.h"
#include "pugixml/pugixml.hpp"
#include <cctype>
#include <algorithm>

void Branch::Destroy()
{
	if (m_showCondition != nullptr)
	{
		delete m_showCondition;
		m_showCondition = nullptr;
	}
}

bool Branch::LoadFromXML(const pugi::xml_node& nodeGoto)
{
	m_questionName = nodeGoto.attribute("QuestionName").as_string();
	auto attribCondition = nodeGoto.attribute("If");
	if (attribCondition)
	{
		m_showCondition = Condition::Create(attribCondition.as_string());
		return m_showCondition != nullptr;
	}
	else
	{
		m_showCondition = nullptr;
		return true;
	}
}

void TrimString(std::string& inoutString)
{
	auto end = std::remove_if(
		inoutString.begin(),
		inoutString.end(),
		[](char c) { return std::isspace(c); });

	inoutString.erase(end, inoutString.end());
}

bool Save::HasValue(const std::string& name)
{
	return m_values.count(name) > 0;
}

variant& Save::GetValue(const std::string& name)
{
	if (!HasValue(name))
	{
		m_values.insert(std::make_pair(name, variant(0)));
	}
	return m_values[name];
}

void Save::Clear()
{
	m_values.clear();
}

void Save::SetValue(const std::string& name, int v)
{
	if (HasValue(name))
	{
		m_values[name] = v;
	}
	else
	{
		m_values.insert(std::make_pair(name, variant(v)));
	}
}

void Save::SetValue(const std::string& name, bool v)
{
	if (HasValue(name))
	{
		m_values[name] = v;
	}
	else
	{
		m_values.insert(std::make_pair(name, variant(v)));
	}
}

void Save::SetValue(const std::string& name, float v)
{
	if (HasValue(name))
	{
		m_values[name] = v;
	}
	else
	{
		m_values.insert(std::make_pair(name, variant(v)));
	}
}

SaveContext g_SaveContext;
extern "C"
{
	void InitialContext()
	{
		if (g_SaveContext.Global == nullptr)
		{
			g_SaveContext.Global = CreateSave();
		}

		if (g_SaveContext.Player == nullptr)
		{
			g_SaveContext.Player = CreateSave();
		}
	}

	void DeinitialContext()
	{
		if (g_SaveContext.Global != nullptr)
		{
			ReleaseSave(g_SaveContext.Global);
			g_SaveContext.Global = nullptr;
		}

		if (g_SaveContext.Player != nullptr)
		{
			ReleaseSave(g_SaveContext.Player);
			g_SaveContext.Player = nullptr;
		}
	}

	SaveContext* GetContext()
	{
		return &g_SaveContext;
	}

	Save* GetGlobalSave(SaveContext* ctx)
	{
		return ctx->Global;
	}

	Save* GetPlayerSave(SaveContext* ctx)
	{
		return ctx->Player;
	}

	Save* GetConversationSave(SaveContext* ctx)
	{
		return ctx->Conversation;
	}

	Save* GetNPCSave(SaveContext* ctx)
	{
		return ctx->NPC;
	}

	void SetConversationSave(SaveContext* ctx, Save* save)
	{
		if (ctx != nullptr)
		{
			ctx->Conversation = save;
		}
	}

	void SetNPCSave(SaveContext* ctx, Save* save)
	{
		if (ctx != nullptr)
		{
			ctx->NPC = save;
		}
	}
}