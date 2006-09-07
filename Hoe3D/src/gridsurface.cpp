
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "gridsurface.h"
#include "texture_system.h"
#include "hoe_texture.h"
#include "hoe_time.h"
#include "model_loader.h"
#include "hoe_model.h"

static CVar v_optimize("gridsurface_optimize", true, 0);

struct VecPDT
{
		HoeMath::VECTOR3 pos;
		HoeMath::VECTOR3 norm;
		dword color;
		HoeMath::VECTOR2 tex1;
		HoeMath::VECTOR2 tex2;
};

struct ModStr
{
		HoeMath::VECTOR3 pos;
		HoeMath::VECTOR3 norm;
		HoeMath::VECTOR2 tex1;
};

// D3DTOP_BLENDTEXTUREALPHA current textura -- alpha current
// rozdelit to na skupiny mrizek
// kazda skupina mrizek svoje rozdeleni podle viditelnosti
// vykresleni podle povrchu (kazdy povrch, svoje quadtree)
// 

inline bool Tol(float a, float b)
{
	return abs(a-b) < 2.f;
}

bool TGridModel::LoadModel(const char * name)
{
	if (name)
	{
		Con_Print("Loading gridsurface model %s",name);
		ModelLoader ml(0, 0);
		mod = ml.LoadModel(name, true);
		// update radku atd
		HoeStream & s = *mod->m_stream[0];
		ModStr * m = (ModStr *)s.Lock();
		for (uint i=0;i < s.GetNumVert();i++)
		{
			m[i].pos.y *= 1.8f;
			if (Tol(m[i].pos.x, -10.f) && Tol(m[i].pos.z, -10.f))
			{
				m[i].pos.x = -10.f; m[i].pos.z = -10.f;this->coigns[0] = m[i].pos.y;
			}
			if (Tol(m[i].pos.x, 10.f) && Tol(m[i].pos.z, -10.f))
			{
				m[i].pos.x = 10.f; m[i].pos.z = -10.f;this->coigns[1] = m[i].pos.y;
			}
			if (Tol(m[i].pos.x, -10.f) && Tol(m[i].pos.z, 10.f))
			{
				m[i].pos.x = -10.f; m[i].pos.z = 10.f;this->coigns[2] = m[i].pos.y;
			}
			if (Tol(m[i].pos.x, 10.f) && Tol(m[i].pos.z, 10.f))
			{
				m[i].pos.x = 10.f; m[i].pos.z = 10.f;this->coigns[3] = m[i].pos.y;
			}
		}
		s.Unlock();
	}
	else
	{
		mod = NULL; /*!!!*/
	}

	return true;
}

//////////////////////////////////////////////////////////
// GridSurfaceType
void TGridSurfaceType::Setup()
{
#ifdef _HOE_D3D_

		D3DDevice()->SetTexture(0, tex1->GetTexture());
		if (tex2)
			D3DDevice()->SetTexture(1, tex2->GetTexture());
		else
			D3DDevice()->SetTexture(1, NULL);
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE); 
		if (tex2)
		{
			D3DDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA); 
			D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
			D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		}
		else
		{
			D3DDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE); 
		}
		//D3DDevice()->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
#if 0
		D3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
#endif

#endif
#ifdef _HOE_OPENGL_
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex1->GetTexture());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		if (tex2)
		{
		glEnable(GL_TEXTURE_2D);

		// Here we turn on the COMBINE properties and increase our RGB
		// gamma for the detail texture. 2 seems to work just right.
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,	GL_INTERPOLATE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB_ARB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

		glCullFace(GL_FRONT);
		glDisable(GL_CULL_FACE);

		// Bind the detail texture
		glBindTexture(GL_TEXTURE_2D, tex2->GetTexture());

		}
		else
			glDisable(GL_TEXTURE_2D);

		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);// Zapne alfa testing
		checkgl("multitexture");
#endif
}

//////////////////////////////////////////////////////////
void TGridSurfaceTreeItem::DeleteSub()
{
	if (q1)
	{
		q1->DeleteSub();
		delete q1;
		q1 = NULL;
	}
	if (q2)
	{
		q2->DeleteSub();
		delete q2;
		q2 = NULL;
	}
	if (q3)
	{
		q3->DeleteSub();
		delete q3;
		q3 = NULL;
	}
	if (q4)
	{
		q4->DeleteSub();
		delete q4;
		q4 = NULL;
	}
}


