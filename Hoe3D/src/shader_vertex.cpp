
#include "system.h"
#include "shared.h"
#include "ref.h"
#include "shader_vertex.h"
#include "scene.h"
#include "light_system.h"

const DWORD g_vs11_main[] =
{
    0xfffe0101, 0x0000001f, 0x80000000, 0x900f0000, 0x0000001f, 0x80000003, 
    0x900f0001, 0x00000014, 0xc00f0000, 0x90e40000, 0xa0e40000, 0x00000002, 
    0x800f0000, 0xa0e4000a, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 
    0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 
    0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 
    0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000005, 0x800f0001, 
    0x80e40000, 0xa0e40028, 0x00000002, 0x800f0000, 0xa0e4000b, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 0xa0e40029, 0x80e40001, 
    0x00000002, 0x800f0000, 0xa0e4000c, 0x91e40000, 0x00000008, 0x80080000, 
    0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 
    0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 
    0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 
    0x800f0001, 0x80e40000, 0xa0e4002a, 0x80e40001, 0x00000002, 0x800f0000, 
    0xa0e4000d, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 
    0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 
    0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 
    0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 
    0xa0e4002b, 0x80e40001, 0x00000002, 0x800f0000, 0xa0e4000e, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 0xa0e4002c, 0x80e40001, 
    0x00000002, 0x800f0000, 0xa0e4000f, 0x91e40000, 0x00000008, 0x80080000, 
    0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 
    0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 
    0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 
    0x800f0001, 0x80e40000, 0xa0e4002d, 0x80e40001, 0x00000002, 0x800f0000, 
    0xa0e40010, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 
    0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 
    0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 
    0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 
    0xa0e4002e, 0x80e40001, 0x00000002, 0x800f0000, 0xa0e40011, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 0xa0e4002f, 0x80e40001, 
    0x00000002, 0x800f0000, 0xa0e40012, 0x91e40000, 0x00000008, 0x80080000, 
    0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 
    0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 
    0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 
    0x800f0001, 0x80e40000, 0xa0e40030, 0x80e40001, 0x00000002, 0x800f0000, 
    0xa0e40013, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 
    0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 
    0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 
    0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 
    0xa0e40031, 0x80e40001, 0x00000002, 0x800f0000, 0xa0e40014, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 0xa0e40032, 0x80e40001, 
    0x00000002, 0x800f0000, 0xa0e40015, 0x91e40000, 0x00000008, 0x80080000, 
    0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 
    0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 
    0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 
    0x800f0001, 0x80e40000, 0xa0e40033, 0x80e40001, 0x00000002, 0x800f0000, 
    0xa0e40016, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 
    0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 
    0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 
    0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 
    0xa0e40034, 0x80e40001, 0x00000002, 0x800f0000, 0xa0e40017, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 0xa0e40035, 0x80e40001, 
    0x00000002, 0x800f0000, 0xa0e40018, 0x91e40000, 0x00000008, 0x80080000, 
    0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 
    0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 
    0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 
    0x800f0001, 0x80e40000, 0xa0e40036, 0x80e40001, 0x00000002, 0x800f0000, 
    0xa0e40019, 0x91e40000, 0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 
    0x00000007, 0x80080000, 0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 
    0x80ff0000, 0x00000008, 0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 
    0x800f0000, 0x80e40000, 0xa0e40005, 0x00000004, 0x800f0001, 0x80e40000, 
    0xa0e40037, 0x80e40001, 0x00000002, 0x800f0000, 0xa0e4001a, 0x91e40000, 
    0x00000008, 0x80080000, 0x80e40000, 0x80e40000, 0x00000007, 0x80080000, 
    0x80ff0000, 0x00000005, 0x80070000, 0x80e40000, 0x80ff0000, 0x00000008, 
    0x800f0000, 0x80e40000, 0x90e40001, 0x0000000b, 0x800f0000, 0x80e40000, 
    0xa0e40005, 0x00000004, 0xd00f0000, 0x80e40000, 0xa0e40038, 0x80e40001, 
    0x0000ffff
};

