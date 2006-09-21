
#include "system.h"
#include "utils.h"
#include "ref.h"
#include "texture_system.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_material.h"
#include "hoe_model.h"
#include "hoe3d_math.h"
#include "shared.h"
#include "states.h"
#include "hoe_time.h"

using namespace HoeMath;

HoeModel::HoeModel(int nums, int numi, int numm)
{
	m_stream = new HoeStream*[nums];
	m_index = new HoeIndex*[numi];
	m_mat = new HoeMaterial*[numm];
	m_num_stream = 0;
	m_num_index = 0;
	m_num_mat = 0;

	//m_shader.Load();
}

void HoeModel::AddDefStream(HoeStream * stream)
{
	if (m_num_stream == 0)
		m_box.Set(*stream->GetBounding());
	else
		m_box.Add(*stream->GetBounding());
	m_stream[m_num_stream++] = stream;
}

void HoeModel::AddDefIndex(HoeIndex * index)
{
	m_index[m_num_index++] = index;
}

void HoeModel::AddDefMaterial(HoeMaterial * mat)
{
	m_mat[m_num_mat++] = mat;
}

bool HoeModel::GetParameter(const char * name, THoeParameter * par)
{
	assert(par);
	if (strcmp("boundbox",name) == 0)
	{
		par->box.front = m_box.min.z;
		par->box.back = m_box.max.z;
		par->box.left = m_box.min.x;
		par->box.right = m_box.max.x;
		par->box.top = m_box.max.y;
		par->box.bottom = m_box.min.y;
		return true;
	}
	return false;
}

void HoeModel::Render(const HoeScene * scene, float f) const
{
	if (m_num_stream == m_num_index)
	{
		// pozmenovaci funkce na render
		for (uint i=0;i<m_num_stream;i++)
		{
			if (i < m_num_mat) m_mat[i]->Setup();
			//m_shader.Setup(scene);
			Ref::DrawStdObject(m_stream[i],m_index[i]);
		}
	}
	else if (m_num_stream > 1 && m_num_index > 0)
	{
		// vykresleni aktualniho streamu
		if (m_num_mat > 0)
			m_mat[0]->Setup();
		// prepocitat frame na snimek
		uint frame = f * (m_num_stream-1);
		Ref::DrawStdObject(m_stream[frame%m_num_stream],m_index[0]);
	}
}