//////////////////////////////////////////////////////////
GridSurface::GridSurface()
{
	m_worldpos.Identity();
	m_loaded = false;
	m_sizeX = m_sizeY = 10.f;
	m_grids = NULL;
	m_gst_first = NULL;
	m_wire = false;
	memset(m_textures, 0, sizeof m_textures);
	memset(m_models, 0, sizeof(m_models));
}

GridSurface::~GridSurface()
{
	ReleaseData();
}

bool GridSurface::PlaneToMulti(float vx, float vy, const HoeMath::MATRIX & matrix, const TGridData & grid)
{
	const float tx1 = (grid.tex1 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex1].nx;
	const float ty1 = (grid.tex1 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex1].ny;
	const float tx2 = (grid.tex2 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex2].nx;
	const float ty2 = (grid.tex2 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex2].ny;

	// ulozit grid
	VecPDT * pv = (VecPDT*)m_multi.LockNewVertices(4, 6);

	const HoeMath::VECTOR2 s[4] = { HoeMath::VECTOR2(0,1/4.f),
									HoeMath::VECTOR2(1/8.f,1/4.f),
									HoeMath::VECTOR2(0,0),
									HoeMath::VECTOR2(1/8.f,0)};
	vx /= 2;
	vy /= 2;
	pv[0].pos = HoeMath::VECTOR3(-vx, grid.plane_heights[0], -vy).Multiply(matrix);
	pv[1].pos = HoeMath::VECTOR3(+vx, grid.plane_heights[1], -vy).Multiply(matrix);
	pv[2].pos = HoeMath::VECTOR3(+vx, grid.plane_heights[3], +vy).Multiply(matrix);
	pv[3].pos = HoeMath::VECTOR3(-vx, grid.plane_heights[2], +vy).Multiply(matrix);
	pv[0].norm = pv[1].norm = pv[2].norm = pv[3].norm = HoeMath::VECTOR3(0,1,0);
	pv[0].color = 0xffffff00;
	pv[1].color = 0xffffff00;
	pv[2].color = 0xffffff00;
	pv[3].color = 0xffffff00;
	pv[0].tex1 = HoeMath::VECTOR2(grid.x1*tx1,(grid.y1+1)*ty1);
	pv[1].tex1 = HoeMath::VECTOR2((grid.x1+1)*tx1,(grid.y1+1)*ty1);
	pv[2].tex1 = HoeMath::VECTOR2((grid.x1+1)*tx1,grid.y1*ty1);
	pv[3].tex1 = HoeMath::VECTOR2(grid.x1*tx1,grid.y1*ty1);
	pv[0].tex2 = HoeMath::VECTOR2(grid.x2*tx2,(grid.y2+1)*ty2);
	pv[1].tex2 = HoeMath::VECTOR2((grid.x2+1)*tx2,(grid.y2+1)*ty2);
	pv[2].tex2 = HoeMath::VECTOR2((grid.x2+1)*tx2,grid.y2*ty2);

	pv[3].tex2 = HoeMath::VECTOR2(grid.x2*tx2,grid.y2*ty2);

	/*for (int i=0;i < 4;i++) {
	// levo nahore
		// orientace
		// vypocitat souradnice podle map
		pv->tex1 = s[(grid.ori1+i)%4] + HoeMath::VECTOR2(grid.x1*tx1,grid.y1*ty1);
		pv->tex2 = s[(grid.ori2+i)%4] + HoeMath::VECTOR2(grid.x2*tx2,grid.y2*ty2);
		pv++;
	}*/

	m_multi.AddIndex(0);
	m_multi.AddIndex(3);
	m_multi.AddIndex(1);
	m_multi.AddIndex(1);
	m_multi.AddIndex(3);
	m_multi.AddIndex(2);
	/*const float px = x*m_sizeX/hx-m_sizeX/2;
	const float py = y*m_sizeY/hy-m_sizeY/2;
	const float l = sqrtf((0-px)*(0-px)+(0-py)*(0-py));
	pv->pos = HoeMath::VECTOR3(px,m_heights.getHeightAt(x,y),py);
	pv->tex = HoeMath::VECTOR2((x % 2) * (1/8.f) + (7/8.f),(y % 2) * (1/4.f) + (2/4.f));

	// compute color
	//if (l > 100.f)
		pv->color = 0xffffffff;
	//else
	//{
	//	byte c = byte(0xff * l/100.f);
	//	pv->color = (c << 8 | c) | 0xffff0000;
	//}
	pv++;
	}*/
	return true;
}

