#include<string>
#include "variant.h"

variant::variant()
{
	m_value_type = VT_NONE;
	memset(m_value_buffer, 0, MMAX::SIZE);
}

variant::variant(const variant& other)
{
	m_value_type = other.m_value_type;
	memcpy(m_value_buffer, other.m_value_buffer, MMAX::SIZE);
}

variant::variant(int v)
{
	*this = v;
}

variant::variant(float v)
{
	*this = v;
}

variant::variant(bool v)
{
	*this = v;
}

variant::operator int() const
{
	switch (m_value_type)
	{
	case VT_INT:
		return *((int*)m_value_buffer);
	case VT_FLOAT:
		return static_cast<int>(this->as<float>());
	case VT_BOOL:
		return (this->as<bool>() ? 1 : 0);
	default:
		return 0;
	}
}

variant::operator float() const
{
	switch (m_value_type)
	{
	case VT_INT:
		return static_cast<float>(this->as<int>());
	case VT_FLOAT:
		return *((float*)m_value_buffer);
	case VT_BOOL:
		return (this->as<bool>() ? 1.0f : 0.0f);
	default:
		return 0.0f;
	}
}

variant::operator bool() const
{
	switch (m_value_type)
	{
	case VT_INT:
		return this->as<int>() != 0;
	case VT_BOOL:
		return *((bool*)m_value_buffer);
	default:
		return false;
	}
}

variant& variant::operator=(int v)
{
	m_value_type = VT_INT;
	*((int*)m_value_buffer) = v;
	return *this;
}

variant& variant::operator=(float v)
{
	m_value_type = VT_FLOAT;
	*((float*)m_value_buffer) = v;
	return *this;
}

variant& variant::operator=(bool v)
{
	m_value_type = VT_BOOL;
	*((bool*)m_value_buffer) = v;
	return *this;
}

variant& variant::operator=(const variant& rhs)
{
	if (&rhs == this)
		return *this;

	m_value_type = rhs.m_value_type;
	memcpy(m_value_buffer, rhs.m_value_buffer, MMAX::SIZE);
	return *this;
}


variant& variant::operator+=(const variant& rhs)
{
	switch (rhs.get_type())
	{
	case VT_NONE:
		break;
	case VT_INT:
		*this = this->as<int>() + rhs.as<int>();
		break;
	case VT_FLOAT:
		*this = this->as<float>() + rhs.as<float>();
		break;
	case VT_BOOL:
		*this = (this->as<int>() ^ rhs.as<int>()) ? true : false;
		break;
	}
	return *this;
}

variant& variant::operator-=(const variant& rhs)
{
	switch (rhs.get_type())
	{
	case VT_NONE:
		break;
	case VT_INT:
		*this = this->as<int>() - rhs.as<int>();
		break;
	case VT_FLOAT:
		*this = this->as<float>() - rhs.as<float>();
		break;
	case VT_BOOL:
		*this = this->as<int>() - rhs.as<int>() > 0;
		break;
	}
	return *this;
}

bool variant::operator>= (const variant& rhs) const
{
	if (this == &rhs)
		return true;

	switch (rhs.get_type())
	{
	case VT_INT: return this->as<int>() >= rhs.as<int>();
	case VT_FLOAT: return this->as<float>() >= rhs.as<float>();
	case VT_BOOL: return this->as<bool>() >= rhs.as<bool>();
	default: return false;
	}
}

bool variant::operator<= (const variant& rhs) const
{
	if (this == &rhs)
		return true;

	switch (rhs.get_type())
	{
	case VT_INT: return this->as<int>() <= rhs.as<int>();
	case VT_FLOAT: return this->as<float>() <= rhs.as<float>();
	case VT_BOOL: return this->as<bool>() <= rhs.as<bool>();
	default: return false;
	}
}

bool variant::operator> (const variant& rhs) const
{
	if (this == &rhs)
		return false;

	switch (rhs.get_type())
	{
	case VT_INT: return this->as<int>() > rhs.as<int>();
	case VT_FLOAT: return this->as<float>() > rhs.as<float>();
	case VT_BOOL: return this->as<bool>() > rhs.as<bool>();
	default: return false;
	}
}

bool variant::operator< (const variant& rhs) const
{
	if (this == &rhs)
		return false;

	switch (rhs.get_type())
	{
	case VT_INT: return this->as<int>() < rhs.as<int>();
	case VT_FLOAT: return this->as<float>() < rhs.as<float>();
	case VT_BOOL: return this->as<bool>() < rhs.as<bool>();
	default: return false;
	}
}

bool variant::operator== (const variant& rhs) const
{
	if (this == &rhs)
		return true;

	if (m_value_type == rhs.m_value_type)
	{
		return (0 == memcmp(m_value_buffer, rhs.m_value_buffer, MMAX::SIZE));
	}
	else
	{
		VAR_TYPE dst = m_value_type;
		if (m_value_type == VT_NONE)
		{
			dst = rhs.m_value_type;
		}
		else if (rhs.m_value_type == VT_NONE)
		{
			dst = m_value_type;
		}

		switch (dst)
		{
		default:
		case VT_NONE:	return false;
		case VT_INT:	return this->as<int>() == rhs.as<int>();
		case VT_FLOAT:	return this->as<float>() == rhs.as<float>();
		case VT_BOOL:	return this->as<bool>() == rhs.as<bool>();
		}
	}
}

bool variant::operator!= (const variant& rhs) const
{
	return !(*this == rhs);
}

bool variant::operator!() const
{
	if (m_value_type == VT_BOOL)
		return !this->as<bool>();
	else if (m_value_type == VT_INT)
		return this->as<int>() == 0;
	return true;
}

variant::VAR_TYPE variant::get_type() const
{
	return m_value_type;
}
