
#ifndef _HOE_EXPORT_UTILS_
#define _HOE_EXPORT_UTILS_ 

namespace Utils {
	// A collection of overloaded value to string converters.
	TCHAR*	FixupName(TCHAR* name);
	void	CommaScan(TCHAR* buf);
	TSTR	Format(int value);
	TSTR	Format(float value);
	TSTR	Format(Color value);
	TSTR	Format(Point3 value); 
	TSTR	Format(AngAxis value); 
	TSTR	Format(Quat value);
	TSTR	Format(ScaleValue value);
};

#endif // _HOE_EXPORT_UTILS_