const char g_vs11_char[] = "!!ARBvp1.0\
\
ATTRIB iPos = vertex.position;\
ATTRIB iNormal = vertex.normal;\
\
PARAM mvp[4] = { state.matrix.mvp };\
# PARAM mvp[4] = { program.local[0..3] };\
PARAM light[17] = { program.local[10..26] };\
PARAM color[17] = { program.local[40..56] };\
PARAM zero = { 0.0, 0.0, 0.0, 0.0 };\
\
TEMP r0,r1;\
\
OUTPUT oPos = result.position;\
OUTPUT oD0 = result.color;\
\
# shader\
DP4 oPos.x, mvp[0], iPos;\
DP4 oPos.y, mvp[1], iPos;\
DP4 oPos.z, mvp[2], iPos;\
DP4 oPos.w, mvp[3], iPos;\
\
# light 0\
ADD  r0, light[0], -iPos;	# L = light_pos - vertex_pos\
# normalize L\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
# compute diffuse light, L in r0\
DP3  r0, r0, iNormal;		# L.N\
MAX  r0, r0, zero;			# max r0,0\
# apply diffuse material color\
MUL  r1, r0, color[0];\
\
# light 1\
ADD  r0, light[1], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[1], r1;	# apply diffuse material color\
\
# light 2\
ADD  r0, light[2], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[2], r1;	# apply diffuse material color\
\
# light 3\
ADD  r0, light[3], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[3], r1;	# apply diffuse material color\
\
# light 4\
ADD  r0, light[4], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[4], r1;	# apply diffuse material color\
\
# light 5\
ADD  r0, light[5], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[5], r1;	# apply diffuse material color\
\
# light 6\
ADD  r0, light[6], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[6], r1;	# apply diffuse material color\
\
# light 7\
ADD  r0, light[7], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[7], r1;	# apply diffuse material color\
\
# light 8\
ADD  r0, light[8], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[8], r1;	# apply diffuse material color\
\
# light 9\
ADD  r0, light[9], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[9], r1;	# apply diffuse material color\
\
# light 10\
ADD  r0, light[10], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[10], r1;	# apply diffuse material color\
\
# light 11\
ADD  r0, light[11], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[11], r1;	# apply diffuse material color\
\
# light 12\
ADD  r0, light[12], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[12], r1;	# apply diffuse material color\
\
# light 13\
ADD  r0, light[13], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[13], r1;	# apply diffuse material color\
\
# light 14\
ADD  r0, light[14], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[14], r1;	# apply diffuse material color\
\
# light 15\
ADD  r0, light[15], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  r1, r0, color[15], r1;	# apply diffuse material color\
\
# light 16\
ADD  r0, light[16], -iPos;	# L = light_pos - vertex_pos\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\
DP3  r0, r0, iNormal;		# L.N = diffuse light\
MAX  r0, r0, zero;			# max r0,0\
MAD  oD0, r0, color[16], r1;# apply diffuse material color\
\
END";

bool HoeVertexShader::Load()
{
#ifdef _HOE_D3D9_
	HRESULT hRes = D3DDevice()->CreateVertexShader( g_vs11_main, &m_shader);
	assert(hRes == S_OK);
	return true;
#endif
#ifdef _HOE_OPENGL_
	if (!GetRef()->ext.vs.IsSupported())
		return false;
	GetRef()->ext.vs.glGenProgramsARB( 1, &m_shader );
	GetRef()->ext.vs.glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_shader );
	GetRef()->ext.vs.glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, sizeof(g_vs11_char),g_vs11_char); 
	assert(GL_INVALID_OPERATION != glGetError());
#endif

	return false;
}

void HoeVertexShader::Setup(const HoeScene * scene) const
{
	HoeMath::MATRIX m;
	scene->GetCamera()->GetViewProjMatrix(&m);
	HoeMath::MATRIX w;
#ifdef _HOE_D3D9_
	HRESULT hRes = D3DDevice()->SetVertexShader( m_shader);
	assert(hRes == S_OK);
	D3DDevice()->GetTransform(D3DTS_WORLD, w);
#endif
#ifdef _HOE_OPENGL_
	if (!GetRef()->ext.vs.IsSupported())
		return;
	GetRef()->ext.vs.glBindProgramARB(GL_VERTEX_PROGRAM_ARB, m_shader);
	glEnable(GL_VERTEX_PROGRAM_ARB);
	// consts
	w.Identity();
#endif

	//HoeMath::VECTOR3 vv(0,0,0);
	//vv.Multiply(w);
	w.Multiply(m);
	w.Transpoze();

	HoeMath::VECTOR4 zero(0,0,0,0);
#ifdef _HOE_D3D9_
	D3DDevice()->SetVertexShaderConstantF(  0, (float*)w.m, 4 );		// c0-c3 contains composite transform matrix
	D3DDevice()->SetVertexShaderConstantF(  5, zero.m, 1 );	// c5   0,0,0,0

	for(uint i=0; i<17 && i < scene->GetLS()->GetNumActiveLights(); i++)
		{
			D3DDevice()->SetVertexShaderConstantF( i+10, scene->GetLS()->GetActiveLight(i)->GetPosition().m, 1 );
			D3DDevice()->SetVertexShaderConstantF( i+40, scene->GetLS()->GetActiveLight(i)->GetColor().m,  1 );
		}
#endif
#ifdef _HOE_OPENGL_
	GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 0, &w._11 );
	GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 1, &w._21 );
	GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 2, &w._31 );
	GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 3, &w._41 );
	GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB,  5, zero.m);	// c5   0,0,0,0

	for(uint i=0; i<17 && i < scene->GetLS()->GetNumActiveLights(); i++)
		{
			GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i+10, scene->GetLS()->GetActiveLight(i)->GetPosition().m);
			GetRef()->ext.vs.glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i+40, scene->GetLS()->GetActiveLight(i)->GetColor().m);
		}
#endif
}

