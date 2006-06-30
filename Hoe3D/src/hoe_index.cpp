
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "hoe_index.h"

HoeIndex::HoeIndex()
{
	m_num = 0;
	m_ib = 0;
	m_sw = NULL;
}

bool HoeIndex::Create(int num_indices)
{
	if (m_num==num_indices)
		return true;

	m_num = num_indices;
#ifdef _HOE_D3D_ 
	SAFE_RELEASE(m_ib);
	HRESULT hRes = D3DDevice()->CreateIndexBuffer(m_num * 2,D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_ib RESERVE_PAR);
#endif
#ifdef _HOE_OPENGL_
	SAFE_DELETE_ARRAY(m_sw);
	m_sw = new word[num_indices];
#endif
	return true;
}

#define SETMAX(m) if (m > max) max = m; if (m < min) min = m;

void HoeIndex::Dump(HoeLog *log)
{
	if (!log)
		return;

	if (!m_sw)
	{
		log->Log("NULL pointer -> stop dumping");
		return;
	}
	log->Log("Memory address %p -> %p", m_sw, m_sw+m_num);
	int min = 0;
	int max = 0;
	if (m_num > 0)
	{
		min = m_sw[0];
		max = m_sw[0];
	}
	for (int i=0; i < this->m_num/3;i++)
	{
		log->Log("[%d %d %d]",m_sw[i*3+0],m_sw[i*3+1],m_sw[i*3+2]);
		SETMAX(m_sw[i*3+0]);
		SETMAX(m_sw[i*3+1]);
		SETMAX(m_sw[i*3+2]);
	}
	log->Log("min: %d max %d", min, max);
}

word * HoeIndex::Lock()
{
#ifdef _HOE_D3D_
	m_ib->Lock(0,m_num * 2,(D3DLOCKTYPE)&m_sw,0);
#endif
	return m_sw;
}

void HoeIndex::Unlock()
{
#ifdef _HOE_D3D_
	m_ib->Unlock();
	m_sw = NULL;
#endif
}
