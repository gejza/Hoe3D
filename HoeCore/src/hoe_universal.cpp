
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"
#include "../include/hoe_universal.h"

namespace HoeCore {

Universal::Universal()
{
	Reset();
}

Universal::Universal(const char * value)
{
	Reset();
	Set(value);
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
	default:
		return "unknown";
	};
}

const tchar * Universal::GetStringValue() const
{
	switch (type)
	{
	case TypeNone: return T("");
	case TypeString: return value.str ? value.str : T("");
	case TypeDecimal: return T("decimal");
	case TypeUnsigned: return T("unsigned");
	case TypeFloat: return T("real");
	case TypeBool: return T("boolean");
	default:
		return T("unknown");
	};
}

void Universal::Clear()
{
    if (!size) return;
	switch (GetType())
	{
	case TypeString:
		SAFE_DELETE_ARRAY(value.str);
		break;
	case TypeData:
		SAFE_DELETE_ARRAY(value.p);
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
		this->value.str = new tchar[sl];
		allocated = size = sl;
		memcpy(this->value.str, value, sl * sizeof(tchar));
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

void Universal::Set(void * v)
{
	Clear();
	type = (type & 0x80000000) | TypePointer;
	value.p = v;
}

void Universal::Set(const void * v, size_t s)
{
	if (GetType() == TypeData && allocated >= s && s > 0)
	{
		memcpy(this->value.p, v, s); this->size = s; return;
	}
	Clear();
	type = (type & 0x80000000) | TypeData;
	if (s > 0)
	{
		this->value.p = new byte[s];
		allocated = this->size = s;
		memcpy(this->value.p, v, s);
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
	case TypeData: return size > 0;
	case TypePointer: return value.p != NULL;
	};
	return false;
}

unsigned long Universal::GetUnsigned() const
{
	switch (GetType())
	{
	case TypeString:
		{
			register unsigned long ret = 0;
			register const tchar * str = GetStringValue();
			return string::strtoi(str);
		}
	case TypeDecimal: return (unsigned long)value.l;
	case TypeUnsigned: return value.ul;
	case TypeFloat: return (unsigned long)value.f;
	case TypeBool: return (unsigned long)value.b;
	//case TypeFloatVector: return (unsigned long)vec_GetFloat(0);
	};
	return 0;
}

long Universal::GetDecimal() const
{
	switch (GetType())
	{
	case TypeString:
		{
			register long ret = 0;
			register const tchar * str = GetStringValue();
			if (str[0] == '0' && str[1] == 'x')
				string::scanf(str+2, T("%x"), &ret);
			else
				string::scanf(str, T("%d"), &ret);
			return ret;
		}
	case TypeDecimal: return value.l;
	case TypeUnsigned: return (long)value.ul;
	case TypeFloat: return (long)value.f;
	case TypeBool: return (long)value.b;
	//case TypeFloatVector: return (unsigned long)vec_GetFloat(0);
	};
	return 0;
}

float Universal::GetFloat() const
{
	switch (GetType())
	{
	case TypeString:
		{
			register const tchar * str = GetStringValue();
			if (str[0] == '0' && str[1] == 'x')
			{
				register float ret = 0;
                string::scanf(str+2, T("%x"), &ret);
				return (float)ret;
			}
			else
			{
				register float ret = 0.f;
                string::scanf(str, T("%f"), &ret);
				return ret;
			}
		}
	case TypeDecimal: return (float)value.l;
	case TypeUnsigned: return (float)value.ul;
	case TypeFloat: return value.f;
	case TypeBool: return value.b ? 1.f:0.f;
	//case TypeFloatVector: return vec_GetFloat(0);
	};
	return 0;
}

void * Universal::GetPointer() const 
{
	switch (GetType())
	{
	case TypePointer:
	case TypeData:
		return value.p;
	default:
		return NULL;
	};
}

/*float Universal::vec_GetFloat(size_t index) const
{
	if (GetType() == TypeFloatVector && index < size)
		return value.vf[index];
	if (index == 0 && GetType() == TypeFloat)
		return value.f;
	return 0.f;
}*/



} // namespace HoeCore


