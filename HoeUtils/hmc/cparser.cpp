
#include "StdAfx.h"
#include "cparser.h"
#include "mshader.h"
#include "parser.h"

void CParser::Error(CPError * err)
{
}

bool CParser::Parse()
{

	while (ScanLine() != NULL)
	{
		switch (lexln(NULL))
		{
		case P_NL:
			break;
		case C_STREAM:
			if (!ScanStream())
				return false;
			break;
		case C_INDEX:
			if (!ScanIndex())
				return false;
			break;
		case C_MATERIAL:
			if (!ScanMaterial())
				return false;
			break;
		case C_MODEL:
			if (!ScanModel())
				return false;
			break;
		default:
			return false;
		}
	}
	
	return true;
}

const char * CParser::ScanLine()
{
	const char * line = GetNextLine();
	if (line == NULL)
		return NULL;

	parse_load(line);
	return line;
}

bool CParser::ScanStream()
{
	std::string name;
	_FVF fvf;

	if (!ScanName(name))
		return false;

	if (!ScanFVF(fvf))
		return false;

	if (lexln(NULL) != P_NL)
		return false;

	CBaseStream * stream = CreateStream(name,fvf);
	if (stream == NULL)
		return false;

	VERTEX vert(fvf);

	int sv;
	
	do {
		if (ScanLine() == NULL)
			return false;

		sv = ScanVertex(vert);
		if (sv == 0)
			continue;
		if (sv == -1)
			return false;
		if (sv == 1)
		{
			stream->AddVertex(vert);
		}

	} while (sv != 2);

	stream->EndData();

	return true;
}

bool CParser::ScanMaterial()
{
	std::string name;

	if (!ScanName(name))
		return false;

	if (lexln(NULL) != P_NL)
		return false;



	CBaseMaterial * mat = CreateMaterial(name);
	if (mat == NULL)
		return false;

	int sv;
	
	do {
		CColor c;

		if (ScanLine() == NULL)
			return false;

		sv = lexln(NULL);
		switch (sv)
		{
		case M_AMBIENT:
			if (lexln(NULL)!=P_EQ || !ScanColor(&c))
				false;
			mat->SetColor(CBaseMaterial::MAmbient, &c);
			break;
		case M_DIFFUSE:
			if (lexln(NULL)!=P_EQ || !ScanColor(&c))
				false;
			mat->SetColor(CBaseMaterial::MDiffuse, &c);
			break;
		case M_SPECULAR:
			if (lexln(NULL)!=P_EQ || !ScanColor(&c))
				false;
			mat->SetColor(CBaseMaterial::MSpecular, &c);
			break;
		case M_TEXTURE: {
			p_value val;
			if (lexln(NULL)!=P_EQ || lexln(&val) != V_STRING)
				false;
			mat->SetTexture(val.str);
			} break;
		case M_ALPHATEST: {
			p_value val;
			if (lexln(NULL)!=P_EQ || lexln(&val) != V_BOOL)
				false;
			mat->SetPar(CBaseMaterial::MAlphaTest,val.i);
			} break;
		case E_MATERIAL:
			break;
		default:
			return false;
		};

	} while (sv != E_MATERIAL);

	//stream->EndData();

	return true;
}

bool CParser::ScanColor(CColor * c)
{
	p_value val;
	int t = lexln(&val);
	if (t == P_STT)
	{
		float valc[4] = { 1.f, 1.f, 1.f, 1.f };
		int i = 0;
		for (i=0;i < 4;i++)
		{
			int tc = lexln(&val);
			if (tc == V_FLOAT)
				valc[i] = val.f;
			else if (tc == V_INT)
				valc[i] = val.i * (1/255.f);
			else
				return false;

			tc = lexln(NULL);
			if (tc == P_END)
				break;
			else if (tc != P_SEP)
				return false;
		}
		c->alpha = (i >= 3) ? valc[i-3]: 1.f;
		c->red = (i >= 2) ? valc[i-2]: 1.f;
		c->green = (i >= 1) ? valc[i-1]: 1.f;
		c->blue = valc[i];


		return true;
	}
	else
		return false;
}

bool CParser::ScanName(std::string &name)
{
	p_value val;
	if (lexln(&val) != V_NAME)
		return false;;

	name = val.str;
	return true;
}

bool CParser::ScanFVF(_FVF &fvf)
{
	
	p_value val;

	// zkontrolovat zda nasleduje [
	if (lexln(&val) != P_STT || val.c != '[')
		return false;

	while (1)
	{
		// load value
		switch (lexln(NULL))
		{
		case F_XYZRHW:
			fvf.AddType('r'); break;
		case F_XYZ:
			fvf.AddType('p'); break;
		case F_NORMAL:
			fvf.AddType('n'); break;
		case F_TEX2:
			fvf.AddType('t'); break;
		default:
			return false;
		}

		int v = lexln(&val);
		if (v == P_PIPE)
			continue;

		if (v == P_END && val.c == ']')
			return true;
		else
			return false;
	}


}
// 1 = ok
// 0 = nic
// -1 = chyba
// 2 = end


