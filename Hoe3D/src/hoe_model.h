
#ifndef _HOE_MODEL_
#define _HOE_MODEL_

//#include "hoe3d_ref.h"
#include "../include/hoeinterfaces.h"
#include "model_shader.h"

#include "hoe_material.h"
#include "shader_vertex.h"

class HoeIndex;
class HoeStream;
class HoeMaterial;

class HoeModel : public IHoeModel
{
	HoeIndex ** m_index;
	HoeStream ** m_stream;
	HoeMaterial **m_mat;
	uint m_num_stream;
	uint m_num_index;
	uint m_num_mat;


	HoeMath::BoundingBox m_box;
	//HoeVertexShader m_shader;
	// atd..
	// type, params, co delat kdyz projde (dalsi uroven)
protected:
	void AddDefStream(HoeStream * stream);
	void AddDefIndex(HoeIndex * index);
	void AddDefMaterial(HoeMaterial * mat);
public:	
	HoeModel(int numr);
	~HoeModel();

	// info functions
	uint GetNumStreams() const { return m_num_stream; }
	uint GetNumIndex() const { return m_num_index; }
	uint GetNumMats() const { return m_num_mat; }

	void Render(const HoeScene * scene) const ;
	const HoeMath::BoundingBox & GetBound() const { return m_box; }

	virtual bool HOEAPI GetParameter(const char * name, THoeParameter * parameter);

	friend class ModelLoader;
	friend class ModelGenerator;
};


#endif // _HOE_MODEL_
