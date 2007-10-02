

#include "StdAfx.h"
#include "ref.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include <hoe_math.h>
#include "particle_emitor.h"
#include "texture_system.h"
#include "states.h"
#include "hoe_time.h"
#include "camera.h"

struct PartsVertex
{
	HoeMath::Vector3 pos;
	dword color;
	HoeMath::Vector2 tex;
};

static dword palete[] = { 0xff8080, 0xffb080, 0xffff80, 0xb0ff80,
0x80ff80, 0x80ffb0, 0x80ffff, 0x80b0ff,
0x8080ff, 0xb080ff, 0xff80ff, 0xff80b0 };

ParticleEmitor::ParticleEmitor() : m_stream(true)
{
	if (!Create(50)) assert(!"failed");
	m_generate = 0;
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

	m_tex = GetTextureSystem()->GetTexture(T("Particle"));

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

void ParticleEmitor::Start()
{
	m_generate = true;
}

void ParticleEmitor::Stop()
{
	m_generate = false;
}

const float halfsize = 1.7f;

void ParticleEmitor::Render(const HoeCamera * cam)
{
	HoeMath::Vector3 look = cam->GetLook();
	look = look * -1.f;
	look.Normalize();
	// vytvorit 4 vektory
	const HoeMath::Vector3 a(look.z,look.y,-look.x);
	const HoeMath::Vector3 b = HoeMath::HoeCross(look, a);
	const HoeMath::Vector3 d = a * -1.f;
	const HoeMath::Vector3 c = b * -1.f;

	// update particles
	PartsVertex * v = (PartsVertex*)m_stream.Lock();
	for (uint i=0;i<m_part_count;i++)
	{
		Particle & p = m_parts.Get(i);
		const float hh = halfsize / (1.f-(p.life*0.8f));
		// renderovani podle kamery
		
		v[0].pos = p.pos + (a * hh);
		v[1].pos = p.pos + (b * hh);
		v[2].pos = p.pos + (c * hh);
		v[3].pos = p.pos + (d * hh);
		dword l = p.life * 0xff;
		dword c = byte(l) << 24;
		c = c | p.color;
		v[0].color = v[1].color = v[2].color = v[3].color = c;
		v[0].tex.x = 1.f; v[0].tex.y = 1.f;
		v[1].tex.x = 0.f; v[1].tex.y = 1.f;
		v[2].tex.x = 1.f; v[2].tex.y = 0.f;
		v[3].tex.x = 0.f; v[3].tex.y = 0.f;
		v += 4;

		// update
		p.pos+= p.posi / 200.f;
		p.posi+=p.posg;
		p.life -= p.fade;
		if (p.life < 0.f && m_generate)
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
	//D3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

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
    D3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); 
#endif

	Ref::DrawStdObject(&m_stream,&m_index);
#ifdef _HOE_D3D_
    D3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); 
#endif
}



