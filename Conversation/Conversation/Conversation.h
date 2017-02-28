#pragma once

#if defined _WIN32
#ifdef CONV_DLLEXPORT
#define CONV_API __declspec(dllexport)
#else
#define CONV_API __declspec(dllimport)
#endif
#else
#ifdef DLL_EXPORTS
#define CONV_API __attributes((visibility("default")))
#else
#define CONV_API 
#endif
#endif


#define VAL_INT		0
#define VAL_FLOAT	1
#define VAL_BOOL	2
#define VAL_NONE	3
#define VAL_INVALID	4


struct Question;
struct Answer;
struct Conversation;
struct SaveContext;
struct Save;


extern "C"
{
	CONV_API void InitialContext();

	CONV_API void DeinitialContext();

	CONV_API SaveContext* GetContext();

	CONV_API Save* GetGlobalSave(SaveContext* ctx);

	CONV_API Save* GetPlayerSave(SaveContext* ctx);

	CONV_API Save* GetConversationSave(SaveContext* ctx);

	CONV_API Save* GetNPCSave(SaveContext* ctx);

	CONV_API void SetConversationSave(SaveContext* ctx, Save* conv);

	CONV_API void SetNPCSave(SaveContext* ctx, Save* save);

	//save
	CONV_API Save* CreateSave();

	CONV_API void ReleaseSave(Save* save);

	CONV_API int SaveHasValue(Save*, const char* name);

	CONV_API int GetSaveValueType(Save*, const char* name);

	CONV_API int GetSaveValueAsInt(Save*, const char* name);

	CONV_API float GetSaveValueAsFloat(Save*, const char* name);

	CONV_API int GetSaveValueAsBoolean(Save*, const char* name);

	CONV_API int GetSaveValueToInt(Save*, const char* name);

	CONV_API float GetSaveValueToFloat(Save*, const char* name);

	CONV_API int GetSaveValueToBoolean(Save*, const char* name);

	CONV_API void SetSaveValueInt(Save*, const char* name, int );

	CONV_API void SetSaveValueFloat(Save*, const char* name, float);

	CONV_API void SetSaveValueBoolean(Save*, const char* name, int );

	//Conversation
	CONV_API Conversation* CreateConversationFromXML(const char* filePath);

	CONV_API void ReleaseConversation(Conversation* pConversation);

	CONV_API int GetConversationID(Conversation*);

	CONV_API int IsConversationValid(Conversation*);

	CONV_API Question* StartConversation(Conversation*);

	CONV_API Question* GetNextQuestion(Conversation*, Answer* selectedAnswer);

	//Question
	CONV_API const char* GetQuestionText(Question*);

	CONV_API int GetAnswerCount(Question*);

	CONV_API Answer* GetAnswerByIndex(Question*, int index);

	//Answer
	CONV_API const char* GetAnswerText(Answer*);

	CONV_API int IsAnswerShown(Answer*);
}
