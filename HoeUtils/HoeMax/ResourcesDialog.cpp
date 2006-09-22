
#include "HoeMax.h"
#include "ResourcesDialog.h"

bool Resources::Show()
{
	if (!DialogBoxParam(hInstance, 
			MAKEINTRESOURCE(IDD_PANEL), 
			GetActiveWindow(), 
			HoeMaxOptionsDlgProc, (LPARAM)this))
			return false;

	return true;
}

BOOL CALLBACK Resources::HoeMaxOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) 
{
	static Resources *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (Resources *)lParam;
			return imp->OnInit(hWnd);

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDOK: {
				/*imp->bExportN = IsDlgButtonChecked(hWnd, IDC_NORMALS) == true;
				imp->bExportC = IsDlgButtonChecked(hWnd, IDC_VCOLORS) == true;
				imp->bExportTV = IsDlgButtonChecked(hWnd, IDC_TEXUV) == true;

				len = SendDlgItemMessage(hWnd, IDC_MODELNAME, WM_GETTEXTLENGTH, 0, 0);
                SendDlgItemMessage(hWnd, IDC_MODELNAME, WM_GETTEXT, len+1, (LPARAM)imp->modelName);
				imp->modelName[len] = '\0';*/
				// get dlgs
				char buff[100];
				int i;
				GetDlgItemText(hWnd, IDC_FRAMEFROM, buff, 100);
				sscanf(buff, "%d", &i);
				imp->m_from = i;
				GetDlgItemText(hWnd, IDC_FRAMETO, buff, 100);
				sscanf(buff, "%d", &i);
				imp->m_to = i;
				EndDialog(hWnd, 1);
				return TRUE;
			}
			break;
			case IDCANCEL:
				EndDialog(hWnd, 0);
				return TRUE;
			default:
				return FALSE;
		}

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}

int Resources::OnInit(HWND hWnd)
{
	hWindow = hWnd;
	hTree = GetDlgItem(hWnd,IDC_TREEC);
	int i;

	//SendDlgItemMessage(hWnd,IDC_MODELNAME, WM_SETTEXT, 0, (LPARAM)imp->modelName);
	CenterWindow(hWnd,GetParent(hWnd));
	
	TreeItem * item = NULL;
	for (i=0;i <mtlList.Count();i++)
	{
		item = AddItem(NULL,item,mtlList.GetMtl(i)->GetName());
	}

	for (i=0;i <meshList.size();i++)
	{
		item = AddMeshItem(meshList.at(i),item);
		//item->SetChecked(true);
	}

	for (i=0;i <helperList.size();i++)
	{
		item = AddHelperItem(helperList.at(i),item);
		//item->SetChecked(true);
	}

      SetDlgItemText(hWnd, IDC_FRAMEFROM, "0");
      SetDlgItemText(hWnd, IDC_FRAMETO, "0");


	return TRUE;
}

TreeItem * Resources::AddItem(TreeItem * parent,TreeItem * after,const char * caption)
{
	TVITEM tvi; 
    TVINSERTSTRUCT tvins;
	TreeItem * item = new TreeItem(hTree);

	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
	tvi.pszText = (char*)caption; 
    tvi.cchTextMax = lstrlen(caption);
	tvi.lParam = (LPARAM) item;
    tvi.stateMask = TVIS_STATEIMAGEMASK;
    tvi.state = INDEXTOSTATEIMAGEMASK(2);

    //Since state images are one-based, 1 in this macro turns the check off, and 
    //2 turns it on.


	tvins.item = tvi; 
    tvins.hInsertAfter = (after != NULL) ? after->Handle():(HTREEITEM) TVI_FIRST;
	tvins.hParent = (parent != NULL) ? parent->Handle():TVI_ROOT;
	
	item->handle = TreeView_InsertItem(hTree, &tvins);

	return item;
}

void Resources::AddMtl(Mtl *mtl)
{
	mtlList.AddMtl(mtl);
}

void Resources::AddGeomObject(INode *node)
{
	ObjectState os = node->EvalWorldState(0);
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return; // Safety net. This shouldn't happen.
	}

	MeshItem * mesh = new MeshItem;
	mesh->node = node;

	meshList.push_back(mesh);
}

