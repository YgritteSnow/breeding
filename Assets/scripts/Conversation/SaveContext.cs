using System;

namespace CV
{
    public class SaveContext : IDisposable
    {
        private IntPtr implPtr = IntPtr.Zero;
        private static SaveContext context = null;
        public static SaveContext Instance
        {
            get
            {
                if (context == null)
                {
                    ConversationAPI.InitialContext();
                    context = new SaveContext();
                }
                return context;
            }
        }

        internal SaveContext()
        {
            implPtr = ConversationAPI.GetContext();
        }

        public Save GlobalSave
        {
            get
            {
                IntPtr global = ConversationAPI.GetGlobalSave(implPtr);
                if (global != IntPtr.Zero)
                {
                    return new Save(global);
                }
                else
                {
                    return null;
                }
            }
        }

        public Save PlayerSave
        {
            get
            {
                IntPtr player = ConversationAPI.GetPlayerSave(implPtr);
                if (player != IntPtr.Zero)
                {
                    return new Save(player);
                }
                else
                {
                    return null;
                }
            }
        }

        public Save NPCSave
        {
            get
            {
                IntPtr npc = ConversationAPI.GetNPCSave(implPtr);
                if (npc != IntPtr.Zero)
                {
                    return new Save(npc);
                }
                else
                {
                    return null;
                }
            }

            set
            {
                ConversationAPI.SetNPCSave(implPtr, value.ImplementPtr);
            }
        }

        public Save ConversationSave
        {
            get
            {
                IntPtr conv = ConversationAPI.GetConversationSave(implPtr);
                if (conv != IntPtr.Zero)
                {
                    return new Save(conv);
                }
                else
                {
                    return null;
                }
            }

            set
            {
                ConversationAPI.SetConversationSave(implPtr, value.ImplementPtr);
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

                ConversationAPI.DeinitialContext();
                disposedValue = true;
            }
        }

        // TODO: 仅当以上 Dispose(bool disposing) 拥有用于释放未托管资源的代码时才替代终结器。
        ~SaveContext()
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