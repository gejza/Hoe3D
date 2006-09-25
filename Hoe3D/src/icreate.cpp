

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "config.h"
#include "texture_system.h"
#include "material_system.h"
#include "light_system.h"
#include "model_loader.h"
#include "model_generator.h"
#include "resmgr.h"
#include "camera.h"
#include "hoe_model.h"
#include "hoe_stream.h"
#include "hoe3d_math.h"
#include "sound.h"
#include "states.h"
#include "hoe_time.h"
#include "hoe2d.h"
#include "hoe_font.h"
#include "particle_emitor.h"
#include "hoe.h"
#include "hoe_tokens.h"

extern "C" {

void parse_load(const char *str); 
int parse();
const char *parser_getstring();

}

namespace icreate {

	static char * get_string(char * buff)
	{
		const char * str = parser_getstring();
		if (str[0] == '\'' || str[0] == '\"')
		{
			strcpy(buff,str + 1);
			int l = strlen(buff);
			buff[l - 1] = '\0'; // remove last character
		}
		else
			strcpy(buff,str);
		return buff;
	}

	static bool get_param(HoeLog *& log, int & flags)
	{
		int token;
		token = parse();
		while (token)
		{
			if (HOE_T_IS_LOGTYPE(token))
			{
				char logname[256] = {0};
				if (parse())
					get_string(logname);
				switch (token)
				{
				case HOE_T_CON:
					log = new HoeLogConsole(logname);
					break;
				case HOE_T_FILE:
					log = new HoeLogFile(logname);
					break;
				}
			}
			else if (token == HOE_T_DUMP)
			{
				flags = 1;
			}
			else
			{
				Con_Print("parse error: unknown '%s'",parser_getstring());
				return false;
			}
			token = parse();
		}
		return true;
	}

	static IHoeModel * CreateModel()
	{
		HoeLog * log = NULL;
		int flags = 0;
		char modelname[512];
		if (HOE_T_IS_STRING(parse())) {
			get_string(modelname);
		}
		else {
			Con_Print("parse error: missing model name");
			return NULL;
		}

		get_param(log, flags);

		ModelLoader ml(log, flags);
		HoeModel * m = ml.LoadModel(modelname, false);
		if (log) delete log;
		return m;
	}

	static IHoePicture * CreatePicture()
	{
		HoeLog * log = NULL;
		int flags = 0;
		char picturename[256];
		if (HOE_T_IS_STRING(parse())) {
			get_string(picturename);
		}
		else {
			Con_Print("parse error: missing picture name");
			return NULL;
		}

		get_param(log, flags);

		IHoePicture * pic = Get2D()->CreatePicture(picturename, log);
		if (log) delete log;
		return pic;
	}

	static IHoeFont * CreateFont()
	{
		char fontname[256];
		int height;
		HoeFont * f = NULL;

		if (HOE_T_IS_STRING(parse())) {
			get_string(fontname);
		}
		else {
			Con_Print("parse error: missing font name");
			return NULL;
		}

		if (parse() == HOE_T_INT) {
			height = atoi(parser_getstring());
		}
		else {
			Con_Print("parse error: missing font height");
			return NULL;
		}

		f = new HoeFont(fontname,height,1.f, 0);
		if (!f->Init())
			return NULL;
		return f;
	}

	static IHoeSound * CreateSound()
	{
		char soundname[256];
		if (HOE_T_IS_STRING(parse())) {
			get_string(soundname);
		}
		else {
			Con_Print("parse error: missing sound name");
			return NULL;
		}

		return GetSound()->GetSound(soundname);
	}
	static IHoeParticleEmitor * CreateParticle()
	{
		/*char soundname[256];
		if (HOE_T_IS_STRING(parse())) {
			get_string(soundname);
		}
		else {
			Con_Print("parse error: missing sound name");
			return NULL;
		}*/

		//return GetSound()->GetSound(soundname);
		ParticleEmitor * pe = new ParticleEmitor();
		return pe;
	}
	////////////////////////////////////////////////////////
	static IHoeModel * GenModel()
	{
		HoeLog * log = NULL;
		int flags = 0;
		int type = parse();
		float size = 1;
		if (parse() == HOE_T_INT) {
			size = (float)atof(parser_getstring());
		}
		else {
			Con_Print("parse error: missing box size");
			return NULL;
		}		
		get_param(log, flags);
		ModelGenerator mg(log, flags);
		HoeModel * ret = NULL;
		switch (type)
		{
		case HOE_T_BOX:
			ret = mg.GenBox(size);
			break;
		case HOE_T_SPHERE:

			break;
		};
		return ret;
	}
}

IHoeResource * HOEAPI Hoe3D::Create(const char * str)
{
	IHoeResource * ret = NULL;

	parse_load(str);	

	switch (parse())
	{
	case HOE_T_MODEL:
		ret = icreate::CreateModel();
		break;
	case HOE_T_PICTURE:
		ret = icreate::CreatePicture();
		break;
	case HOE_T_FONT:
		ret = icreate::CreateFont();
		break;
	case HOE_T_SOUND:
		ret = icreate::CreateSound();
		break;
	case HOE_T_PARTICLE:
		ret = icreate::CreateParticle();
		break;
	case HOE_T_GENERATE:
		switch (parse())
		{
		case HOE_T_MODEL:
			ret = icreate::GenModel();
			break;
		/*case HOE_T_PICTURE:
			ret = icreate::CreatePicture();
			break;
		case HOE_T_FONT:
			ret = icreate::CreateFont();
			break;
		case HOE_T_SOUND:
			ret = icreate::CreateSound();
			break;*/
		default:
			Con_Print("error parse create line '%s'",str);
		};
		break;
	default:
		Con_Print("error parse create line '%s'",str);
	};

	if (parse())
	{
		Con_Print("warning: '%s' not parsed.",parser_getstring());
	}

	return ret;
} 

