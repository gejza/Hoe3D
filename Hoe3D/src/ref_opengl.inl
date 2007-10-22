
#ifndef _HOE_OPENGL_INL__
#define _HOE_OPENGL_INL__

HOE_INLINE void RefOpenGL::SetMatrix(const HoeMath::Matrix4f & m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&m);;
}

#endif //_HOE_OPENGL_INL__




