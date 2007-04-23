
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_universal.h"

namespace HoeCore {

Universal::Universal()
{
	Reset();
}

Universal::~Universal()
{
	Clear();
}

void Universal::Reset()
{
	type = TypeNone;
	size = allocated = 0;
	value.l = 0;
}

const char * Universal::GetTypeName(Universal::Type t)
{
	switch (t)
	{
	case TypeNone: return "none";
	case TypeString: return "string";
	case TypeDecimal: return "decimal";
	case TypeUnsigned: return "unsigned";
	case TypeFloat: return "real";
	case TypeBool: return "boolean";
	case TypeFloatVector: return "real vector";
	case TypeDecimalVector: return "decimal vector";
	default:
		return "unknown";
	};
}

const char * Universal::GetStringValue() const
{
	switch (type)
	{
	case TypeNone: return "";
	case TypeString: return value.str ? value.str : "";
	case TypeDecimal: return "decimal";
	case TypeUnsigned: return "unsigned";
	case TypeFloat: return "real";
	case TypeBool: return "boolean";
	case TypeFloatVector: return "real vector";
	case TypeDecimalVector: return "decimal vector";
	default:
		return "unknown";
	};
}

void Universal::Clear()
{
	switch (GetType())
	{
	case TypeString:
		SAFE_DELETE_ARRAY(value.str);
		break;
	case TypeFloatVector:
		SAFE_DELETE_ARRAY(value.vf);
		break;
	};
	allocated = 0;
	size = 0;
}

void Universal::Set(const char * value)
{
	size_t sl = strlen(value)+1;
	if (GetType() == TypeString && allocated >= sl && sl > 1)
	{
		memcpy(this->value.str, value, sl); size = sl; return;
	}
	Clear();
	type = (type & 0x80000000) | TypeString;
	if (sl > 1)
	{
		this->value.str = new char[sl];
		allocated = size = sl;
		memcpy(this->value.str, value, sl);
	}
}

void Universal::Set(float f)
{
	Clear();
	type = (type & 0x80000000) | TypeFloat;
	value.f = f;
}

void Universal::Set(unsigned long ul)
{
	Clear();
	type = (type & 0x80000000) | TypeUnsigned;
	value.ul = ul;
}

void Universal::Set(long l)
{
	Clear();
	type = (type & 0x80000000) | TypeDecimal;
	value.l = l;
}

void Universal::Set(int i)
{
	Clear();
	type = (type & 0x80000000) | TypeDecimal;
	value.l = (long)i;
}

void Universal::Set(bool b)
{
	Clear();
	type = (type & 0x80000000) | TypeBool;
	value.b = b;

}

/*void Universal::Set(const Universal & u)
{
	if ()
}*/

void Universal::Set(const float * v, size_t s)
{
	if (GetType() == TypeFloatVector && allocated >= s && s > 0)
	{
		memcpy(this->value.vf, v, s * sizeof(float)); this->size = s; return;
	}
	Clear();
	type = (type & 0x80000000) | TypeFloatVector;
	if (s > 0)
	{
		this->value.vf = new float[s];
		allocated = this->size = s;
		memcpy(this->value.vf, v, s * sizeof(float));
	}
}

bool Universal::GetBool() const
{
	switch (GetType())
	{
	case TypeNone: return false;
	case TypeString:
		if (value.str && value.str[0] == 't' || value.str[0] == 'y')
			return true;
		return false;
	case TypeDecimal: return value.l != 0;
	case TypeUnsigned: return value.ul != 0;
	case TypeFloat: return value.f != 0.f;
	case TypeBool: return value.b;
	case TypeFloatVector: return size > 0;
	case TypeDecimalVector: return size > 0;
	};
	return false;
}

float Universal::vec_GetFloat(size_t index) const
{
	if (GetType() == TypeFloatVector && index < size)
		return value.vf[index];
	if (index == 0 && GetType() == TypeFloat)
		return value.f;
	return 0.f;
}

} // namespace HoeCore

