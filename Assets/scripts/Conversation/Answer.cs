using System;
using System.Runtime.InteropServices;

namespace CV
{
    public class Answer
    {
        private IntPtr implPtr = IntPtr.Zero;

        internal Answer(IntPtr implPtr)
        {
            this.implPtr = implPtr;
        }

        internal IntPtr ImplementPtr
        {
            get { return implPtr; }
        }

        public string Text
        {
            get
            {
                IntPtr textPtr = ConversationAPI.GetAnswerText(implPtr);
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

        public bool IsShown
        {
            get
            {
                return ConversationAPI.IsAnswerShown(implPtr) == 1;
            }
        }
    }
}