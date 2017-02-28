#include "InnerConversation.h"
#include "variant.h"
#include <cctype>

const char* EXPRESSION_OP[] =
{
	">=",
	"<=",
	"!=",
	">",
	"<",
	"==",
};

constexpr const int NUM_EXPRESSION_OP = sizeof(EXPRESSION_OP) / sizeof(const char*);

typedef bool(*CheckConditoinFunc)(const variant& m_operand1, const variant& m_operand2);
bool CheckGEqual(const variant& m_operand1, const variant& m_operand2) { return m_operand1 >= m_operand2; }
bool CheckLEqual(const variant& m_operand1, const variant& m_operand2) { return m_operand1 <= m_operand2; }
bool CheckNEqual(const variant& m_operand1, const variant& m_operand2) { return m_operand1 != m_operand2; }
bool CheckGreater(const variant& m_operand1, const variant& m_operand2) { return m_operand1 > m_operand2; }
bool CheckLess(const variant& m_operand1, const variant& m_operand2) { return m_operand1 < m_operand2; }
bool CheckEqual(const variant& m_operand1, const variant& m_operand2) { return m_operand1 == m_operand2; }
constexpr CheckConditoinFunc TEST_CONDITION_FUNC[] =
{
	CheckGEqual, CheckLEqual, CheckNEqual,
	CheckGreater,CheckLess, CheckEqual
};


