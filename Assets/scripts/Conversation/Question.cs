using System;
using System.Runtime.InteropServices;

namespace CV
{
    public class Question
    {
        private IntPtr implPtr = IntPtr.Zero;

        internal Question(IntPtr implPtr)
        {
            this.implPtr = implPtr;
        }

        public string Text
        {
            get
            {
                IntPtr textPtr = ConversationAPI.GetQuestionText(implPtr);
                if (textPtr != IntPtr.Zero)
                {
                    string text = Marshal.PtrToStringAnsi(textPtr);
                    if (text != null)
                    {
                        return text;
                    }
                }

                return "";
            }
        }

        public int AnswerCount
        {
            get
            {
                return ConversationAPI.GetAnswerCount(implPtr);
            }
        }

        public Answer GetAnswerByIndex(int index)
        {
            IntPtr answerPtr = ConversationAPI.GetAnswerByIndex(implPtr, index);
            if (answerPtr != IntPtr.Zero)
            {
                return new Answer(answerPtr);
            }
            else
            {
                return null;
            }
        }
    }
}