int CParser::ScanVertex(VERTEX &vert)
{
	vert.Reset();

	while (1)
	{
		p_value val;

		switch (lexln(&val))
		{
		case V_HEX:
			vert.AddUnsigned(val.hex); break;
		case V_FLOAT:
			vert.AddFloat(val.f); break;
		case V_INT:
			vert.AddNum(val.i); break;
		case P_NL:
			if (vert.IsEmpty())
				return 0;
			else
				return -1;
		case E_STREAM:
			if (lexln(NULL) == P_NL)
				return 2;
			/* no break */
		default:
			return -1;
		}


		int v = lexln(NULL);

		if (v == P_SEP)
			continue;

		if (v == P_NL)
		{
			return 1;
		}
		else
			return -1;

	}

}

bool CParser::ScanModel(void)
{
	std::string name;
	if (!ScanName(name))
		return false;

	ModelShader * shader = CreateModelShader(name);

	while (1)
	{
		int h;

		if (ScanLine() == NULL)
			return false;

		switch (h = lexln(NULL))
		{
		case P_NL:
			break;
		case E_MODEL:
			return true;
		case MS_STREAMS:
			if (!ScanStreamList(shader))
				return false;
			break;
		case MS_INDICES:
			if (!ScanIndexList(shader))
				return false;
			break;
		case MS_INPUT:
			if (!ScanInputList(shader))
				return false;
			break;
		case MS_MATERIALS:
			if (!ScanMaterialList(shader))
				return false;
			break;
		case MS_STDRENDER:
			{
				InsParam param;
				if (!ScanParameters(&param) || param.GetNum() != 2)
					return false;
				shader->i_stdrender(&param);
			}
			break;
		default:
			return false;
		}

	}	

	return true;
}

void CBaseStream::AddVertex(VERTEX & vert)
{

}

void CBaseStream::EndData()
{

}

bool CParser::ScanIndex()
{
	std::string name;
	if (!ScanName(name))
		return false;

	if (lexln(NULL) != P_NL)
		return false;

	CBaseIndex * index = CreateIndex(name);
	if (index == NULL)
		return false;

	p_value val;

	while (1)
	{
		int h;

		if (ScanLine() == NULL)
			return false;

		switch (h = lexln(&val))
		{
		case E_INDEX:
			index->EndData();
			return true;
		case V_INT:
			index->AddIndex(val.i); break;
		case V_HEX:
			index->AddIndex(val.hex); break;
		case P_NL:
			break;
		default:
			return false;
		}

		if (h == P_NL)
			continue;

		while ((h = lexln(&val)) == P_SEP)
		{
			switch (lexln(&val))
			{
			case V_INT:
				index->AddIndex(val.i); break;
			case V_HEX:
				index->AddIndex(val.hex); break;
			default:
				return false;
			}
		}

		if (h != P_NL)
			return false;
	}

}

void CBaseIndex::EndData()
{

}

void CBaseIndex::AddIndex(unsigned short ind)
{

}

void CBaseMaterial::SetColor(Par p, CColor * c)
{

}

void CBaseMaterial::SetTexture(const char * texture)
{

}

void CBaseMaterial::SetPar(Par p, int val)
{
}

bool CParser::ScanParameters(InsParam *par)
{
	p_value val;

	while (1)
	{
		// load value
		switch (lexln(&val))
		{
		case V_INT:
			break;
		case V_HEX:
			break;
		case V_FLOAT:
			break;
		case MS_IPARAM:
			break;
		case MS_FPARAM:
			break;
		case P_NL:
			return true;
		default:
			return false;
		}
	}

}

bool CParser::ScanStreamList(ModelShader *shader)
{
	p_value val;

	while (1)
	{
		// load value
		if (lexln(&val) != V_NAME)
			return false;

		shader->AddDefStream(val.str);

		int v = lexln(&val);
		if (v == P_SEP)
			continue;

		if (v == P_NL)
			return true;
		else
			return false;
	}
}

bool CParser::ScanIndexList(ModelShader *shader)
{

	p_value val;

	while (1)
	{
		// load value
		if (lexln(&val) != V_NAME)
			return false;

		shader->AddDefIndex(val.str);

		int v = lexln(&val);
		if (v == P_SEP)
			continue;

		if (v == P_NL)
			return true;
		else
			return false;
	}
}

bool CParser::ScanMaterialList(ModelShader *shader)
{

	p_value val;

	while (1)
	{
		// load value
		if (lexln(&val) != V_NAME)
			return false;

		shader->AddDefMaterial(val.str);

		int v = lexln(&val);
		if (v == P_SEP)
			continue;

		if (v == P_NL)
			return true;
		else
			return false;
	}
}

bool CParser::ScanInputList(ModelShader *shader)
{
	p_value val;

	while (1)
	{
		// load value
		switch (lexln(&val))
		{
		case MS_FPARAM:
			shader->AddDefFloat(val.i);
			break;
		case MS_IPARAM:
			shader->AddDefInt(val.i);
			break;
		default:
			return false;
		}

		int v = lexln(&val);
		if (v == P_SEP)
			continue;

		if (v == P_NL)
			return true;
		else
			return false;
	}
}
