
#ifndef _RESOURCES_DIALOG_H_
#define _RESOURCES_DIALOG_H_

#include <vector>

class MtlKeeper {
public:
	BOOL	AddMtl(Mtl* mtl);
	int		GetMtlID(Mtl* mtl);
	int		Count();
	Mtl*	GetMtl(int id);

	Tab<Mtl*> mtlTab;
};

class TreeItem;

class MeshItem
{
	
public:

	typedef std::vector<MeshItem*> List;

	TreeItem * item;
	INode* node;
};

class HelperItem
{
public:
	typedef std::vector<HelperItem*> List;
	TreeItem * item;
	INode* node;
};

class TreeItem
{	
	HWND hTree;
	HTREEITEM handle;
public:
	void SetChecked(bool check);
	bool IsChecked();
	enum Type
	{
		eMesh,
		eHelper,
	} type;

	union {
		MeshItem * mesh;
		HelperItem * helper;
	};
	
	TreeItem(HWND tree, HTREEITEM h);
	TreeItem(HWND tree);
	HTREEITEM Handle() { return handle; }

	friend class Resources;
};

class Resources
{
private:
	int findmat;
	int findmesh;
	int findhelper;

	HWND hWindow;
	HWND hTree;
protected:
	TreeItem * AddMeshItem(MeshItem *mesh, TreeItem * after);
	TreeItem * AddHelperItem(HelperItem *helper, TreeItem * after);
	MtlKeeper mtlList;
	MeshItem::List meshList;
	HelperItem::List helperList;

	TimeValue m_from, m_to;

	static BOOL CALLBACK HoeMaxOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	int OnInit(HWND hWnd);

	TreeItem * AddItem(TreeItem * parent,TreeItem * after,const char * caption);
public:
	MeshItem * GetNextMesh();
	HelperItem * GetNextHelper();
	Mtl * GetNextMtl();
	void StartFind();
	MeshItem::List * GetMeshs();
	MtlKeeper * GetMtls();
	void AddGeomObject(INode* node);
	void AddHelper(INode * node);
	
	void AddMtl(Mtl* mtl);
	bool Show();

	TimeValue GetFrom() { return m_from; }
	TimeValue GetTo() { return m_to; }
};

#endif // _RESOURCES_DIALOG_H_

