
#ifndef _SHARED_H_
#define _SHARED_H_

#include <assert.h>

class Hoe3D;
class Config;
class Ref;
class ResourceMgr;
class XHoeFS;
class TextureSystem;
class MaterialSystem;
class Hoe2D;
class HoeStates;
class HoeMap;
class InfoSystem;
class HoeCursor;
class LightSystem;
class Physics;
class SoundSystem;
class InfoSystem;
class HoeInput;
class CmdExec;


namespace shared {
	extern Hoe3D * hoe3d;
	extern Config * config;
	extern Ref * ref;
	extern ResourceMgr * resmgr;
	extern XHoeFS * fs;
	extern TextureSystem * ts;
	extern HoeStates * states;
	extern MaterialSystem * material;
	extern Hoe2D * hoe2d;
	extern Physics * physics;
	extern SoundSystem * sound;
	extern InfoSystem * info;
	extern HoeInput * input;
	extern CmdExec * exec;
}

#define SHARED_PTR(ptr) { assert(shared::ptr);return shared::ptr;}
#define SET_SHARED_PTR(ptr) { assert(shared::ptr == NULL); shared::ptr = this; }
#define UNSET_SHARED_PTR(ptr) { assert(shared::ptr == this); shared::ptr = NULL; }

inline Hoe3D * GetHoe3D() SHARED_PTR(hoe3d)
inline Config * GetConfig() SHARED_PTR(config)
inline Ref * GetRef() SHARED_PTR(ref)
inline ResourceMgr * GetResMgr() SHARED_PTR(resmgr)
inline XHoeFS * GetFileSystem() SHARED_PTR(fs)
inline TextureSystem * GetTextureSystem() SHARED_PTR(ts)
inline HoeStates * GetHoeStates() SHARED_PTR(states)
inline MaterialSystem * GetMaterialSystem() SHARED_PTR(material)
inline Hoe2D * Get2D() SHARED_PTR(hoe2d)
inline Physics * GetPhysics() SHARED_PTR(physics)
inline SoundSystem * GetSound() SHARED_PTR(sound)
inline InfoSystem * GetInfo() SHARED_PTR(info)
inline HoeInput * GetInput() SHARED_PTR(input) 
inline CmdExec * GetExec() SHARED_PTR(exec)

inline bool IfExistHoe3DInstance() { return (shared::hoe3d != 0); }
inline bool IsInputLoaded() { return (shared::input != 0); }
inline bool IsSoundLoaded() { return (shared::sound != 0); }

#endif // _SHARED_H_
