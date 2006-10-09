
#ifndef _PARTICLE_EMITOR_H_
#define _PARTICLE_EMITOR_H_

#include <hoe_structures.h>
#include "hoe_index.h"
#include "hoe_stream.h"

class HoeTexture;

struct Particle
{
	float life;
	float fade;
	HoeMath::VECTOR3 pos;
	dword color;
	HoeMath::VECTOR3 posi;
	HoeMath::VECTOR3 posg;
};

class ParticleEmitor : public IHoeParticleEmitor
{
protected:
	HoeStream m_stream;
	HoeIndex m_index;
	HoeTexture * m_tex;
	HoeCore::Set<Particle> m_parts;
	uint m_part_count;
	bool m_generate;
public:
	ParticleEmitor();
	virtual ~ParticleEmitor();
	bool Create(uint num_part);
	void Update(float time);
	void Render();
	virtual void Start();
	virtual void Stop();
};

#endif // _PARTICLE_EMITOR_H_

