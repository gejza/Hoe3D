
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "hoe_index.h"

HoeIndex::HoeIndex(bool soft)
{
	m_num = 0;
	m_soft = soft;
	m_ib = 0;
	m_sw = NULL;
}

bool HoeIndex::Create(dword num_indices)
{
	assert(num_indices > 0);

	if (m_num==num_indices)
		return true;

	m_num = num_indices;
#ifdef _HOE_D3D_ 
	if (!m_soft)
	{
	SAFE_RELEASE(m_ib);
	HRESULT hRes = D3DDevice()->CreateIndexBuffer(m_num * 2,D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_ib RESERVE_PAR);
	}
	else
#endif
		SAFE_DELETE_ARRAY(m_sw);
	return true;
}

bool HoeIndex::Create(dword num_indices, word * data)
{
	word * li;
	if (!Create(num_indices))
			return false;
	li = Lock();
	assert(li);
	memcpy(li, data, num_indices*sizeof(word));
	Unlock();
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
	for (dword i=0; i < this->m_num/3;i++)
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
	if (!m_soft)
	{
		assert(m_ib);
		m_ib->Lock(0,m_num * 2,(D3DLOCKTYPE)&m_sw,0);
	}
#endif
	if (!m_sw)
		m_sw = new word[m_num];
	return m_sw;
}

void HoeIndex::Unlock()
{
#ifdef _HOE_D3D_
	if (!m_soft)
	{
		m_ib->Unlock();
		m_sw = NULL;
	}
#endif
}
