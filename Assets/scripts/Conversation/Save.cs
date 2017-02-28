using System;

namespace CV
{
    public class Save : IDisposable
    {
        private IntPtr implPtr = IntPtr.Zero;

        internal IntPtr ImplementPtr { get { return implPtr; } }

        internal Save(IntPtr implPtr)
        {
            this.implPtr = implPtr;
            disposedValue = true;
        }

        public Save()
        {
            implPtr = ConversationAPI.CreateSave();
        }

        public Value GetValue(string name)
        {
            if (HasValue(name))
            {
                return new Value(name, this);
            }
            return null;
        }

        private bool HasValue(string name)
        {
            if (name == null || name == "")
                return false;

            return ConversationAPI.SaveHasValue(ImplementPtr, name) == 1;
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
                if (implPtr != IntPtr.Zero)
                {
                    ConversationAPI.ReleaseSave(implPtr);
                    implPtr = IntPtr.Zero;
                }
                disposedValue = true;
            }
        }

        // TODO: 仅当以上 Dispose(bool disposing) 拥有用于释放未托管资源的代码时才替代终结器。
        ~Save()
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
