/**********************************************************************
 *<
	FILE: HoeMax.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "HoeMax.h"
#include "ResourcesDialog.h"
#include "Utils.h"
#include "MeshBuffer.h"

using namespace Utils;

static HoeMaxClassDesc HoeMaxDesc;
ClassDesc2* GetHoeMaxDesc() { return &HoeMaxDesc; }


//--- HoeMax -------------------------------------------------------
HoeMax::HoeMax()
{

}

HoeMax::~HoeMax() 
{

}

int HoeMax::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *HoeMax::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("TXT");
}

const TCHAR *HoeMax::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("Hoe text model file");
}
	
const TCHAR *HoeMax::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("Hoe model");
}

const TCHAR *HoeMax::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Gejza'x");
}

const TCHAR *HoeMax::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *HoeMax::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *HoeMax::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int HoeMax::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 200;
}

void HoeMax::ShowAbout(HWND hWnd)
{			
	MessageBox(hWnd, TEXT("Hoe export model by Gejza'x"),TEXT("Hoe model"),MB_OK);
}

BOOL HoeMax::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

// Dummy function for progress bar
DWORD WINAPI fn(LPVOID arg)
{
	return(0);
} 

#ifndef _HOE_MAX_LOADER

int	HoeMax::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	// Set a global prompt display switch
	showPrompts = suppressPrompts ? FALSE : TRUE;
	exportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE; 
	//nStaticFrame = 0;
	ip = i;
	rsc = new Resources;

	PreProcess(ip->GetRootNode());

	if(!suppressPrompts)
	{
		if (rsc->Show() == false)
		{
			delete rsc;
			return TRUE;
		}
	}

	// Open the stream
	if (!m_file.Open(name)) 
	{
		return 0;
	}
	
	// Startup the progress bar.
	ip->ProgressStart(GetString(IDS_PROGRESS_MSG), TRUE, fn, NULL);

	// First we write out a file header with global information. 
	ExportGlobalInfo();

	nTotalNodeCount = 0;
	nCurNode = 0;

	Mtl * mtl;
	MeshItem * mesh;
	HelperItem * helper;

	rsc->StartFind();
	while ((mtl = rsc->GetNextMtl()) != NULL)
	{
		nTotalNodeCount++;
	}

	
	while ((mesh = rsc->GetNextMesh()) != NULL)
	{
		nTotalNodeCount++;
	}

	rsc->StartFind();
	while ((mtl = rsc->GetNextMtl()) != NULL)
	{
		DumpMaterial(mtl);
		nCurNode++;
	}

	
	while ((mesh = rsc->GetNextMesh()) != NULL)
	{
		ExportMesh(mesh);
	}

	/*while ((helper = rsc->GetNextHelper()) != NULL)
	{
		//ExportMesh(mesh);
		helper->node->
	}	*/

	// We're done. Finish the progress bar.
	ip->ProgressEnd();

	// Close the stream
	m_file.Close();

	delete rsc;

	return TRUE;
}

#else

typedef void * (*GETHOEMAX)();
typedef void (*DELETEHOE)(void *);

int	HoeMax::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	HMODULE hHoeMaxLib = NULL;
	int ret;
	// get lib
	hHoeMaxLib = LoadLibrary("c:/3dsmax6/plugins/HoeMax_devel.dll");
	if (!hHoeMaxLib)
	{
		TCHAR szBuf[80]; 
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError(); 

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		wsprintf(szBuf, 
			"LoadLibrary failed with error %d: %s", dw, lpMsgBuf); 
 
		MessageBox(NULL, szBuf, "Error", MB_OK); 

		LocalFree(lpMsgBuf);
		return FALSE;
	}
	// get interface
	GETHOEMAX gethoe = (GETHOEMAX)GetProcAddress(hHoeMaxLib,"GetHoeMax");
	if (!gethoe)
	{
		MessageBox(NULL,"(GETHOEMAX)GetProcAddress(hHoeMaxLib,\"GetHoeMax\");","Failed",MB_OK);
		return FALSE;
	}
	// run export
	SceneExport * se = (SceneExport *)gethoe();
	if (!se)
	{
		MessageBox(NULL,"(SceneExport *)gethoe();","Failed",MB_OK);
		return FALSE;
	}

	ret = se->DoExport(name,ei,i,suppressPrompts,options);
	// delete hoe
	DELETEHOE delhoe = (DELETEHOE)GetProcAddress(hHoeMaxLib,"DeleteHoeMax");
	if (!delhoe)
	{
		MessageBox(NULL,"(DELETEHOE)GetProcAddress(hHoeMaxLib,\"DeleteHoeMax\");","Failed",MB_OK);
		return FALSE;
	}
	

	delhoe(se);
	// unload lib
	FreeLibrary(hHoeMaxLib);

	return ret;
}