void Resources::AddHelper(INode * node)
{
	ObjectState os = node->EvalWorldState(0);
	if (!os.obj || os.obj->SuperClassID()!=HELPER_CLASS_ID) {
		return; // Safety net. This shouldn't happen.
	}

	if (os.obj->ClassID() != Class_ID(POINTHELP_CLASS_ID,0)) {
		return;
	}

	HelperItem * helper = new HelperItem;
	helper->node = node;

	helperList.push_back(helper);
}

TreeItem * Resources::AddMeshItem(MeshItem *mesh, TreeItem * after)
{
	TreeItem * item = AddItem(NULL,after,mesh->node->GetName());
	item->type = TreeItem::eMesh;
	item->mesh = mesh;
	mesh->item = item;

	TreeItem * stream = AddItem(item,NULL,"Stream");
	TreeItem * it;
	it = AddItem(stream,NULL,"Normals");
	it = AddItem(stream,it,"Textures ");
	it = AddItem(stream,it,"Vertex Colors");

	AddItem(item,stream,"Index");

	return item;	
}

TreeItem * Resources::AddHelperItem(HelperItem *helper, TreeItem * after)
{
	TreeItem * item = AddItem(NULL,after,helper->node->GetName());
	item->type = TreeItem::eHelper;
	item->helper = helper;
	helper->item = item;

	return item;	
}

TreeItem::TreeItem(HWND tree)
{
	this->hTree = tree;
	this->handle = NULL;
}

TreeItem::TreeItem(HWND tree, HTREEITEM h)
{
	this->hTree = tree;
	this->handle = h;
}

bool TreeItem::IsChecked()
{
    TVITEM tvItem;

    // Prepare to receive the desired information.
    tvItem.mask = TVIF_HANDLE | TVIF_STATE;
    tvItem.hItem = handle;
    tvItem.stateMask = TVIS_STATEIMAGEMASK;

    // Request the information.
    TreeView_GetItem(hTree, &tvItem);

    // Return zero if it's not checked, or nonzero otherwise.
    return (((BOOL)(tvItem.state >> 12) -1) != FALSE);
}

void TreeItem::SetChecked(bool check)
{
    TVITEM tvItem;

    tvItem.mask = TVIF_HANDLE | TVIF_STATE;
    tvItem.hItem = handle;
    tvItem.stateMask = TVIS_STATEIMAGEMASK;

    //Since state images are one-based, 1 in this macro turns the check off, and 
    //2 turns it on.

    tvItem.state = INDEXTOSTATEIMAGEMASK((check ? 2 : 1));

    TreeView_SetItem(hTree, &tvItem);
}

MtlKeeper * Resources::GetMtls()
{
	return &mtlList;
}

MeshItem::List * Resources::GetMeshs()
{
	return &meshList;
}

void Resources::StartFind()
{
	findmat = 0;
	findmesh = 0;
	findhelper = 0;
}

Mtl * Resources::GetNextMtl()
{
	if (this->findmat < this->mtlList.Count())
	{
		return this->mtlList.GetMtl(this->findmat++);
	}
	else
	{
		return NULL;
	}
}

MeshItem * Resources::GetNextMesh()
{
	if (this->findmesh < this->meshList.size())
	{
		return this->meshList.at(this->findmesh++);
	}
	else
	{
		return NULL;
	}
}

HelperItem * Resources::GetNextHelper()
{
	if (this->findhelper < this->helperList.size())
	{
		return this->helperList.at(this->findhelper++);
	}
	else
	{
		return NULL;
	}
}

////////  mtl keeper

BOOL MtlKeeper::AddMtl(Mtl* mtl)
{
	if (!mtl) {
		return FALSE;
	}

	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return FALSE;
		}
	}
	mtlTab.Append(1, &mtl, 25);

	return TRUE;
}

int MtlKeeper::GetMtlID(Mtl* mtl)
{
	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++) {
		if (mtlTab[i] == mtl) {
			return i;
		}
	}
	return -1;
}

int MtlKeeper::Count()
{
	return mtlTab.Count();
}

Mtl* MtlKeeper::GetMtl(int id)
{
	return mtlTab[id];
}
