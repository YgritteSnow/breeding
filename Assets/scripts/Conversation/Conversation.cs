using System;

namespace CV
{
    public class Conversation : IDisposable
    {
        private IntPtr implPtr = IntPtr.Zero;

        internal IntPtr ImplementPtr { get { return implPtr; } }

        static public Conversation CreateFromXML(string filePath)
        {
            IntPtr convPtr = ConversationAPI.CreateConversationFromXML(filePath);
            if (convPtr != IntPtr.Zero)
            {
                Conversation conv = new Conversation();
                conv.implPtr = convPtr;
                return conv;
            }
            else
            {
                return null;
            }
        }

        public int ID
        {
            get { return ConversationAPI.GetConversationID(implPtr); }
        }

        public bool IsValid
        {
            get { return ConversationAPI.IsConversationValid(implPtr) == 1; }
        }


        public Question StartConversation()
        {
            IntPtr questionPtr = ConversationAPI.StartConversation(implPtr);
            if (questionPtr != IntPtr.Zero)
            {
                return new Question(questionPtr);
            }
            else
            {
                return null;
            }
        }

        public Question GetNextQuestion(Answer selectedAnswer)
        {
            IntPtr questionPtr = ConversationAPI.GetNextQuestion(implPtr, selectedAnswer.ImplementPtr);
            if (questionPtr != IntPtr.Zero)
            {
                return new Question(questionPtr);
            }
            else
            {
                return null;
            }
        }

        #region IDisposable Support
        private bool disposedValue = false; // 要检测冗余调用

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: 释放托管状态(托管对象)。
                }

                // TODO: 释放未托管的资源(未托管的对象)并在以下内容中替代终结器。
                // TODO: 将大型字段设置为 null。
                if (implPtr != IntPtr.Zero)
                {
                    ConversationAPI.ReleaseConversation(implPtr);
                    implPtr = IntPtr.Zero;
                }
                disposedValue = true;
            }
        }

        // TODO: 仅当以上 Dispose(bool disposing) 拥有用于释放未托管资源的代码时才替代终结器。
        ~Conversation()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(false);
        }

        // 添加此代码以正确实现可处置模式。
        public void Dispose()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(true);
            // TODO: 如果在以上内容中替代了终结器，则取消注释以下行。
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}