
/**
   @file    ref_d3d8.inl
   @date    Mar 2006
   @version 1.0
   @brief   Definice inline funkci RefD3D8
*/

#ifndef _HOE_D3D9_INL__
#define _HOE_D3D9_INL__

HOE_INLINE IDirect3DDevice8 * D3DDevice()
{
	return RefD3D8::Device();
}

HOE_INLINE void RefD3D8::SetMatrix(const HoeMath::Matrix & m)
{
	D3DDevice()->SetTransform( D3DTS_WORLD, (const D3DMATRIX *)&m );
}


#endif //_HOE_D3D9_INL__

