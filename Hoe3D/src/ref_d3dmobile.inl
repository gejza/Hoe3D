
/**
   @file    ref_d3d9.inl
   @date    Oct 2007
   @version 1.0
   @brief   Definice inline funkci RefD3DM
*/

#ifndef _HOE_D3DMOBILE_INL__
#define _HOE_D3DMOBILE_INL__

HOE_INLINE IDirect3DMobileDevice * D3DDevice()
{
//	assert(RefD3D9::m_dev);
	return RefD3DM::Device();
}

template<int type> void RefD3DM::SetMatrix(const HoeMath::Matrix4fx & m)
{
	if (type == MatrixViewProj)
	{
		HoeMath::Matrix4fx matIdent;
		matIdent.Identity();
		m_Dev->SetTransform( D3DMTS_VIEW, 
			reinterpret_cast<const D3DMMATRIX*>(&m), D3DMFMT_D3DMVALUE_FIXED );
		m_Dev->SetTransform( D3DMTS_PROJECTION, 
			reinterpret_cast<const D3DMMATRIX*>(&matIdent), D3DMFMT_D3DMVALUE_FIXED );
	}
	else
	{
		m_Dev->SetTransform( (D3DMTRANSFORMSTATETYPE)type, 
			reinterpret_cast<const D3DMMATRIX*>(&m), D3DMFMT_D3DMVALUE_FIXED );
	}
}

template<int type> void RefD3DM::SetMatrix(const HoeMath::Matrix4f & m)
{
	if (type == MatrixViewProj)
	{
		HoeMath::Matrix4f matIdent;
		matIdent.Identity();
		m_Dev->SetTransform( D3DMTS_VIEW, 
			reinterpret_cast<const D3DMMATRIX*>(&m), D3DMFMT_D3DMVALUE_FLOAT );
		m_Dev->SetTransform( D3DMTS_PROJECTION, 
			reinterpret_cast<const D3DMMATRIX*>(&matIdent), D3DMFMT_D3DMVALUE_FLOAT );
	}
	else
	{
		m_Dev->SetTransform( (D3DMTRANSFORMSTATETYPE)type, 
		reinterpret_cast<const D3DMMATRIX*>(&m), D3DMFMT_D3DMVALUE_FLOAT );
	}
}


#endif //_HOE_D3DMOBILE_INL__