#endif

void HoeMax::PreProcess(INode* node)
{
	if(exportSelected && node->Selected() == FALSE)
		return;

	// Add the nodes material to out material list
	// Null entries are ignored when added...
	rsc->AddMtl(node->GetMtl());

	// The ObjectState is a 'thing' that flows down the pipeline containing
	// all information about the object. By calling EvalWorldState() we tell
	// max to eveluate the object at end of the pipeline.
	ObjectState os = node->EvalWorldState(0); 

	// The obj member of ObjectState is the actual object we will export.
	if (os.obj && os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID && os.obj->ClassID() != Class_ID(TARGET_CLASS_ID, 0)) 
	{

		rsc->AddGeomObject(node);
	}
	else if (os.obj && os.obj->SuperClassID() == HELPER_CLASS_ID)
	{
		rsc->AddHelper(node);
	}

	// For each child of this node, we recurse into ourselves 
	// and increment the counter until no more children are found.
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		PreProcess(node->GetChildNode(c));
	}
}

/****************************************************************************

  Global output [Scene info]
  
****************************************************************************/

// Dump some global animation information.
void HoeMax::ExportGlobalInfo()
{
	struct tm *newtime;
	time_t aclock;

	time( &aclock );
	newtime = localtime(&aclock);

	TSTR today = _tasctime(newtime);	// The date string has a \n appended.
	today.remove(today.length()-1);		// Remove the \n

	// Start with a file identifier and format version
	m_file.Printf( "// %s\t%.2f\n", this->LongDesc(), this->Version()*0.01f);
	m_file.Printf( "// %s\t%s\n", this->AuthorName(), this->CopyrightMessage());
	m_file.Printf( "// \"%s\" - %s\n", Utils::FixupName(ip->GetCurFileName()), (TCHAR*)today);
	m_file.Printf( "\n");
}

void HoeMax::ExportMesh(MeshItem *mesh)
{
	char progressmsg[512];
	sprintf(progressmsg,"Export node '%s' (%d/%d)",mesh->node->GetName(),nCurNode,nTotalNodeCount);
	ip->ProgressStart(progressmsg, TRUE, fn, NULL);
	nCurNode++;

	MeshBuffer buffer(true, false, true);
	buffer.Export(FixupName(mesh->node->GetName()),mesh->node, 0, 29, &m_file);
}


void HoeMax::DumpMaterial(Mtl* mtl)
{

	TimeValue t = 0;//GetStaticFrame();
	
	if (!mtl) return;

	TSTR className;
	mtl->GetClassName(className);
	m_file.Printf("// Mat: %s\n",FixupName(className));

	m_file.Printf("Material %s\n",FixupName(mtl->GetName()));

	// We know the Standard material, so we can get some extra info
	//if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
	//	StdMat* std = (StdMat*)mtl;

		m_file.Printf("\tDiffuse = %s\n", Format(mtl->GetDiffuse(t)));
		m_file.Printf("\tAmbient = %s\n", Format(mtl->GetAmbient(t)));
		m_file.Printf("\tSpecular = %s\n", Format(mtl->GetSpecular(t)));


	m_file.Printf("~Material\n\n");
}

void HoeMax::Progress(int p)
{ 
	ip->ProgressUpdate(p);	
}



