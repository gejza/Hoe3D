
#ifndef _SHARED_H_
#define _SHARED_H_

#include <HoeCore/hoe_types.h>

class HoeEngine;
class Config;
class Ref;
class ResourceMgr;
namespace HoeRes {
class XResourceMgr;
};
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
class MainCodePage;

namespace shared {
	extern HoeEngine * hoe;
	extern Config * config;
	extern Ref * ref;
	extern HoeRes::XResourceMgr * resmgr;
	extern TextureSystem * ts;
	extern HoeStates * states;
	extern MaterialSystem * material;
	extern Hoe2D * hoe2d;
	extern Physics * physics;
	extern SoundSystem * sound;
	extern InfoSystem * info;
	extern HoeInput * input;
	extern CmdExec * exec;
	extern MainCodePage * codepage;
}

#define SHARED_PTR(ptr) { hoe_assert(shared::ptr);return shared::ptr;}
#define SET_SHARED_PTR(ptr) { hoe_assert(shared::ptr == NULL); shared::ptr = this; }
#define UNSET_SHARED_PTR(ptr) { hoe_assert(shared::ptr == this); shared::ptr = NULL; }

inline HoeEngine * GetHoe() SHARED_PTR(hoe)
inline Config * GetConfig() SHARED_PTR(config)
inline Ref * GetRef() SHARED_PTR(ref)
inline HoeRes::XResourceMgr * GetResMgr() SHARED_PTR(resmgr)
inline TextureSystem * GetTextureSystem() SHARED_PTR(ts)
inline HoeStates * GetStates() SHARED_PTR(states)
inline MaterialSystem * GetMaterialSystem() SHARED_PTR(material)
inline Hoe2D * Get2D() SHARED_PTR(hoe2d)
inline Physics * GetPhysics() SHARED_PTR(physics)
inline SoundSystem * GetSound() SHARED_PTR(sound)
inline InfoSystem * GetInfo() SHARED_PTR(info)
inline HoeInput * GetInput() SHARED_PTR(input) 
inline CmdExec * GetExec() SHARED_PTR(exec)
inline MainCodePage * GetCodePage() SHARED_PTR(codepage)

inline bool IfExistHoeInstance() { return (shared::hoe != 0); }
inline bool IsInputLoaded() { return (shared::input != 0); }
inline bool IsSoundLoaded() { return (shared::sound != 0); }

#endif // _SHARED_H_
