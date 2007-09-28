
#ifndef _HOE_VERTEX_SHADER_H_
#define _HOE_VERTEX_SHADER_H_

class HoeScene;

class HoeVertexShader
{
protected:
#if defined (_HOE_D3D9_)
	IDirect3DVertexShader9 * m_shader;
#elif defined (_HOE_OPENGL_)
	GLuint m_shader;
#else
	int m_shader;
#endif
public:
	bool Load();
	void Setup(const HoeScene * scene) const;
};

#endif // _HOE_VERTEX_SHADER_H_

