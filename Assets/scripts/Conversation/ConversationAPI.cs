using System;
using System.Runtime.InteropServices;


namespace CV
{
    public enum ValueType : int
    {
        Int = 0,
        Float = 1,
        Boolean = 2,
        None = 3,
        Invalid = 4,
    }
    static class ConversationAPI
    {
        [DllImport("Conversation")]
        public static extern void InitialContext();

        [DllImport("Conversation")]
        public static extern void DeinitialContext();

        [DllImport("Conversation")]
        public static extern IntPtr GetContext();

        [DllImport("Conversation")]
        public static extern IntPtr GetGlobalSave(IntPtr ctx);

        [DllImport("Conversation")]
        public static extern IntPtr GetPlayerSave(IntPtr ctx);

        [DllImport("Conversation")]
        public static extern IntPtr GetConversationSave(IntPtr ctx);

        [DllImport("Conversation")]
        public static extern IntPtr GetNPCSave(IntPtr ctx);

        [DllImport("Conversation")]
        public static extern void SetConversationSave(IntPtr ctx, IntPtr save);

        [DllImport("Conversation")]
        public static extern void SetNPCSave(IntPtr ctx, IntPtr save);

        [DllImport("Conversation")]
        public static extern IntPtr CreateSave();

        [DllImport("Conversation")]
        public static extern void ReleaseSave(IntPtr save);

        [DllImport("Conversation")]
        public static extern int SaveHasValue(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern int GetSaveValueType(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern int GetSaveValueAsInt(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern float GetSaveValueAsFloat(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern int GetSaveValueAsBoolean(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern int GetSaveValueToInt(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern float GetSaveValueToFloat(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern int GetSaveValueToBoolean(IntPtr save, string name);

        [DllImport("Conversation")]
        public static extern void SetSaveValueInt(IntPtr save, string name, int v);

        [DllImport("Conversation")]
        public static extern void SetSaveValueFloat(IntPtr save, string name, float v);

        [DllImport("Conversation")]
        public static extern void SetSaveValueBoolean(IntPtr save, string name, int v);

        [DllImport("Conversation")]
        public static extern IntPtr CreateConversationFromXML(string filePath);

        [DllImport("Conversation")]
        public static extern void ReleaseConversation(IntPtr pConversation);

        [DllImport("Conversation")]
        public static extern int GetConversationID(IntPtr pConversation);

        [DllImport("Conversation")]
        public static extern int IsConversationValid(IntPtr pConversation);

        [DllImport("Conversation")]
        public static extern IntPtr StartConversation(IntPtr pConversation);

        [DllImport("Conversation")]
        public static extern IntPtr GetNextQuestion(IntPtr pConversation, IntPtr selectedAnswer);

        [DllImport("Conversation")]
        public static extern IntPtr GetQuestionText(IntPtr question);

        [DllImport("Conversation")]
        public static extern int GetAnswerCount(IntPtr question);

        [DllImport("Conversation")]
        public static extern IntPtr GetAnswerByIndex(IntPtr question, int index);

        [DllImport("Conversation")]
        public static extern IntPtr GetAnswerText(IntPtr answer);

        [DllImport("Conversation")]
        public static extern int IsAnswerShown(IntPtr answer);
    }
}