bool GridSurface::ModelToMulti(const HoeMath::MATRIX & matrix, const TGridData & grid)
{
	const float tx1 = (grid.tex1 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex1].nx;
	const float ty1 = (grid.tex1 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex1].ny;
	const float tx2 = (grid.tex2 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex2].nx;
	const float ty2 = (grid.tex2 == 0xff) ? 1/4.f:1.f/m_textures[grid.tex2].ny;

	// vertex
	assert(m_models[grid.modelid].mod);
	HoeModel * m = m_models[grid.modelid].mod;
	HoeStream * str = m->m_stream[0];
	HoeIndex * ind = m->m_index[0];
	VecPDT * pv = (VecPDT*)m_multi.LockNewVertices(str->GetNumVert(), ind->GetNumIndices());
	
	ModStr * mv = (ModStr *)str->Lock();
	for (dword i=0;i<str->GetNumVert();i++)
	{
		HoeMath::VECTOR3 pos = mv[i].pos;
		pv[i].pos = pos.Multiply(matrix);
		pv[i].color = 0xffffff00;
		pv[i].norm = mv[i].norm;
		// upravit tex
		HoeMath::VECTOR2 tex = mv[i].tex1;
		pv[i].tex2.x = (grid.x2+tex.x)*tx2;
		pv[i].tex2.y = (grid.y2+tex.y)*ty2;
		// tex 1 podle stareho pravidla
		pv[i].tex1.x = (grid.x1+((pos.x + 10.f) / 20.f))*tx1;
		pv[i].tex1.y = (grid.y1+((pos.z + 10.f) / 20.f))*ty1;
	}
	str->Unlock();

	word * w = ind->Lock();
	for (dword i=0;i<ind->GetNumIndices();i++)
	{
		m_multi.AddIndex(w[i]);		
	}
	ind->Unlock();

	return true;
}

TGridSurfaceTreeItem * GridSurface::CreateQuadTree(dword * gr, uint ngr, uint minx, uint maxx, uint miny, uint maxy)
{
	// podle v_optimize
	// najdrive rozdelit na kvadranty
	// spocitat v kterem kvadrantu je kolik ctvercu
	// pustit subtree na kazdy vyznamny kvadrant
	// mel by dostat strukturu informaci
	// minimalni vyska kvadrantu
	// velikost kvadrantu
	// 
	// nakonec spocitat sam sebe
	//if (maxx-minx < neco && maxy-miny  < neco)
	{
		dword start = m_multi.GetPos();
		// pridat gridy a pocitat velikost
		// preneseni gridu do streamu
		// nalezeni vsech pusobnych gridu
		for (uint i=0;i < ngr;i++)
		{
			const uint x = hiword(gr[i]);
			const uint y = loword(gr[i]);
			if (x < minx || x > maxx || y < miny || y > maxy)
				continue;
			HoeMath::MATRIX mat;
			const float vx = m_sizeX / m_width;
			const float vy = m_sizeY / m_height;
			mat.Translate((x * vx) - (m_sizeX*0.5f) + (vx * 0.5f),(m_grids[y*m_width+x].type == TGridData::eModel) ? m_grids[y*m_width+x].base_height:0, // tady bude height
				(y * vy) - (m_sizeY*0.5f) + (vx * 0.5f));

			switch (m_grids[y*m_width+x].type)
			{
			case TGridData::ePlane:
				PlaneToMulti(vx, vy, mat, m_grids[y*m_width+x]);
				break;
			case TGridData::eModel:
				ModelToMulti(mat, m_grids[y*m_width+x]);
				break;
			default:
				assert(!"Unknown grid type.");
			};
		}

		dword end = m_multi.GetPos();
		if (end-start == 0)
			return NULL;
		// posledni hotovy
		TGridSurfaceTreeItem * i = new TGridSurfaceTreeItem();
		i->q1 = i->q2 = i->q3 = i->q4 = NULL;
		// pridat vsechny vrcholy 
		i->start = start;
		i->end = end;
		// pridat velikost
		return i;
	}

	// nastavit vsechny na grid

	// pokud je jen jeden tak vratit ten jeden
	// pokud je vice, vytvorit strom

	// spocitat udaje

	return NULL;
}


