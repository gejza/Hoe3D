
#include "StdAfx.h"
#include "utils.h"
#include "shared.h"
#include "ref.h"
#include "shader_vertex.h"
#include "scene.h"
#include "light_system.h"

const unsigned long g_vs11_main[] =
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

const char g_vs11_char[] = "!!ARBvp1.0\n\
\n\
ATTRIB iPos = vertex.position;\n\
ATTRIB iNormal = vertex.normal;\n\
\n\
#PARAM mvp[4] = { state.Matrix.mvp };\n\
PARAM mvp[4] = { program.local[0..3] };\n\
PARAM light[17] = { program.local[10..26] };\n\
PARAM color[17] = { program.local[40..56] };\n\
PARAM zero = { 0.0, 0.0, 0.0, 0.0 };\n\
\n\
TEMP r0,r1;\n\
\n\
OUTPUT oPos = result.position;\n\
OUTPUT oD0 = result.color;\n\
\n\
# shader\n\
DP4 oPos.x, mvp[0], iPos;\n\
DP4 oPos.y, mvp[1], iPos;\n\
DP4 oPos.z, mvp[2], iPos;\n\
DP4 oPos.w, mvp[3], iPos;\n\
\n\
# light 0\n\
ADD  r0, light[0], -iPos;	# L = light_pos - vertex_pos\n\
# normalize L\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
# compute diffuse light, L in r0\n\
DP3  r0, r0, iNormal;		# L.N\n\
MAX  r0, r0, zero;			# max r0,0\n\
# apply diffuse material color\n\
MUL  r1, r0, color[0];\n\
\n\
# light 1\n\
ADD  r0, light[1], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[1], r1;	# apply diffuse material color\n\
\n\
# light 2\n\
ADD  r0, light[2], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[2], r1;	# apply diffuse material color\n\
\n\
# light 3\n\
ADD  r0, light[3], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[3], r1;	# apply diffuse material color\n\
\n\
# light 4\n\
ADD  r0, light[4], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[4], r1;	# apply diffuse material color\n\
\n\
# light 5\n\
ADD  r0, light[5], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[5], r1;	# apply diffuse material color\n\
\n\
# light 6\n\
ADD  r0, light[6], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[6], r1;	# apply diffuse material color\n\
\n\
# light 7\n\
ADD  r0, light[7], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[7], r1;	# apply diffuse material color\n\
\n\
# light 8\n\
ADD  r0, light[8], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[8], r1;	# apply diffuse material color\n\
\n\
# light 9\n\
ADD  r0, light[9], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[9], r1;	# apply diffuse material color\n\
\n\
# light 10\n\
ADD  r0, light[10], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[10], r1;	# apply diffuse material color\n\
\n\
# light 11\n\
ADD  r0, light[11], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[11], r1;	# apply diffuse material color\n\
\n\
# light 12\n\
ADD  r0, light[12], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[12], r1;	# apply diffuse material color\n\
\n\
# light 13\n\
ADD  r0, light[13], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[13], r1;	# apply diffuse material color\n\
\n\
# light 14\n\
ADD  r0, light[14], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[14], r1;	# apply diffuse material color\n\
\n\
# light 15\n\
ADD  r0, light[15], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  r1, r0, color[15], r1;	# apply diffuse material color\n\
\n\
# light 16\n\
ADD  r0, light[16], -iPos;	# L = light_pos - vertex_pos\n\
DP3  r0.w, r0, r0;			# r0.w = |r0|^2\n\
RSQ  r0.w, r0.w;			# r0.w = 1/sqrt(r0.w) = 1/|r0|\n\
MUL  r0.xyz, r0, r0.w;		# r0 - normalized L vector\n\
DP3  r0, r0, iNormal;		# L.N = diffuse light\n\
MAX  r0, r0, zero;			# max r0,0\n\
MAD  oD0, r0, color[16], r1;# apply diffuse material color\n\
\n\
END";

bool HoeVertexShader::Load()
{
	Con_Print("shader version: %x", GetRef()->GetVertexShaderVersion());
	if (GetRef()->GetVertexShaderVersion() >= makeword(1,1))
	{
#ifdef _HOE_D3D9_
	HRESULT hRes = D3DDevice()->CreateVertexShader( g_vs11_main, &m_shader);
	checkres(hRes,"CreateVertexShader")
	return true;
#endif
#ifdef _HOE_OPENGL_
	glGenProgramsARB( 1, &m_shader );
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_shader );
	glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, sizeof(g_vs11_char)-1,g_vs11_char); 
	if (GL_INVALID_OPERATION == glGetError())
	{
		GLint errPos;
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errPos );

		// Print implementation-dependent program errors and warnings string.
		const unsigned char *errString;
		errString = glGetString( GL_PROGRAM_ERROR_STRING_ARB );
		Con_Print("Vertex program failed on line %d:", errPos);
		Con_Print("%s", errString);
		exit(1);
	}
#endif
	}
	else
		m_shader = 0;

	return false;
}

void HoeVertexShader::Setup(const HoeScene * scene) const
{
	if (m_shader)
	{
	HoeMath::Matrix4v m;
	scene->GetCamera()->GetViewProjMatrix(&m);
	HoeMath::Matrix4v w;
#ifdef _HOE_D3D9_
	D3DDevice()->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&w);
#endif
#ifdef _HOE_OPENGL_
	// consts
	w.Identity();
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)w);
#endif

	//HoeMath::Vector3 vv(0,0,0);
	//vv.Multiply(w);
	w.Multiply(m);
	w.Transpoze();
//	Con_Print(w);
/*
diRECT x:
2.402153 0.000000 -0.241019 169.857834
0.038399 2.383378 0.382707 171.245438
0.098568 -0.159334 0.982394 344.738098
0.098558 -0.159318 0.982295 345.703613
ogl:
2.402153 0.000000 -0.241019 169.857834
0.038399 2.383378 0.382707 171.245438
0.098568 -0.159334 0.982394 344.738098
0.098558 -0.159318 0.982295 345.703613

*/
//	exit(1);

	HoeMath::Vector4v zero(0,0,0,0);
#ifdef _HOE_D3D9_
	HRESULT hRes = D3DDevice()->SetVertexShader( m_shader);
	assert(hRes == S_OK);
	D3DDevice()->SetVertexShaderConstantF(  0, (float*)w.m, 4 );		// c0-c3 contains composite transform Matrix
	D3DDevice()->SetVertexShaderConstantF(  5, zero, 1 );	// c5   0,0,0,0

	for(uint i=0; i<17 && i < scene->GetLS()->GetNumActiveLights(); i++)
		{
			D3DDevice()->SetVertexShaderConstantF( i+10, scene->GetLS()->GetActiveLight(i)->GetPosition(), 1 );
			D3DDevice()->SetVertexShaderConstantF( i+40, scene->GetLS()->GetActiveLight(i)->GetColor(),  1 );
		}
#endif
#ifdef _HOE_OPENGL_
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, m_shader);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 0, &w._11 );
	glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 1, &w._21 );
	glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 2, &w._31 );
	glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 3, &w._41 );
	glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB,  5, zero);	// c5   0,0,0,0

	for(uint i=0; i<17 && i < scene->GetLS()->GetNumActiveLights(); i++)
		{
			glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i+10, scene->GetLS()->GetActiveLight(i)->GetPosition());
			glProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i+40, scene->GetLS()->GetActiveLight(i)->GetColor());
		}
#endif
	}
}

