
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"
#include "../include/hoe_universal.h"
#include "../include/hoe_mem.h"

namespace HoeCore {

Universal::Universal()
{
	Reset();
}

Universal::Universal(const char * value, HoeCore::MemoryPool* pool)
{
	Reset();
	Set(value, pool);
}

Universal::Universal(TDecimal value)
{
	Reset();
	Set(value);
}

Universal::Universal(const Universal & value, HoeCore::MemoryPool* pool)
{
	Reset();
	Set(value, pool);
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
	case TypeWString: return "unicode";
	case TypeDecimal: return "decimal";
	case TypeUnsigned: return "unsigned";
	case TypeFloat: return "real";
	case TypeBool: return "boolean";
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
	case TypeWString: return "unicode";
	case TypeDecimal: return "decimal";
	case TypeUnsigned: return "unsigned";
	case TypeFloat: return "real";
	case TypeBool: return "boolean";
	default:
		return "unknown";
	};
}

const wchar_t * Universal::GetWideStringValue() const
{
	switch (type)
	{
	case TypeNone: return L"";
	case TypeString: return L"string";
	case TypeWString: return value.wstr ? value.wstr : L"";
	case TypeDecimal: return L"decimal";
	case TypeUnsigned: return L"unsigned";
	case TypeFloat: return L"real";
	case TypeBool: return L"boolean";
	default:
		return L"unknown";
	};
}

void Universal::Clear()
{
    if (!size) return;
	if (!IsPooled())
	{
		switch (GetType())
		{
		case TypeString:
			SAFE_DELETE_ARRAY(value.str);
			break;
		case TypeData:
			SAFE_DELETE_ARRAY(value.p);
			break;
		};
	}
	allocated = 0;
	size = 0;
}

void Universal::Set(const char * value, HoeCore::MemoryPool* pool)
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
		if (pool)
		{
			this->value.str = new (*pool) char[sl];
			type |= 0x40000000;
		}
		else
			this->value.str = new char[sl];
		allocated = size = sl;
		memcpy(this->value.str, value, sl);
	}
}

void Universal::Set(const wchar_t * value, HoeCore::MemoryPool* pool)
{
	size_t sl = (string::len(value)+1)*sizeof(wchar_t);
	if (GetType() == TypeWString && allocated >= sl && sl > 1)
	{
		memcpy(this->value.str, value, sl); size = sl; return;
	}
	Clear();
	type = (type & 0x80000000) | TypeWString;
	if (sl > 1)
	{
		if (pool)
		{
			this->value.wstr = new (*pool) wchar_t[sl];
			type |= 0x40000000;
		}
		else
			this->value.wstr = new wchar_t[sl];
		allocated = size = sl;
		memcpy(this->value.str, value, sl);
	}
}

void Universal::Set(TReal f)
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

void Universal::Set(TDecimal l)
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

void Universal::Set(const Universal & value, HoeCore::MemoryPool* pool)
{
	Clear();
	switch (value.type & 0x0fffffff)
	{
	case TypeString:
		Set(value.GetStringValue(), pool);
		break;
	case TypeWString:
		Set(value.GetWideStringValue(), pool);
		break;
	case TypeData:
		Set(value.GetPointer(), value.GetSize(), pool);
		break;
	default:
		this->type = value.type;
		this->size = value.size;
		this->allocated = value.allocated;
		memcpy(&this->value, &value.value, sizeof(this->value));
		break;
	};
}

void Universal::Set(void * v)
{
	Clear();
	type = (type & 0x80000000) | TypePointer;
	value.p = v;
}

void Universal::Set(const void * v, size_t s, HoeCore::MemoryPool* pool)
{
	if (GetType() == TypeData && allocated >= s && s > 0)
	{
		memcpy(this->value.p, v, s); this->size = s; return;
	}
	Clear();
	type = (type & 0x80000000) | TypeData;
	if (s > 0)
	{
		if (pool)
		{
			this->value.p = new (*pool) byte[s];
			type |= 0x40000000;
		}
		else
		{
			this->value.p = new byte[s];
		}
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
	case TypeString: return string::GetNumber(GetStringValue());
	case TypeWString: return string::GetNumber(GetWideStringValue());
	case TypeDecimal: return (unsigned long)value.l;
	case TypeUnsigned: return value.ul;
	case TypeFloat: return (unsigned long)value.f;
	case TypeBool: return (unsigned long)value.b;
	//case TypeFloatVector: return (unsigned long)vec_GetFloat(0);
	};
	return 0;
}

Universal::TDecimal Universal::GetDecimal() const
{
	switch (GetType())
	{
	case TypeString: return string::GetNumber(GetStringValue());
	case TypeWString: return string::GetNumber(GetWideStringValue());
	case TypeDecimal: return value.l;
	case TypeUnsigned: return (TDecimal)value.ul;
	case TypeFloat: return (TDecimal)value.f;
	case TypeBool: return (TDecimal)value.b;
	//case TypeFloatVector: return (unsigned long)vec_GetFloat(0);
	};
	return 0;
}

Universal::TReal Universal::GetFloat() const
{
	switch (GetType())
	{
	case TypeString: return string::GetReal(GetStringValue());
	case TypeWString: return string::GetReal(GetWideStringValue());
	case TypeDecimal: return (TReal)value.l;
	case TypeUnsigned: return (TReal)value.ul;
	case TypeFloat: return value.f;
	case TypeBool: return (TReal)(value.b ? 1:0);
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