void GridSurface::Load()
{
	if (!m_grids)
		return;

	// odstraneni predesleho
	Unload();
	// podle urovne rozdeleni do skupin (meni se jen jedna skupina),
	// zase na druhou stranu pomalejsi render (pro editor)

	// na zacatku grid a nekolik textur
	// predelat do streamu, indexu a matrose
	// postupne vyplnovat pole gridu, dokud nebude cele predelane
	// vytvoreni pole gridu
	bool * pg = new bool[m_width*m_height];
	memset(pg, 0,m_width*m_height * sizeof(byte));

	dword * gl = new dword[m_width*m_height];
	// multistream create
	// dobre by bylo spocitat kolik budevrcholu (to je mozne, protoze se tam musi premistit vsichny)
	if (!m_multi.Begin("pndtt", sizeof(VecPDT)))
		return;

	for (uint y=0;y < m_height;y++)
	{
		for (uint x=0;x < m_width;x++)
		{
			if (pg[y*m_width+x])
				continue; // preskoceni zpracovanych

			// nalezeni prvniho gridu a sestaveni surface
			TGridDesc * type = &m_grids[y*m_width+x];
			TGridSurfaceType * nt = new TGridSurfaceType();
			
			// nastaveni textur
			nt->tex1 = m_textures[type->tex1].tex;
			//GetTextureSystem()->GetTexture("trava");
			nt->tex2 = type->tex2 == 0xff ? NULL:m_textures[type->tex2].tex;
			//GetTextureSystem()->GetTexture("strom_war3");
			assert(nt->tex1 || nt->tex2);


			uint numgrids = 1;
			uint maxx,maxy,minx,miny;
			maxx = minx = x;
			maxy = miny = y;
			gl[0] = makedword(x,y);
			pg[y*m_width+x] = true;
			// pridani prvniho
			// hledani stejnych poli
			for (uint yy=y;yy < m_height;yy++)
			{
				for (uint xx=0;xx < m_width;xx++)
				{
					if (!pg[yy*m_width+xx] && m_grids[yy*m_width+xx].tex1 == type->tex1
						&& m_grids[yy*m_width+xx].tex2 == type->tex2)
					{
						// pokud stejne tak pridat
						gl[numgrids] = makedword(xx,yy);
						pg[yy*m_width+xx] = true;
						if (xx < minx) minx = xx;
						if (xx > maxx) maxx = xx;
						if (yy < miny) miny = yy;
						if (yy > maxy) maxy = yy;
						numgrids++;
					}
				}
			}
			// sestaveni opt. stromu
			// rozdeleni na oblasti
			// vyteckovat
			// kazdy grid se nakopiruje do streamu (neresit zda jsou vedle sebe, jen uvnitr gridu)
			nt->root = CreateQuadTree(gl, numgrids, minx, maxx, miny, maxy);
			// pridani do seznamu povrchu
			TGridSurfaceType ** nt2 = &m_gst_first;
			while (*nt2) nt2 = &((*nt2)->next);
			*nt2 = nt;
			nt->next = NULL;

		}
	}

	m_multi.End();

	delete [] pg;
	delete [] gl;
	m_loaded = true;

}

void HOEAPI GridSurface::ReleaseData()
{
	// projit a vymazat heightmapy
	

	SAFE_DELETE_ARRAY(m_grids);
}

void GridSurface::Unload()
{
	TGridSurfaceType * gst = m_gst_first;
	while (gst)
	{
		TGridSurfaceType * next = gst->next;
		if (gst->root)
		{
			gst->root->DeleteSub();
			delete gst->root;
		}
		delete gst;
		gst = next;
	}
	m_gst_first = NULL;
	m_loaded = false;
}

void GridSurface::Render()
{
	// vykreslovat jenom celky v polickach

	if (m_loaded && m_gst_first)
	{
		/*static TimeMeter tm("Load terrain",true); 
		tm.Begin();
		Load();
		tm.End();*/
		// wireframe
		Ref::SetMatrix(m_worldpos);
		GetStates()->SetupMap();
		// nastavit matros
		HoeMaterial mat;
		mat.SetColor(HoeMaterial::Diffuse, HoeMaterialColor(1,1,1,1));
		mat.Setup();
		// enable textures
		GetStates()->EnableTexture();

		if (m_wire)
			GetStates()->StartWireframe();
		//m_mat.Setup();

		// toto by mohlo byt i v predrenderingu
		TGridSurfaceType * gst = m_gst_first;
		while (gst)
		{
			gst->Setup();
			// renderovani multistreamu pres grid tree
			assert(gst->root);
			m_multi.Render(gst->root->start,gst->root->end);
			gst = gst->next;
		}
#ifdef _HOE_OPENGL_
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
#endif
		// wireframe
		if (m_wire)
			GetStates()->EndWireframe();
	}
}

