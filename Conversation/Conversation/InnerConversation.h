#pragma once

#include "variant.h"
#include <string>
#include <vector>
#include <map>

namespace pugi
{
	class xml_node;
}

void TrimString(std::string& inoutString);

enum ACTION_TYPE
{
	AOP_SET = 0,
	AOP_INC = 1,
	AOP_DEC = 2,
	AOP_NOT = 3,
	AOP_FUNC = 4,
};

enum EXPR_TYPE
{
	EXPR_GEQUAL = 0,
	EXPR_LEQUAL = 1,
	EXPR_NOSAME = 2,
	EXPR_GREATER = 3,
	EXPR_LESS = 4,
	EXPR_SAME = 5,
};

enum COND_TYPE
{
	COND_SINGLE, COND_AND, COND_OR,
};

struct Save
{
public:
	bool HasValue(const std::string& name);
	variant& GetValue(const std::string& name);
	void SetValue(const std::string& name, int v);
	void SetValue(const std::string& name, bool v);
	void SetValue(const std::string& name, float v);
	void Clear();

private:
	std::map<std::string, variant> m_values;
};

struct SaveContext
{
	Save* Global = nullptr;
	Save* Player = nullptr;
	Save* NPC = nullptr;
	Save* Conversation = nullptr;
};

struct Operand
{
public:
	bool Parse(const std::string& operand);
	variant& GetValue();
	inline bool IsValue() const { return m_isValue; }

private:
	bool m_isValue = true;
	variant m_value;
	std::string m_targetName = "";
	std::string m_variableName = "";
};

struct Expression
{
public:
	bool Parse(std::string expression);
	bool Check();

private:
	EXPR_TYPE m_type = EXPR_SAME;
	Operand m_lhs, m_rhs;
};

struct Condition
{
public:
	static Condition* Create(std::string expression);

	~Condition();
	bool Check();
	inline COND_TYPE GetType() const { return m_type; }

private:
	friend bool ParseCondtion(const std::string &expression, Condition*& outCondition);
	Condition* GenNextChild();


	COND_TYPE m_type;
	Expression m_expression;
	std::vector<Condition*> m_subConditions;
};

struct Action
{
public:
	bool LoadFromXML(pugi::xml_node& node);
	void Do();
	void Destroy();

private:
	ACTION_TYPE type;
	Operand m_operand1;
	Operand m_operand2;
	std::string m_funcName;
	Condition* m_showCondition = nullptr;
};

struct Branch
{
	bool LoadFromXML(const pugi::xml_node&);
	void Destroy();
	inline bool Check() { return m_showCondition == nullptr || m_showCondition->Check(); }
	inline const std::string& QuestioName() const { return m_questionName; }

private:
	std::string m_questionName = "";
	Condition* m_showCondition = nullptr;

};

struct Answer
{
public:
	void Destroy();
	bool LoadFromXML(const pugi::xml_node& node);
	void DoActions();
	std::string PickBranchQuestionName();
	inline int GetID() const { return ID; }
	inline const char* GetText() const { return m_text.c_str(); }
	bool IsShown();

private:
	int ID;
	std::string m_text = "";
	Condition* m_showCondition = nullptr;
	std::vector<Branch> m_jumpBranches;
	std::vector<Action> m_actions;
};

struct Question
{
public:
	void Destroy();
	bool LoadFromXML(const pugi::xml_node& node);
	inline const char* GetName() const { return m_name.c_str(); }
	inline const char* GetText() const { return m_text.c_str(); }
	inline int GetAnswerCount() const { return static_cast<int>(m_answers.size()); }
	Answer* GetAnswerByIndex(int index);

private:
	std::string m_name = "";
	std::string m_text = "";
	std::vector<Answer> m_answers;
};


struct Conversation
{
public:
	~Conversation();
	bool LoadFromXML(const pugi::xml_node& node);
	bool IsValid();
	inline int GetID() const { return ID; }
	Question* StartConversation();
	Question* GetNextQuestion(Answer* selectedAnswer);

private:
	Question* GetDefaultQuestion();
	void OnNextQuestion();

	int ID = 0;
	Condition* m_validCondition = nullptr;
	std::vector<Branch> m_entranceBranches;
	std::map<std::string, Question> m_questions;
};
