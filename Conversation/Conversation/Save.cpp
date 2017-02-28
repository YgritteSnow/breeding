#include "Conversation.h"
#include "InnerConversation.h"

extern "C"
{
	Save* CreateSave()
	{
		return new Save();
	}

	void ReleaseSave(Save* save)
	{
		if (save != nullptr)
		{
			delete save;
		}
	}

	int SaveHasValue(Save* save, const char* name)
	{
		return (save != nullptr && name != nullptr && save->HasValue(name)) ? 1 : 0;
	}

	int GetSaveValueType(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			auto type = save->GetValue(name).get_type();
			switch (type)
			{
			case variant::VT_BOOL:return VAL_BOOL;
			case variant::VT_INT:return VAL_INT;
			case variant::VT_FLOAT:return VAL_FLOAT;
			case variant::VT_NONE:return VAL_NONE;
			}
		}
		return VAL_INVALID;
	}

	int GetSaveValueAsInt(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return save->GetValue(name).as<int>();
		}
		else
		{
			return 0;
		}
	}

	float GetSaveValueAsFloat(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return save->GetValue(name).as<float>();
		}
		else
		{
			return 0.0f;
		}
	}

	int GetSaveValueAsBoolean(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return (save->GetValue(name).as<bool>() ? 1 : 0);
		}
		else
		{
			return 0;
		}
	}

	int GetSaveValueToInt(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return (int)(save->GetValue(name));
		}
		else
		{
			return 0;
		}
	}

	float GetSaveValueToFloat(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return (float)(save->GetValue(name));
		}
		else
		{
			return 0.0f;
		}
	}

	int GetSaveValueToBoolean(Save* save, const char* name)
	{
		if (save != nullptr && name != nullptr && save->HasValue(name))
		{
			return ((bool)(save->GetValue(name))) ? 1 : 0;
		}
		else
		{
			return 0;
		}
	}

	void SetSaveValueInt(Save* save, const char* name, int v)
	{
		if (save != nullptr && name != nullptr)
		{
			save->SetValue(name, v);
		}
	}

	void SetSaveValueFloat(Save* save, const char* name, float v)
	{
		if (save != nullptr && name != nullptr)
		{
			save->SetValue(name, v);
		}
	}

	void SetSaveValueBoolean(Save* save, const char* name, int v)
	{
		if (save != nullptr && name != nullptr)
		{
			save->SetValue(name, (v != 0));
		}
	}
}