void HOEAPI GridSurface::SetPosCenter(float x, float y, float z)
{
	m_worldpos.Translate(x,z,y);
}

void HOEAPI GridSurface::SetTexture(int slot, const char *texname, int width, int height)
{
	assert(slot >= 0 && slot < MaxTextureSlots);
	if (texname)
	{
		m_textures[slot].tex = GetTextureSystem()->GetTexture(texname);
		assert(width > 0 && height > 0);
		m_textures[slot].nx = width;
		m_textures[slot].ny = height;
	}
	else
	{
		m_textures[slot].tex = NULL;
	}
}

void HOEAPI GridSurface::SetModel(int slot, const char *modname)
{
	assert(slot >= 0 && slot < MaxTextureSlots);
	m_models[slot].LoadModel(modname);
}

void HOEAPI GridSurface::Create(float sizeX, float sizeY, int resX,int resY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	//m_heights.generatePlaneMap( resX, resY, 0.f);
	ReleaseData();
	m_width = (size_t)resX;
	m_height = (size_t)resY;
	m_grids = new TGridData[m_width*m_height];
	for (size_t i=0;i < m_width * m_height;i++)
	{
		memset(&m_grids[i], 0, sizeof(TGridData));
		m_grids[i].tex1 = 0;
		m_grids[i].tex2 = 1;
		m_grids[i].x2 = rand() % 8;
		m_grids[i].y2 = rand() % 4;

	}
	Load();
}

void HOEAPI GridSurface::GetDesc(float *sizeX, float *sizeY, uint *resX,uint *resY)
{
	if (sizeX) *sizeX = m_sizeX;
	if (sizeY) *sizeY = m_sizeY;
	if (resX) *resX = m_width;
	if (resY) *resY = m_height;
}

void HOEAPI GridSurface::SetGridDesc(int x, int y, IHoeEnv::GridSurface::TGridDesc *desc)
{
	assert(m_grids);
	assert(x >= 0 && (uint)x < m_width);
	assert(y >= 0 && (uint)y < m_height);
	memcpy(&m_grids[m_width*y+x], desc, sizeof(IHoeEnv::GridSurface::TGridDesc));
}

void HOEAPI GridSurface::GetGridDesc(int x, int y, IHoeEnv::GridSurface::TGridDesc *desc)
{
	assert(m_grids);
	assert(x >= 0 && (uint)x < m_width);
	assert(y >= 0 && (uint)y < m_height);
	memcpy(desc, &m_grids[m_width*y+x], sizeof(IHoeEnv::GridSurface::TGridDesc));

}

void HOEAPI GridSurface::SetGridModel(int x, int y, float height, int modelid)
{
	// prenastaveni v gridu na model, pokud height mapa tak odstranit
	if (m_grids[m_width*y+x].type == TGridData::eModel)
	{
		m_grids[m_width*y+x].modelid = (m_grids[m_width*y+x].modelid + 1) % 12;
	}
	else
	{
		m_grids[m_width*y+x].type = TGridData::eModel;
		m_grids[m_width*y+x].base_height = height;
		m_grids[m_width*y+x].modelid = 0;
	}
	Opt_ProcessPlanes(x-1,x+1,y-1,y+1);
}

void HOEAPI GridSurface::SetGridPlane(int x, int y, float height,float lt, float rt, float lb, float rb)
{
	// jestli puvodne heightmapa tak odstranit
	m_grids[m_width*y+x].type = TGridData::ePlane;
	m_grids[m_width*y+x].plane_heights[0] = lt + height;
	m_grids[m_width*y+x].plane_heights[1] = rt + height;
	m_grids[m_width*y+x].plane_heights[2] = lb + height;
	m_grids[m_width*y+x].plane_heights[3] = rb + height;
}

