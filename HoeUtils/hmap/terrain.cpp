#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <assert.h>

#include "../../src/hoe3d_math.h"
#include "../utils.h"
#include "../../src/hmap_file.h"

HoeUtils::File mapfile;
HoeMapHeader head;

using namespace HoeMath;
using namespace HoeUtils;

struct Node
{
	List<TRIANGLE> m_list;
	List<VECTOR3> m_vertices;
	List<unsigned short> m_indices;
	HoeMath::RECT rect;

	int AddVector(VECTOR3 &v);
	bool SortVectors(VECTOR3 *p,int &num);
public:
	void SetRect(HoeMath::RECT &r);
	void AddTriangle(TRIANGLE &t);
	void CreateIndices();

	bool SaveToFile(HoeUtils::File * file);
};

class VectorMap
{
	Node * nodes;
	int num_x;
	int num_y;
	HoeMath::RECT rect;
	void GetRect();
	void AddToNode(TRIANGLE &t);
public:
	List<TRIANGLE> m_list;

	VectorMap(int mx,int my);
	void Add(TRIANGLE &t);
	void Compute();

	bool SaveToFile(HoeUtils::File * file);
};

/////////////////////////////////////////////
int Node::AddVector(VECTOR3 &v)
{
	for (int n=0;n < m_vertices.GetNum();n++)
	{
		if (v == m_vertices[n])
			return n;
	}
	return m_vertices.Add(v);
}


void Node::SetRect(HoeMath::RECT &r)
{
	rect = r;
}

void Node::AddTriangle(TRIANGLE &t)
{
	int i,j;
	// vsechno spatne zpatky na stromy

	if (t.a.x > rect.right && t.b.x > rect.right && t.c.x > rect.right)
		return;
	if (t.a.x < rect.left && t.b.x < rect.left && t.c.x < rect.left)
		return;
	if (t.a.y < rect.top && t.b.y < rect.top && t.c.y < rect.top)
		return;
	if (t.a.y > rect.bottom && t.b.y > rect.bottom && t.c.y > rect.bottom)
		return;
	
	if (t.NumVectorsXY(rect) == 3)
	{
		this->m_list.Add(t);
		return;
	}

	VECTOR3 normal;
	float d;
	HoePlaneNormal(t.a,t.b,t.c,normal);
	if (normal.z == 0)
		return;
	d = HoePlaneDistance(normal,t.a);

	LINE2D lines[20];
	int dbg[20];
	lines[0].Set(rect.right,rect.bottom,rect.right,rect.top);
	lines[1].Set(rect.right,rect.top,rect.left,rect.top);
	lines[2].Set(rect.left,rect.top,rect.left,rect.bottom);
	lines[3].Set(rect.left,rect.bottom,rect.right,rect.bottom);
	lines[4].Set(t.a.x,t.a.y,t.b.x,t.b.y);
	lines[5].Set(t.b.x,t.b.y,t.c.x,t.c.y);
	lines[6].Set(t.c.x,t.c.y,t.a.x,t.a.y);
	VECTOR3 p[20];
	int np = 0;

	// add intersections triangle
	if (rect.IsVectorOnXY(t.a))
	{
		dbg[np] = 0; 
		p[np++] = t.a;
	}
	if (rect.IsVectorOnXY(t.b))
	{
		dbg[np] = 1;
		p[np++] = t.b;
	}
	if (rect.IsVectorOnXY(t.c))
	{
		dbg[np] = 2;
		p[np++] = t.c;
	}

	// add intersections 
	if (HoeInsidePolygon(lines,7,rect.right,rect.bottom,0,3))
	{
		dbg[np] = 3;
		p[np].x = rect.right;
		p[np].y = rect.bottom;
		p[np].z = (-(normal.x * p[np].x + normal.y * p[np].y + d) / normal.z);
		np++;
	}
	if (HoeInsidePolygon(lines,7,rect.right,rect.top,0,1))
	{
		dbg[np] = 4;
		p[np].x = rect.right;
		p[np].y = rect.top;
		p[np].z = (-(normal.x * p[np].x + normal.y * p[np].y + d) / normal.z);
		np++;
	}
	if (HoeInsidePolygon(lines,7,rect.left,rect.top,1,2))
	{
		dbg[np] = 5;
		p[np].x = rect.left;
		p[np].y = rect.top;
		p[np].z = (-(normal.x * p[np].x + normal.y * p[np].y + d) / normal.z);
		np++;
	}
	if (HoeInsidePolygon(lines,7,rect.left,rect.bottom,2,3))
	{
		dbg[np] = 6;
		p[np].x = rect.left;
		p[np].y = rect.bottom;
		p[np].z = (-(normal.x * p[np].x + normal.y * p[np].y + d) / normal.z);
		np++;
	}

	for(i=0;i < 4;i++)
		for (j=4;j < 7;j++)
		{
			LINE2D &l1 = lines[i];
			LINE2D &l2 = lines[j];
			float x,y;
			if (l1.Intersection(l2,x,y) && HoeInsidePolygon(lines,7,x,y,i,j))
			{
				dbg[np] = 7;
				p[np].x = x;
				p[np].y = y;
				p[np].z = (-(normal.x * x + normal.y * y + d) / normal.z);
				np++;		
			}
		}

	if (np < 3)
		return;

	// kontrola dvojvertexu
	for (i=0;i < np;i++)
	{
		for (j=i+1;j < np;j++)
		{
			if (p[i].x == p[j].x && p[i].y == p[j].y)
			{
				int s = np-1;
				if (j < s)
				{
					VECTOR3 tmp = p[s];
					p[s] = p[j];
					p[j] = tmp;
				}
				np--;
			}
		}
	}

	if (np < 3)
		return;

	assert(np <= 7);
		

	// hehe ted je seradit...
	if (!SortVectors(p,np))
	{
		printf("Porad chyba\n");
		exit(0);
	}

	// a pak vygenerovat
	// mam p a indexy...zbejva vytvorit trojuhelnik
	TRIANGLE tt;
	tt.a = p[0];
	tt.b = p[1];
	for (i = 2;i < (np-1);i++)
	{
		tt.c = p[i];
		this->m_list.Add(tt);
		tt.b = tt.c;
	}
	tt.c = p[i];
	this->m_list.Add(tt);
}

