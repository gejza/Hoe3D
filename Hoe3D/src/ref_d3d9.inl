
/**
   @file    ref_d3d9.inl
   @date    Mar 2006
   @version 1.0
   @brief   Definice inline funkci RefD3D9
*/

#ifndef _HOE_D3D9_INL__
#define _HOE_D3D9_INL__

HOE_INLINE IDirect3DDevice9 * D3DDevice()
{
//	assert(RefD3D9::m_dev);
	return RefD3D9::Device();
}

template<int type> void RefD3D9::SetMatrix(const HoeMath::Matrix4f & m)
{
	m_Dev->SetTransform( (D3DTRANSFORMSTATETYPE)type, 
		reinterpret_cast<const D3DMATRIX*>(&m) );
}

HOE_INLINE IDirect3DVertexBuffer9 * RefD3D9::CreateVertexBuffer(dword size, HoeFVF& fvf,
		bool dynamic)
{
	IDirect3DVertexBuffer9 * vb;
	HRESULT hRes = m_Dev->CreateVertexBuffer( size,
		dynamic ? D3DUSAGE_DYNAMIC:0 /* Usage */, fvf.GetFVF(), D3DPOOL_DEFAULT, &vb, 0 );
	checkres(hRes, "CreateVertexBuffer");
	return vb;
}

#endif //_HOE_D3D9_INL__