void HOEAPI GridSurface::SetGridHeightmap(int x, int y, float height, int resx, int resy, float * h)
{
}

void HOEAPI GridSurface::MoveHeight(float x, float y, float moveheight, float radius)
{
	// hybnuti s mapou na spravnem miste
	// spocitani mista? vymezit okruh pro pocitani
	// 
	const float distX = m_sizeX / m_width;
	const float distY = m_sizeY / m_height;
	const uint sx = (uint)((x-radius+(m_sizeX*0.5f))/(distX));
	const uint sy = (uint)((y-radius+(m_sizeY*0.5f))/(distY));
	const uint ex = (uint)((x+radius+(m_sizeX*0.5f))/(distX));
	const uint ey = (uint)((y+radius+(m_sizeY*0.5f))/(distY));

	for (uint i=sx;i<=ex;i++)
		for (uint j=sy;j<=ey;j++)
		{
			if (i<0 || i>=m_width || j<0 || j>=m_height)
				continue;
			TGridData & g = m_grids[m_width*j+i];
			if (g.type == TGridData::ePlane)
			{
				float dx,dy, l;
				// dopocitat zpatky na plusove souradnice
				dx = (i * distX) - (m_sizeX*0.5f);// - (distX * 0.5f);
				dy = (j * distY) - (m_sizeY*0.5f);// - (distY * 0.5f);
				l = sqrt((dx-x)*(dx-x) + (dy-y)*(dy-y));
				if (l < radius)
				{
					g.plane_heights[0] += (1.f-l/radius)*moveheight;
				}
				dx = (i * distX) - (m_sizeX*0.5f) + distX;
				dy = (j * distY) - (m_sizeY*0.5f);// - (distY * 0.5f);
				l = sqrt((dx-x)*(dx-x) + (dy-y)*(dy-y));
				if (l < radius)
				{
					g.plane_heights[1] += (1.f-l/radius)*moveheight;
				}
				dx = (i * distX) - (m_sizeX*0.5f);// + (distX * 0.5f);
				dy = (j * distY) - (m_sizeY*0.5f) + distY;
				l = sqrt((dx-x)*(dx-x) + (dy-y)*(dy-y));
				if (l < radius)
				{
					g.plane_heights[2] += (1.f-l/radius)*moveheight;
				}
				dx = (i * distX) - (m_sizeX*0.5f) + distX;
				dy = (j * distY) - (m_sizeY*0.5f) + distY;
				l = sqrt((dx-x)*(dx-x) + (dy-y)*(dy-y));
				if (l < radius)
				{
					g.plane_heights[3] += (1.f-l/radius)*moveheight;
				}				
			}
		}

	// vyladit zpatky
	Opt_ProcessPlanes(sx,ex,sy,ey);
}

void HOEAPI GridSurface::ShowWireframe(bool show)
{
	m_wire = show;
}

bool GridSurface::GetHeight(const float x, const float y, float * height)
{
	if (!m_grids)
		return false;
	const float distX = m_sizeX / m_width;
	const float distY = m_sizeY / m_height;
	const uint sx = (uint)((x+(m_sizeX*0.5f))/(distX));
	const uint sy = (uint)((y+(m_sizeY*0.5f))/(distY));
	if (sx < 0 || sx >= m_width || sy < 0 || sy >= m_height)
		return false;
	const TGridData & g = m_grids[m_width*sy+sx];
	const float xx = x - ((sx * distX) - (m_sizeX*0.5f));// - (distX * 0.5f);
	const float yy = y - ((sy * distY) - (m_sizeY*0.5f));// - (distY * 0.5f);
	switch (g.type)
	{
	case TGridData::ePlane:
		{
			float l1 = sqrt(xx*xx+yy*yy);
			if (l1 < 0.0001f)
			{
				*height = g.plane_heights[0]; return true;
			}
			float l2 = sqrt((1-xx)*(1-xx)+yy*yy);
			if (l2 < 0.0001f)
			{
				*height = g.plane_heights[1]; return true;
			}
			float l3 = sqrt(xx*xx+(1-yy)*(1-yy));
			if (l3 < 0.0001f)
			{
				*height = g.plane_heights[2]; return true;
			}
			float l4 = sqrt((1-xx)*(1-xx)+(1-yy)*(1-yy));
			if (l4 < 0.0001f)
			{
				*height = g.plane_heights[3]; return true;
			}
			l1 = 1.f / l1;
			l2 = 1.f / l2;
			l3 = 1.f / l3;
			l4 = 1.f / l4;
			*height = (l1*g.plane_heights[0]+l2*g.plane_heights[1]+
				l3*g.plane_heights[2]+l4*g.plane_heights[3]) / (l1+l2+l3+l4);
			return true;
		}
		return true;
	default:
		return false;
	};
}