bool Node::SortVectors(VECTOR3 *p,int &num)
{
	int i,j;

	// vypocitani uhlu
	for (i=0;i < num;i++)
	{
		bool _ok = true;
		for (j=i+1;j < num;j++)
		{
			LINE2D line;
			line.SetXY(p[i],p[j]);
			_ok = true;
			for (int m=0;m < num;m++)
			{
				if (m == i || m == j)
					continue;
				
				float c = line.Compute(p[m].x,p[m].y);
				if (c < 0)
				{
					_ok = false;
					break;
				}
				if (c == 0)
				{
					// nejak doslo k duplikaci bodu.. mel by se vymazat
					assert(1);
				}
			}
			if (_ok)
				break;
		}

		if (_ok && j != num)
		{
			// bod nalezen a je to j
			if (j == i+1) // uz je serazenej
				continue;
			VECTOR3 tmp = p[i+1];
			p[i+1] = p[j];
			p[j] = tmp;
		}
	}
	return true;
}

void Node::CreateIndices()
{
	for (int n=0;n < m_list.GetNum();n++)
	{
		unsigned short i;
		i = this->AddVector(m_list[n].a);
		m_indices.Add(i);
		i = this->AddVector(m_list[n].b);
		m_indices.Add(i);
		i = this->AddVector(m_list[n].c);
		m_indices.Add(i);
	}
	m_list.Clean();
}

bool Node::SaveToFile(HoeUtils::File * file)
{
	HoeMapNode node;
	node.id_leaf = 'L';
	node.num_vert = m_vertices.GetNum();
	node.num_indices = m_indices.GetNum();
	assert(m_vertices.GetSize() == (node.num_vert * sizeof(float) * 3));
	assert(m_indices.GetSize() == (node.num_indices * 2));

	file->Write(&node,sizeof(HoeMapNode));
	file->Write(m_vertices.GetData(),m_vertices.GetSize());
	file->Write(m_indices.GetData(),m_indices.GetSize());
	return true;
}

/////////////////////////////////////////////

VectorMap::VectorMap(int mx,int my)
{
	nodes = new Node[mx * my];
	num_x = mx;
	num_y = my;
}

void VectorMap::Add(TRIANGLE &t)
{
	m_list.Add(t);
}

void VectorMap::GetRect()
{
	if (m_list.Empty())
		return;
	rect.left = rect.right = m_list[0].a.x;
	rect.top = rect.bottom = m_list[0].a.y;
	for (int i=0;i < m_list.GetNum();i++)
	{
		m_list[i].SignXY(rect);
	}
	printf("l: %f r: %f t: %f b: %f\n",rect.left,rect.right,rect.top,rect.bottom);
}

