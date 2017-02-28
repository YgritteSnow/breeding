using System;

namespace CV
{
    public class Value
    {
        private Save save = null;
        private string name = "";

        internal Value(string name, Save save)
        {
            this.name = name;
            this.save = save;
        }

        public ValueType Type
        {
            get
            {
                int type = ConversationAPI.GetSaveValueType(save.ImplementPtr, name);
                return (ValueType)type;
            }
        }

        public bool IsInt { get { return Type == ValueType.Int; } }

        public bool IsBoolean { get { return Type == ValueType.Boolean; } }

        public bool IsFloat { get { return Type == ValueType.Float; } }

        public int Int
        {
            get
            {
                if (save == null)
                {
                    return 0;
                }
                else
                {
                    return ConversationAPI.GetSaveValueAsInt(save.ImplementPtr, name);
                }
            }

            set
            {
                if (save != null)
                {
                    ConversationAPI.SetSaveValueInt(save.ImplementPtr, name, value);
                }
            }
        }

        public bool Boolean
        {
            get
            {
                if (save == null)
                {
                    return false;
                }
                else
                {
                    return ConversationAPI.GetSaveValueAsBoolean(save.ImplementPtr, name) == 1;
                }
            }

            set
            {
                if (save != null)
                {
                    ConversationAPI.SetSaveValueBoolean(save.ImplementPtr, name, value ? 1 : 0);
                }
            }
        }

        public float Float
        {
            get
            {
                if (save == null)
                {
                    return 0.0f;
                }
                else
                {
                    return ConversationAPI.GetSaveValueAsFloat(save.ImplementPtr, name);
                }
            }

            set
            {
                if (save != null)
                {
                    ConversationAPI.SetSaveValueFloat(save.ImplementPtr, name, value);
                }
            }
        }

        public int ToInt()
        {
            if (save == null)
            {
                return 0;
            }
            else
            {
                return ConversationAPI.GetSaveValueToInt(save.ImplementPtr, name);
            }
        }

        public bool ToBoolean()
        {
            if (save == null)
            {
                return false;
            }
            else
            {
                return ConversationAPI.GetSaveValueToBoolean(save.ImplementPtr, name) == 1;
            }
        }

        public float ToFloat()
        {
            if (save == null)
            {
                return 0.0f;
            }
            else
            {
                return ConversationAPI.GetSaveValueToFloat(save.ImplementPtr, name);
            }
        }
    }
}
