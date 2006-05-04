

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "config.h"
#include "texture_system.h"
#include "material_system.h"
#include "light_system.h"
#include "model_loader.h"
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
#include "hoe.h"
#include "hoe_tokens.h"

extern "C" {

void parse_load(const char *str); 
int parse();
const char *parser_getstring();

}

namespace icreate {

	char * get_string(char * buff)
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

	IHoeModel * CreateModel()
	{
		HoeLog * log = NULL;
		char modelname[512];
		int token;
		if (HOE_T_IS_STRING(parse())) {
			get_string(modelname);
		}
		else {
			Con_Print("parse error: missing model name");
			return NULL;
		}

		token = parse();
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
				log = NULL;//new HoeLogFile(logname);
				break;
			}
		}
		else if (token)
		{
			Con_Print("parse error: unknown '%s'",parser_getstring());
		}

		ModelLoader ml(log);
		return ml.LoadModel(modelname);

	}

	IHoePicture * CreatePicture()
	{
		char picturename[256];
		if (HOE_T_IS_STRING(parse())) {
			get_string(picturename);
		}
		else {
			Con_Print("parse error: missing picture name");
			return NULL;
		}

		return Get2D()->CreatePicture(picturename);
	}

	IHoeFont * CreateFont()
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

		f = new HoeFont(fontname,height,0);
		if (!f->Init())
			return NULL;
		return f;
	}

	IHoeSound * CreateSound()
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
	default:
		Con_Print("error parse create line '%s'",str);
	};

	if (parse())
	{
		Con_Print("warning: '%s' not parsed.",parser_getstring());
	}

	return ret;
} 