void VectorMap::Compute()
{
	int x,y;
	printf("Compute rect..\n");
	GetRect();
	float fxs = (rect.right - rect.left) / num_x;
	float fys = (rect.bottom - rect.top) / num_y;
	for (x=0; x < num_x;x++)
		for (y=0;y < num_y;y++)
		{
			HoeMath::RECT r;
			r.left = rect.left + fxs * x;
			r.right = rect.left + fxs * (x+1);
			r.top = rect.top + fys * y;
			r.bottom = rect.top + fys * (y+1);
			nodes[y * num_x + x].SetRect(r);
		}
	printf("Add to nodes..\n");
	for (int i=0;i < m_list.GetNum();i++)
	{
		printf("%d%%   \r",(i * 100) / m_list.GetNum());
		AddToNode(m_list[i]);
	}
	printf("100% \n");
	int nt = 0;
	int ni = 0;
	printf("Create indexes..\n");
	for (x=0; x < num_x;x++)
		for (y=0;y < num_y;y++)
		{
			nodes[y * num_x + x].CreateIndices();
			nt += nodes[y * num_x + x].m_vertices.GetNum();
			ni += nodes[y * num_x + x].m_indices.GetNum();
		}
	printf("Added (%d/%d)\n",ni,nt);
}

void VectorMap::AddToNode(TRIANGLE &t)
{
	for (int y=0;y < num_y;y++)
		for (int x=0; x < num_x;x++)
		{
			nodes[y * num_x + x].AddTriangle(t);
		}
}

bool VectorMap::SaveToFile(HoeUtils::File * file)
{
	HoeMapTerrain ter;
	memset(&ter,0,sizeof(HoeMapTerrain));
	ter.num_x = num_x;
	ter.num_y = num_y;

	do
	{
		printf("Get num leafs X Y(%d,%d): ",num_x,num_y);scanf("%d %d",&ter.tx,&ter.ty);
		if (ter.tx == 0 || ter.ty == 0 || (num_x % ter.tx) != 0 || (num_y % ter.ty) != 0)
		{
			printf("error\n");continue;
		}
		break;
	} while (1);

	printf("Get mask for textures (%%x - axisX, %%y - axisY): \n");
	scanf("%s",ter.mask_texture_name);

	file->Write(&ter,sizeof(HoeMapTerrain));

	for (int y=0;y < num_y;y++)
		for (int x=0; x < num_x;x++)
		{
			if (!nodes[y * num_x + x].SaveToFile(file))
				return false;
		}	

	return true;
}

/////////////////////////////////////////////

bool CreateLeafs()
{
	char fname[256];
	FILE *f = NULL;
	int numvert = 0;
	int x=0,y=0;
	bool textfile = true;


	while (1)
	{
        printf("Jmeno souboru pro vertexy: ");gets(fname);
		textfile = HoeUtils::is_ext(fname,"txt");
		if (textfile)
			f = fopen(fname,"rt");
		else
			f = fopen(fname,"rb");
		if (!f)
		{
			printf("error open file '%s'\n",fname);
			continue;
		}
		break;
	}

	VectorMap map(x,y);
	VECTOR3 normal;

	while(1)
	{
		TRIANGLE t;
		size_t num;
		if (textfile)
		{
			if (fscanf(f,"%f %f %f",&t.a.x,&t.a.y,&t.a.z) != 3)
				break;
			if (fscanf(f,"%f %f %f",&t.b.x,&t.b.y,&t.b.z) != 3 ||
				fscanf(f,"%f %f %f",&t.c.x,&t.c.y,&t.c.z) != 3)
			{
				printf("WARNING: Pocet bodu terenu neni delitelny trema.\n");
				break;
			}
		}
		else
		{
			num = fread(&t,sizeof(float) * 3,3,f);
			if (num == 0)
			{
				break;
			}
		}

		if (num != 3)
		{
			printf("WARNING: Pocet bodu terenu neni delitelny trema.\n");
			break;
		}
		
		//HoePlaneNormal(t.a,t.b,t.c,normal);
		//if (normal.z > 0)
			
		map.Add(t);
	}

	printf("Read %d triangles(%d)\n",map.m_list.GetNum(),map.m_list.GetMax());

	do
	{
		printf("Get num nodes X Y: ");scanf("%d %d",&x,&y);
		if (x <= 0 || y <= 0)
		{
			printf("error\n");continue;
		}
		break;
	} while (1);

	map.Compute();

	map.SaveToFile(&mapfile);

	fclose(f);
	return true;
}

int main(int argc,char *argv[])
{
	/*memset(&head,0,sizeof(head));	
	head.id = IDMAPHEADER;
	head.ver = IDMAPVER;

	if (argc < 2)
	{
		printf("Usage: %s <file>\n",argv[0]);
		return 0;
	}

	if (!mapfile.Open(argv[1],HoeUtils::File::mWrite))
	{
		printf("Cannot open file %s.\n",argv[1]);
		return 0;
	}
	mapfile.Write(&head,sizeof(HoeMapHeader));

	CreateLeafs();

	mapfile.SetPos(0);
	mapfile.Write(&head,sizeof(HoeMapHeader));
	mapfile.Close();*/

	return 0;

	return 0;
}