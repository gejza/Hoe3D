
#include "Max.h"
//#include "resource.h"
//#include "istdplug.h"
//#include "iparamb2.h"
//#include "iparamm2.h" 
#include "Utils.h"

const char * szFmtStr = "%f";

/****************************************************************************

  String manipulation functions
  
****************************************************************************/

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

// Replace some characters we don't care for.
TCHAR* Utils::FixupName(TCHAR* name)
{
	static char buffer[256];
	TCHAR* cPtr;
	
	if (*name >= '0' && *name <= '9')
	{
		*buffer = '_';
		_tcscpy(buffer + 1, name);
	}
	else
	{
		_tcscpy(buffer, name);
	}
	
	cPtr = buffer;
	
    while(*cPtr) {
		if (*cPtr == ' ')
			*cPtr = '_';
		else if (*cPtr == '"')
			*cPtr = SINGLE_QUOTE;
        else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
        cPtr++;
    }
	
	return buffer;
}

// International settings in Windows could cause a number to be written
// with a "," instead of a ".".
// To compensate for this we need to convert all , to . in order to make the
// format consistent.
void Utils::CommaScan(TCHAR* buf)
{
    for(; *buf; buf++) if (*buf == ',') *buf = '.';
}

TSTR Utils::Format(int value)
{
	TCHAR buf[50];
	
	sprintf(buf, _T("%d"), value);
	return buf;
}


TSTR Utils::Format(float value)
{
	TCHAR buf[40];
	
	sprintf(buf, "%f", value);
	CommaScan(buf);
	return TSTR(buf);
}

TSTR Utils::Format(Point3 value)
{
	TCHAR buf[120];
	TCHAR fmt[120];
	
	sprintf(fmt, "%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf, fmt, value.x, value.y, value.z);

	CommaScan(buf);
	return buf;
}

TSTR Utils::Format(Color value)
{
	TCHAR buf[160];
	//TCHAR fmt[160];
	
	//sprintf(fmt, "%s\t%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf,"{ %f, %f, %f}", value.r, value.g, value.b);

	//CommaScan(buf);
	return buf;

}

TSTR Utils::Format(AngAxis value)
{
	TCHAR buf[160];
	TCHAR fmt[160];
	
	sprintf(fmt, "%s\t%s\t%s\t%s", szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	sprintf(buf, fmt, value.axis.x, value.axis.y, value.axis.z, value.angle);

	CommaScan(buf);
	return buf;
}


TSTR Utils::Format(Quat value)
{
	// A Quat is converted to an AngAxis before output.
	
	Point3 axis;
	float angle;
	AngAxisFromQ(value, &angle, axis);
	
	return Format(AngAxis(axis, angle));
}

TSTR Utils::Format(ScaleValue value)
{
	TCHAR buf[280];
	
	sprintf(buf, "%s %s", Format(value.s), Format(value.q));
	CommaScan(buf);
	return buf;
}
