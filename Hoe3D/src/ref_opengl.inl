
#ifndef _HOE_OPENGL_INL__
#define _HOE_OPENGL_INL__

template<int type> void RefOpenGL::SetMatrix(const HoeMath::Matrix4f & m)
{
	glMatrixMode(type);
	glLoadMatrixf((GLfloat*)&m);;
}

#endif //_HOE_OPENGL_INL__




