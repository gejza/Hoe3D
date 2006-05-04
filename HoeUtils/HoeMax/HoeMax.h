/**********************************************************************
 *<
	FILE: HoeMax.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef __HOEMAX__H
#define __HOEMAX__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define HOEMAX_CLASS_ID	Class_ID(0x7adbf172, 0x5301b161)

class Resources;
class MeshBuffer;
class MeshItem;

class HoeMax : public SceneExport {
	public:
		bool ComputeMesh(INode * node, TimeValue t, MeshBuffer & mesh);
		void ExportMesh(MeshItem * mesh);


		static HWND hParams;


		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		// Node enumeration
		BOOL	nodeEnum(INode* node, int indentLevel);
		void	PreProcess(INode* node);


		// High level export
		void	ExportGlobalInfo();
		void	ExportGeomObject(INode* node, int indentLevel);
		
		//void	ExportStream(INode* node, TimeValue t);

		void	DumpMaterial(Mtl* mtl);



		// Misc methods
		Point3	GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
		TSTR	GetIndent(int indentLevel);
		TriObject* GetTriObjectFromNode(INode * node, TimeValue t, int& deleteIt);
		BOOL	TMNegParity(Matrix3& m);

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

		//TimeValue GetStaticFrame(void) { return nStaticFrame; }
		void Printf(const char * szFormat,...);
		void Progress(int p); 

		//Constructor/Destructor

		HoeMax();
		~HoeMax();		

		
private:
		Interface*	ip;
		FILE*		pStream; 
		int			nTotalNodeCount;
		int			nCurNode;

		TCHAR		szFmtStr[16]; 

		bool		exportSelected;
		bool		showPrompts;
		//bool		bExportTV;
		//bool		bExportC;
		//bool		bExportN;


		//TimeValue	nStaticFrame;

		//MtlKeeper	mtlList;

		Resources	*rsc;

};


class HoeMaxClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new HoeMax(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return HOEMAX_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("HoeMax"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

};

DWORD WINAPI fn(LPVOID arg);

#endif // __HOEMAX__H
