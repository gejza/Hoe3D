
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

HOE_INLINE void RefD3D9::SetMatrix(const HoeMath::MATRIX & m)
{
	D3DDevice()->SetTransform( D3DTS_WORLD, reinterpret_cast<const D3DMATRIX*>(&m) );
}


#endif //_HOE_D3D9_INL__

