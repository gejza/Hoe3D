
#ifndef _HOE_INDEX_H_
#define _HOE_INDEX_H_

class HoeIndex
{
	SysIndexBuffer m_ib;
	word  *m_sw;
	dword m_num;
public:
	HoeIndex();
	bool Create(int num_indices);
	word * Lock();
	void Unlock();
	//void Set();

	SysIndexBuffer GetIndexBuffer() { 
#ifdef _HOE_D3D_
		return m_ib;
#else
		return (SysIndexBuffer)m_sw;
#endif
	}
	dword GetNumIndices() { 
		return m_num;
	}
};

#endif // _HOE_INDEX_H_