void HOEAPI GridSurface::Dump(XHoeStreamWrite * stream)
{
	// write version
	assert(m_grids);
	stream->WriteValue<uint>(le_uint(4));
	// size
	stream->Write<uint>(le_uint(m_width));
	stream->Write<uint>(le_uint(m_height));
	stream->Write<float>(le_float(m_sizeX));
	stream->Write<float>(le_float(m_sizeY));
	// struktura pro data
	// pak jednotlive gridy

	for (uint i=0;i < m_height * m_width;i++)
	{
		TGridData & d = m_grids[i];
		// textures
		stream->Write(&d, 6); // trosku prasarnicka, proste se nacte tex informace 
		
		stream->WriteValue<byte>(le_byte((byte)d.type));
		switch (d.type)
		{
		case TGridData::ePlane:
			stream->Write<float>(le_float(d.plane_heights[0]));
			stream->Write<float>(le_float(d.plane_heights[1]));
			stream->Write<float>(le_float(d.plane_heights[2]));
			stream->Write<float>(le_float(d.plane_heights[3]));
			break;
		case TGridData::eModel:
			stream->WriteValue<byte>(le_byte((byte)d.modelid));
			stream->Write<float>(le_float(d.base_height));
			break;
		};
	}
}

void HOEAPI GridSurface::LoadDump(XHoeStreamRead * stream)
{
	uint ver = stream->Read<uint>();
	if (ver!=4)
	{
		Con_Print("Error: GridSurface Dump - bad version (%d)",ver);
		return;
	}
	assert(ver==4);
	// size
	m_width=le_uint(stream->Read<uint>());
	m_height=le_uint(stream->Read<uint>());
	m_sizeX=le_float(stream->Read<float>());
	m_sizeY=le_float(stream->Read<float>());
	// struktura pro data
	// pak jednotlive gridy
	ReleaseData();
	m_grids = new TGridData[m_height * m_width];
	for (uint i=0;i < m_height * m_width;i++)
	{
		TGridData & d = m_grids[i];
		// textures
		stream->Read(&d, 6); // trosku prasarnicka, proste se nacte tex informace 
		
		d.type = (TGridData::Type)le_byte(stream->Read<byte>());
		switch (d.type)
		{
		case TGridData::ePlane:
			d.plane_heights[0] = le_float(stream->Read<float>());
			d.plane_heights[1] = le_float(stream->Read<float>());
			d.plane_heights[2] = le_float(stream->Read<float>());
			d.plane_heights[3] = le_float(stream->Read<float>());
			break;
		case TGridData::eModel:
			d.modelid = le_byte(stream->Read<byte>());
			d.base_height = le_float(stream->Read<float>());
			break;
		};
	}

	// modely atd..


	// hotovo
	Load();
}

//////////////////////////////
// Optimize
float GridSurface::Opt_GetHeight(uint x, uint y)
{
	float rplane = 0.f, rmodel = 0.f;
	uint nplane =0, nmodel = 0;
	/*!!!*/
	for (int i=0;i<4;i++)
	{
		// tady se to zepta vsech tech okolnich ploch a vrati se podle toho prumer
		const uint xx = (i&1) ? x-1:x;
		const uint yy = (i&2) ? y-1:y;
		if (xx < 0 || yy < 0 || xx >= m_width || yy >= m_height)
			continue;
		if (m_grids[m_width*yy+xx].type == TGridData::ePlane)
		{
			rplane += m_grids[m_width*yy+xx].plane_heights[i];
			nplane++;
		}
		else if (m_grids[m_width*yy+xx].type == TGridData::eModel)
		{
			// zeptat se modelu na vysku praveho rohu
			rmodel += m_grids[m_width*yy+xx].base_height;
			assert(m_models[m_grids[m_width*yy+xx].modelid].mod);
			rmodel += m_models[m_grids[m_width*yy+xx].modelid].coigns[i];
			nmodel++;
		}
		else
		{
			assert(!"unknown type");
		}
	}
	// prumer rovin
	// prumer modelu
	float ret = (nmodel) ? rmodel / nmodel:rplane / nplane;
	assert(ret > -100.f && ret < 100.f);
	return ret;
}

