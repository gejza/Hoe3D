
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref_utils.h"
#include "ref_d3d9.h"

const char * GetRenderStateString(dword type)
{
	switch (type)
	{
	case 7:
		return "D3DRS_ZENABLE";/* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	case 8:
		return "D3DRS_FILLMODE";/* D3DFILLMODE */
	case 9:
		return "D3DRS_SHADEMODE";/* D3DSHADEMODE */
	case 14:
		return "D3DRS_ZWRITEENABLE";/* TRUE to enable z writes */
	case 15:
		return "D3DRS_ALPHATESTENABLE";/* TRUE to enable alpha tests */
	case 16:
		return "D3DRS_LASTPIXEL";/* TRUE for last-pixel on lines */
	case 19:
		return "D3DRS_SRCBLEND";/* D3DBLEND */
	case 20:
		return "D3DRS_DESTBLEND";/* D3DBLEND */
	case 22:
		return "D3DRS_CULLMODE";/* D3DCULL */
	case 23:
		return "D3DRS_ZFUNC";/* D3DCMPFUNC */
	case 24:
		return "D3DRS_ALPHAREF";/* D3DFIXED */
	case 25:
		return "D3DRS_ALPHAFUNC";/* D3DCMPFUNC */
	case 26:
		return "D3DRS_DITHERENABLE";/* TRUE to enable dithering */
	case 27:
		return "D3DRS_ALPHABLENDENABLE";/* TRUE to enable alpha blending */
	case 28:
		return "D3DRS_FOGENABLE";/* TRUE to enable fog blending */
	case 29:
		return "D3DRS_SPECULARENABLE";/* TRUE to enable specular */
	case 34:
		return "D3DRS_FOGCOLOR";/* D3DCOLOR */
	case 35:
		return "D3DRS_FOGTABLEMODE";/* D3DFOGMODE */
	case 36:
		return "D3DRS_FOGSTART";/* Fog start (for both vertex and pixel fog) */
	case 37:
		return "D3DRS_FOGEND";/* Fog end      */
	case 38:
		return "D3DRS_FOGDENSITY";/* Fog density  */
	case 48:
		return "D3DRS_RANGEFOGENABLE";/* Enables range-based fog */
	case 52:
		return "D3DRS_STENCILENABLE";/* BOOL enable/disable stenciling */
	case 53:
		return "D3DRS_STENCILFAIL";/* D3DSTENCILOP to do if stencil test fails */
	case 54:
		return "D3DRS_STENCILZFAIL";/* D3DSTENCILOP to do if stencil test passes and Z test fails */
	case 55:
		return "D3DRS_STENCILPASS";/* D3DSTENCILOP to do if both stencil and Z tests pass */
	case 56:
		return "D3DRS_STENCILFUNC";/* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	case 57:
		return "D3DRS_STENCILREF";/* Reference value used in stencil test */
	case 58:
		return "D3DRS_STENCILMASK";/* Mask value used in stencil test */
	case 59:
		return "D3DRS_STENCILWRITEMASK";/* Write mask applied to values written to stencil buffer */
	case 60:
		return "D3DRS_TEXTUREFACTOR";/* D3DCOLOR used for multi-texture blend */
	case 128:
		return "D3DRS_WRAP0";/* wrap for 1st texture coord. set */
	case 129:
		return "D3DRS_WRAP1";/* wrap for 2nd texture coord. set */
	case 130:
		return "D3DRS_WRAP2";/* wrap for 3rd texture coord. set */
	case 131:
		return "D3DRS_WRAP3";/* wrap for 4th texture coord. set */
	case 132:
		return "D3DRS_WRAP4";/* wrap for 5th texture coord. set */
	case 133:
		return "D3DRS_WRAP5";/* wrap for 6th texture coord. set */
	case 134:
		return "D3DRS_WRAP6";/* wrap for 7th texture coord. set */
	case 135:
		return "D3DRS_WRAP7";/* wrap for 8th texture coord. set */
	case 136:
		return "D3DRS_CLIPPING";
	case 137:
		return "D3DRS_LIGHTING";
	case 139:
		return "D3DRS_AMBIENT";
	case 140:
		return "D3DRS_FOGVERTEXMODE";
	case 141:
		return "D3DRS_COLORVERTEX";
	case 142:
		return "D3DRS_LOCALVIEWER";
	case 143:
		return "D3DRS_NORMALIZENORMALS";
	case 145:
		return "D3DRS_DIFFUSEMATERIALSOURCE";
	case 146:
		return "D3DRS_SPECULARMATERIALSOURCE";
	case 147:
		return "D3DRS_AMBIENTMATERIALSOURCE";
	case 148:
		return "D3DRS_EMISSIVEMATERIALSOURCE";
	case 151:
		return "D3DRS_VERTEXBLEND";
	case 152:
		return "D3DRS_CLIPPLANEENABLE";
	case 154:
		return "D3DRS_POINTSIZE";/* float point size */
	case 155:
		return "D3DRS_POINTSIZE_MIN";/* float point size min threshold */
	case 156:
		return "D3DRS_POINTSPRITEENABLE";/* BOOL point texture coord control */
	case 157:
		return "D3DRS_POINTSCALEENABLE";/* BOOL point size scale enable */
	case 158:
		return "D3DRS_POINTSCALE_A";/* float point attenuation A value */
	case 159:
		return "D3DRS_POINTSCALE_B";/* float point attenuation B value */
	case 160:
		return "D3DRS_POINTSCALE_C";/* float point attenuation C value */
	case 161:
		return "D3DRS_MULTISAMPLEANTIALIAS      ";// BOOL - set to do FSAA with multisample buffer
	case 162:
		return "D3DRS_MULTISAMPLEMASK";// DWORD - per-sample enable/disable
	case 163:
		return "D3DRS_PATCHEDGESTYLE";// Sets whether patch edges will use float style tessellation
	case 165:
		return "D3DRS_DEBUGMONITORTOKEN";// DEBUG ONLY - token to debug monitor
	case 166:
		return "D3DRS_POINTSIZE_MAX";/* float point size max threshold */
	case 167:
		return "D3DRS_INDEXEDVERTEXBLENDENABLE";
	case 168:
		return "D3DRS_COLORWRITEENABLE";// per-channel write enable
	case 170:
		return "D3DRS_TWEENFACTOR";// float tween factor
	case 171:
		return "D3DRS_BLENDOP";// D3DBLENDOP setting
	case 172:
		return "D3DRS_POSITIONDEGREE";// NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
	case 173:
		return "D3DRS_NORMALDEGREE";// NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
	case 174:
		return "D3DRS_SCISSORTESTENABLE";
	case 175:
		return "D3DRS_SLOPESCALEDEPTHBIAS";
	case 176:
		return "D3DRS_ANTIALIASEDLINEENABLE";
	case 178:
		return "D3DRS_MINTESSELLATIONLEVEL";
	case 179:
		return "D3DRS_MAXTESSELLATIONLEVEL";
	case 180:
		return "D3DRS_ADAPTIVETESS_X";
	case 181:
		return "D3DRS_ADAPTIVETESS_Y";
	case 182:
		return "D3DRS_ADAPTIVETESS_Z";
	case 183:
		return "D3DRS_ADAPTIVETESS_W";
	case 184:
		return "D3DRS_ENABLEADAPTIVETESSELLATION";
	case 185:
		return "D3DRS_TWOSIDEDSTENCILMODE";/* BOOL enable/disable 2 sided stenciling */
	case 186:
		return "D3DRS_CCW_STENCILFAIL";/* D3DSTENCILOP to do if ccw stencil test fails */
	case 187:
		return "D3DRS_CCW_STENCILZFAIL";/* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	case 188:
		return "D3DRS_CCW_STENCILPASS";/* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
	case 189:
		return "D3DRS_CCW_STENCILFUNC";/* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	case 190:
		return "D3DRS_COLORWRITEENABLE1";/* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	case 191:
		return "D3DRS_COLORWRITEENABLE2";/* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	case 192:
		return "D3DRS_COLORWRITEENABLE3";/* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	case 193:
		return "D3DRS_BLENDFACTOR";/* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
	case 194:
		return "D3DRS_SRGBWRITEENABLE";/* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
	case 195:
		return "D3DRS_DEPTHBIAS";
	case 198:
		return "D3DRS_WRAP8";/* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
	case 199:
		return "D3DRS_WRAP9";
	case 200:
		return "D3DRS_WRAP10";
	case 201:
		return "D3DRS_WRAP11";
	case 202:
		return "D3DRS_WRAP12";
	case 203:
		return "D3DRS_WRAP13";
	case 204:
		return "D3DRS_WRAP14";
	case 205:
		return "D3DRS_WRAP15";
	case 206:
		return "D3DRS_SEPARATEALPHABLENDENABLE";/* TRUE to enable a separate blending function for the alpha channel */
	case 207:
		return "D3DRS_SRCBLENDALPHA";/* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	case 208:
		return "D3DRS_DESTBLENDALPHA";/* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	case 209:
		return "D3DRS_BLENDOPALPHA";/* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	default:
		return NULL;
	};
}

void ListState()
{
	Con_Print("List render states");
	for (dword dw=0;dw<210;dw++)
	{
		if (GetRenderStateString(dw))
		{
			DWORD state;
			D3DDevice()->GetRenderState((D3DRENDERSTATETYPE)dw, &state);
			Con_Print("%s = %d", GetRenderStateString(dw), state);
		}
	}
}


