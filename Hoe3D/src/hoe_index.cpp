
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
	m_num = num_indices;
#ifdef _HOE_D3D_ 
	HRESULT hRes = D3DDevice()->CreateIndexBuffer(m_num * 2,D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_ib RESERVE_PAR);
#endif
#ifdef _HOE_OPENGL_
	m_sw = new word[num_indices];
#endif
	return true;
}

word * HoeIndex::Lock()
{
#ifdef _HOE_D3D_
	word * ptr;
	m_ib->Lock(0,m_num * 2,(D3DLOCKTYPE)&ptr,0);
	return ptr;
#endif
#ifdef _HOE_OPENGL_
	return m_sw;
#endif

}

void HoeIndex::Unlock()
{
#ifdef _HOE_D3D_
	m_ib->Unlock();
#endif
}