void GridSurface::Opt_ProcessPlanes(uint fromx, uint tox, uint fromy, uint toy)
{
	for (uint x=fromx;x<=tox;x++)
		for (uint y=fromy;y<=toy;y++)
		{
			if (x<0 || x>=m_width || y<0 || y>=m_height)
				continue;
			if (m_grids[m_width*y+x].type==TGridData::ePlane)
			{
				m_grids[m_width*y+x].plane_heights[0] = Opt_GetHeight(x,y);
				m_grids[m_width*y+x].plane_heights[1] = Opt_GetHeight(x+1,y);
				m_grids[m_width*y+x].plane_heights[2] = Opt_GetHeight(x,y+1);
				m_grids[m_width*y+x].plane_heights[3] = Opt_GetHeight(x+1,y+1);
			}
		}
}

/////////////////////////////
MultiStream::MultiStream()
{
	m_nummesh = 0;
	m_vbuffer = NULL;
	m_ibuffer = NULL;
}

MultiStream::~MultiStream()
{
}

bool MultiStream::Begin(const char * fvf, size_t sn)
{
	m_fvf.Set(fvf);
	m_sn = sn;
	assert(m_vbuffer == NULL && m_ibuffer == NULL);
	// vytvorit prvni pole
	// vytvorit buffer pro data a vynulovat nastaveni
	m_vbuffer = new byte[sn*MaxVerticesSize];
	m_ibuffer = new word[MaxIndicesSize];
	m_actvertex = 0;
	m_actindex = 0;
	m_vertexoffset = 0;
	// smazat naposledy pouzite buffery
	/*!!!*/
	m_nummesh = 0;
	return true;
}

void MultiStream::End()
{
	// vytvorit z bufferu stream a vymazat buffer
	CreateMeshFromBuffer();
	SAFE_DELETE_ARRAY(m_vbuffer);
	SAFE_DELETE_ARRAY(m_ibuffer);
}

byte * MultiStream::LockNewVertices(dword numvert, dword numind)
{
	byte * ret = NULL;
	// zkusit pridat, pokud vetsi nez dosavadni tak novy buffer
	if ((m_actvertex+numvert) > MaxVerticesSize || (m_actindex+numind) > MaxIndicesSize)
	{
		if (!CreateMeshFromBuffer())
			return false;
	}
	ret = m_vbuffer + (m_actvertex * m_sn);
	m_vertexoffset = (word)m_actvertex;
	m_actvertex += numvert;
	return ret;
}

void MultiStream::AddIndex(word ind)
{
	assert(m_actindex < MaxIndicesSize);
	m_ibuffer[m_actindex++] = ind + m_vertexoffset;
}

dword MultiStream::GetPos() const
{
	return (((byte)m_nummesh) << 24) | m_actindex;
}

void MultiStream::Render(dword from, dword to)
{
	uint fm = (from&0xff000000) >> 24;
	uint tm = (to&0xff000000) >> 24;
	for (;fm <= tm;fm++)
	{
		assert(fm < m_nummesh);
		dword f = (fm << 24) > from ? 0:from & 0xffff;
		dword t = (fm < tm) ? m_mesh[fm].numind : (to & 0xffff);
		Ref::DrawStdObjectFT(&m_mesh[fm].vertices, &m_mesh[fm].indices, f, t-f);
	}
}

bool MultiStream::CreateMeshFromBuffer()
{
	assert(m_vbuffer && m_ibuffer);
	if (m_actvertex > 0 || m_actindex > 0)
	{
		assert(m_actvertex > 0 && m_actindex > 0);
		assert(m_nummesh < 29);
		m_mesh[m_nummesh].numvert = m_actvertex;
		m_mesh[m_nummesh].numind = m_actindex;
		if (!m_mesh[m_nummesh].vertices.Create(m_actvertex, m_fvf.GetStringFVF(), m_actvertex * m_sn, m_vbuffer))
			return false;
		if (!m_mesh[m_nummesh].indices.Create(m_actindex,m_ibuffer))
			return false;
		m_nummesh++;

		m_actvertex = 0;
		m_actindex = 0;
		m_vertexoffset = 0;
	}
	return true;
}


