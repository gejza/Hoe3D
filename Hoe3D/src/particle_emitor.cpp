

#include "system.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe3d_math.h"
#include "particle_emitor.h"
#include "texture_system.h"
#include "states.h"
#include "hoe_time.h"

struct PartsVertex
{
	HoeMath::VECTOR3 pos;
	dword color;
	HoeMath::VECTOR2 tex;
};

static dword palete[] = { 0xff8080, 0xffb080, 0xffff80, 0xb0ff80,
0x80ff80, 0x80ffb0, 0x80ffff, 0x80b0ff,
0x8080ff, 0xb080ff, 0xff80ff, 0xff80b0 };

ParticleEmitor::ParticleEmitor() : m_stream(true)
{
	if (!Create(1500)) assert(!"failed");
}

ParticleEmitor::~ParticleEmitor()
{
}

const float xspeed = 1.f;
const float yspeed = 30.f;

bool ParticleEmitor::Create(uint num_part)
{
	m_part_count = num_part;
	// inicializace partiklu
	for (uint i=0;i < num_part;i++)
	{
		Particle & p = m_parts.Add();
		p.life = 1.f;
		p.fade = float(rand()%100)/1000.0f+0.003f;// Rychlost stárnutí
		p.color = palete[i*12/num_part];
		p.pos.Set(0,0,0);
		p.posi.Set(xspeed+float((rand()%60)-32.0f),
			yspeed+float((rand()%60)-30.0f),
			float((rand()%60)-30.0f));
		p.posg.Set(0,0.8f,0);
	}

	m_tex = GetTextureSystem()->GetTexture("Particle");

	// inicializace streamu a indexu
	if (!m_stream.Create(num_part*4, "pdt", sizeof(PartsVertex)*num_part*4))
		return false;

	if (!m_index.Create(num_part*6))
		return false;
	word * w = m_index.Lock();
	for (uint i=0;i < num_part;i++)
	{
		*w++ = i*4+0;
		*w++ = i*4+1;
		*w++ = i*4+2;
		*w++ = i*4+2;
		*w++ = i*4+1;
		*w++ = i*4+3;
	}
	m_index.Unlock();
	return true;
}

void ParticleEmitor::Update(float time)
{
}

const float halfsize = 1.7f;

void ParticleEmitor::Render()
{

	// update particles
	PartsVertex * v = (PartsVertex*)m_stream.Lock();
	for (uint i=0;i<m_part_count;i++)
	{
		Particle & p = m_parts.Get(i);
		const float hh = halfsize / (1.f-(p.life*0.8f));
		v[0].pos = p.pos + HoeMath::VECTOR3(hh,hh,0);
		v[1].pos = p.pos + HoeMath::VECTOR3(-hh,hh,0);
		v[2].pos = p.pos + HoeMath::VECTOR3(hh,-hh,0);
		v[3].pos = p.pos + HoeMath::VECTOR3(-hh,-hh,0);
		v[0].color = v[1].color = v[2].color = v[3].color = byte(p.life * 0xff) << 24 | p.color;
		v[0].tex.x = 1.f; v[0].tex.y = 1.f;
		v[1].tex.x = 0.f; v[1].tex.y = 1.f;
		v[2].tex.x = 1.f; v[2].tex.y = 0.f;
		v[3].tex.x = 0.f; v[3].tex.y = 0.f;
		v += 4;

		// update
		p.pos+= p.posi / 200.f;
		p.posi+=p.posg;
		p.life -= p.fade;
		if (p.life < 0.f)
		{
			p.life = 1.f;
			p.fade = float(rand()%100)/1000.0f+0.003f;// Rychlost stárnutí
			p.pos.Set(0,0,float((rand()%30)-15.f)/10.f);
			p.posi.Set(float((rand()%30)-15.0f),
				yspeed+float((rand()%60)-30.0f),
				float((rand()%30)-15.0f));
		}
	}
	m_stream.Unlock();
	GetStates()->EnableTexture();
	GetTextureSystem()->SetTexture(0,m_tex);
#ifdef _HOE_D3D_
	D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

    D3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    D3DDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	D3DDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3DDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    D3DDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
    D3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); 
#endif

	Ref::DrawStdObject(&m_stream,&m_index);
}



