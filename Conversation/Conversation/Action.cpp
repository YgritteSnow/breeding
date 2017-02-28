#include "Conversation.h"
#include "InnerConversation.h"
#include "pugixml/pugixml.hpp"
#include <cctype>

const char* ACTION_OP[] =
{
	":=",
	"+=",
	"-=",
	"!=",
};

constexpr int NUM_ACTION_OP = sizeof(ACTION_OP) / sizeof(const char*);

inline float ToFloat(const char* number, int length, int dot_offset)
{
	float rst = 0;
	for (int i = 0; i < dot_offset; i++)
	{
		rst *= 10.0f;
		rst += number[i] - '0';
	}

	float frac = 1.0;
	for (int i = dot_offset + 1; i < length; i++)
	{
		frac /= 10;
		rst += (number[i] - '0') * frac;
	}
	return rst;
}

inline int ToInt(const char* number, int length)
{
	int rst = 0;
	for (int i = 0; i < length; i++)
	{
		rst *= 10;
		rst += number[i] - '0';
	}
	return rst;
}

bool Operand::Parse(const std::string& expr)
{
	int length = static_cast<int>(expr.length());
	if (length == 0)
		return false;

	if (expr == "true")
	{
		m_isValue = true;
		m_value = true;
		return true;
	}
	else if (expr == "false")
	{
		m_isValue = true;
		m_value = false;
		return true;
	}
	else if (std::isalpha(expr[0]))
	{
		auto dotPos = expr.find('.');
		if (dotPos == expr.npos || dotPos == length || dotPos == 1)
			return false;

		m_targetName = expr.substr(0, dotPos);
		m_variableName = expr.substr(dotPos + 1);
		m_isValue = false;
		return true;
	}
	else
	{
		bool negative = false;
		auto beg = 0;
		if (expr[0] == '-')
		{
			negative = true;
			beg++;
		}

		auto cur = beg;
		bool isFloat = false;
		int dotOffset = 0;
		while (cur != length)
		{
			if (expr[cur] == '.')
			{
				if (isFloat || cur == beg || cur == length)
					return false;
				isFloat = true;
				dotOffset = cur;
			}
			else if (!std::isdigit(expr[cur]))
				return false;
			cur++;
		}

		if (isFloat)
		{
			float number = ToFloat(expr.c_str() + beg, length, dotOffset);
			m_value = negative ? -number : number;
		}
		else
		{
			int number = ToInt(expr.c_str() + beg, length);
			m_value = negative ? -number : number;
		}
		m_isValue = true;
		return true;
	}
}

variant& Operand::GetValue()
{
	if (m_isValue)
	{
		return m_value;
	}
	else
	{
		static variant invalidVariant;
		Save* save = nullptr;
		if (m_targetName == "global")
		{
			save = GetContext()->Global;
		}
		else if (m_targetName == "player")
		{
			save = GetContext()->Player;
		}
		else if (m_targetName == "conversation")
		{
			save = GetContext()->Conversation;
		}
		else if (m_targetName == "npc")
		{
			save = GetContext()->NPC;
		}

		if (save != nullptr)
		{
			return (*save).GetValue(m_variableName);
		}
		else
		{
			return invalidVariant;
		}
	}
}

bool Action::LoadFromXML(pugi::xml_node& node)
{
	std::string expression = node.attribute("Do").as_string();

	size_t opPos = std::string::npos;
	int i = 0;
	for (i = 0; i < NUM_ACTION_OP; i++)
	{
		opPos = expression.find(ACTION_OP[i]);
		if (opPos != std::string::npos)
		{
			break;
		}
	}

	if (i != NUM_ACTION_OP)
	{
		type = ACTION_TYPE(i);
		std::string operand1 = expression.substr(0, opPos);
		std::string operand2 = expression.substr(opPos + std::string(ACTION_OP[i]).length());

		TrimString(operand1);
		TrimString(operand2);

		if (!m_operand1.Parse(operand1) ||
			m_operand1.IsValue() ||
			!m_operand2.Parse(operand2))
		{
			return false;
		}

		if (node.attribute("If"))
		{
			m_showCondition = Condition::Create(node.attribute("If").as_string());
		}

		return true;
	}
	else
	{
		return false;
	}
}

typedef void(*DoActionFunc)(variant& m_operand1, const variant& m_operand2);
void DoSET(variant& m_operand1, const variant& m_operand2) { m_operand1 = m_operand2; }
void DoINC(variant& m_operand1, const variant& m_operand2) { m_operand1 += m_operand2; }
void DoDEC(variant& m_operand1, const variant& m_operand2) { m_operand1 -= m_operand2; }
void DoNOT(variant& m_operand1, const variant& m_operand2) { m_operand1 = !m_operand2; }
constexpr DoActionFunc DO_ACTION_FUNC[] =
{ DoSET, DoINC, DoDEC, DoNOT };

void Action::Do()
{
	if (m_showCondition == nullptr || m_showCondition->Check())
	{
		DO_ACTION_FUNC[type](m_operand1.GetValue(), m_operand2.GetValue());
	}
}

void Action::Destroy()
{
	if (m_showCondition != nullptr)
	{
		delete m_showCondition;
		m_showCondition = nullptr;
	}
}
