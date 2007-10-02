
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

HOE_INLINE void RefD3DM::SetMatrix(const HoeMath::Matrix & m)
{
	assert(0);
	//D3DDevice()->SetTransform( D3DMTS_WORLD, reinterpret_cast<const D3DMMATRIX*>(&m) );
}


#endif //_HOE_D3DMOBILE_INL__