bool Expression::Parse(std::string expr)
{
	size_t opPos = std::string::npos;
	int i = 0;
	for (i = 0; i < NUM_EXPRESSION_OP; i++)
	{
		opPos = expr.find(EXPRESSION_OP[i]);
		if (opPos != std::string::npos)
		{
			break;
		}
	}

	if (i == NUM_EXPRESSION_OP)
	{
		auto lb_pos = expr.find('(');
		auto rb_pos = expr.find(')');
		if (lb_pos == std::string::npos || rb_pos == std::string::npos)
		{
			return nullptr;
		}
		else
		{
			//function
			return nullptr;
		}
	}

	m_type = EXPR_TYPE(i);
	std::string leftOperand = expr.substr(0, opPos);
	std::string rightOperand = expr.substr(opPos + std::string(EXPRESSION_OP[i]).length());

	TrimString(leftOperand);
	TrimString(rightOperand);


	if (m_lhs.Parse(leftOperand) &&
		m_rhs.Parse(rightOperand))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Expression::Check()
{
	return TEST_CONDITION_FUNC[m_type](m_lhs.GetValue(), m_rhs.GetValue());
}

bool Condition::Check()
{
	if (m_type == COND_SINGLE)
	{
		return m_expression.Check();
	}
	else if (m_subConditions.size() == 0)
	{
		return true;
	}
	else if (m_type == COND_AND)
	{
		for (auto& cond : m_subConditions)
		{
			if (!cond->Check())
				return false;
		}
		return true;
	}
	else if (m_type == COND_OR)
	{
		for (auto& cond : m_subConditions)
		{
			if (cond->Check())
				return true;
		}
		return false;
	}

	//should not be here.
	//throw;
	return true;
}

Condition::~Condition()
{
	for (auto cond : m_subConditions)
	{
		delete cond;
	}
	m_subConditions.clear();
}

Condition* Condition::GenNextChild()
{
	Condition* cond = new Condition();
	m_subConditions.push_back(cond);
	return cond;
}

bool ParseCondtion(const std::string &expression, Condition*& outCondition)
{
	bool isFunction = false;
	bool hasAndToken = false;
	int curr = 0;
	int prev = 0;
	int bracketIndent = 0;
	std::vector<Condition*> stackParent;
	Condition* currCondition = outCondition;
	int exprLength = static_cast<int>(expression.length());

	while (curr < exprLength)
	{
		auto& c = expression[curr];
		if (c == '(')
		{
			curr++;
			prev = curr;
			bracketIndent++;
			stackParent.push_back(currCondition);
			currCondition = currCondition->GenNextChild();
		}
		else if (c == ')')
		{
			if (isFunction)//函数结束
			{
				isFunction = false;
				curr++;
			}
			else
			{
				if (bracketIndent == 0) //括号不对称
				{
					return false;
				}
				bracketIndent--;

				std::string subExpresstion = expression.substr(prev, curr - prev);
				if (!ParseCondtion(subExpresstion, currCondition))
				{
					return false;
				}
				currCondition = stackParent.back();
				stackParent.pop_back();
				curr++;
				while (std::isspace(expression[curr]))
				{
					curr++;
				}
				prev = curr;
			}
		}
		else
		{
			if (std::isdigit(c) || std::isalpha(c))
			{
				curr++;
				if (curr == exprLength)
				{
					if (currCondition->GetType() == COND_SINGLE)
					{
						std::string subExpresstion = expression.substr(prev);
						if (!currCondition->m_expression.Parse(subExpresstion))
						{
							return false;
						}
					}
					else
					{
						auto child = currCondition->GenNextChild();
						std::string subExpresstion = expression.substr(prev, curr - prev);
						if (!child->m_expression.Parse(subExpresstion))
						{
							return false;
						}
					}
				}
				else if (expression[curr] == '(')
				{
					isFunction = true;
					curr++;
				}
			}
			else if (c == '&')
			{
				if (curr++ >= exprLength - 2 || expression[curr] != '&')
				{
					return false;
				}
				else if (currCondition->m_type == COND_OR)
				{
					if (currCondition->m_subConditions.size() == 0)
					{
						return false;
					}
					Condition* prevCondition = currCondition->m_subConditions.back();
					Condition* andCondition = currCondition->m_subConditions.back() = new Condition();
					andCondition->m_subConditions.push_back(prevCondition);
					stackParent.push_back(currCondition);
					currCondition = andCondition;
					hasAndToken = true;
				}

				currCondition->m_type = COND_AND;
				int andExprLength = curr - prev - 1;
				if (andExprLength > 0)
				{
					auto child_cond = currCondition->GenNextChild();
					std::string subExpresstion = expression.substr(prev, andExprLength);
					if (!child_cond->m_expression.Parse(subExpresstion))
					{
						return false;
					}
				}
				else
				{
					//前一个使用括号包起来的。
				}
				curr++;
				prev = curr;
			}
			else if (c == '|')
			{
				if (curr++ >= exprLength - 2 || expression[curr] != '|')	//如果|只出现一次，或者||在最后
				{
					return false;
				}
				//or
				int orExprLength = curr - prev - 1;

				//如果是or-and-or，就把括号去掉，把原来的or表达式出栈。
				if (hasAndToken || currCondition->m_type == COND_AND)
				{
					if (orExprLength > 0)
					{
						auto child_cond = currCondition->GenNextChild();
						std::string subExpresstion = expression.substr(prev, orExprLength);
						if (!child_cond->m_expression.Parse(subExpresstion))
						{
							return false;
						}
					}
					else
					{
						//有可能是空的
					}

					if (hasAndToken)
					{
						if (stackParent.size() == 0)
						{
							outCondition = new Condition();
							outCondition->m_type = COND_OR;
							outCondition->m_subConditions.push_back(currCondition);
							currCondition = outCondition;
						}
						else
						{
							Condition* parentCondition = stackParent.back();
							stackParent.pop_back();
							if (parentCondition->m_type != COND_OR)
							{
								//should not be here.
								return false;
							}
							else
							{
								currCondition = parentCondition;
							}
						}
						hasAndToken = false;
					}
				}
				else
				{
					currCondition->m_type = COND_OR;
					if (orExprLength > 0)
					{
						auto child_cond = currCondition->GenNextChild();
						std::string subExpresstion = expression.substr(prev, orExprLength);
						if (!child_cond->m_expression.Parse(subExpresstion))
						{
							return false;
						}
					}
					else
					{
						//前一个使用括号包起来的。
					}
				}
				curr++;
				prev = curr;
			}
			else
			{
				curr++;
			}
		}
	}

	return true;
}

Condition* Condition::Create(std::string expression)
{
	Condition* condition = new Condition();
	if (ParseCondtion(expression, condition))
	{
		return condition;
	}
	else
	{
		delete condition;
		return nullptr;
	}
}
