
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "hoe_format.h"
#include "../include/hfmt/htex_file.h"
#include "texture_loader.h"
#include "texture_converter.h"

static void ReadPng(png_structp p, png_bytep buff, png_size_t s)
{
	reinterpret_cast<TextureLoader*>(p->io_ptr)->GetData(buff, s);
}

static void Err(png_structp, png_const_charp msg)
{
	Con_Print(msg);
}

static void Warn(png_structp, png_const_charp msg)
{
	Con_Print(msg);
}

//////////////////////////////////////////

TextureConverterPNG::TextureConverterPNG(TextureLoader * l,HoeLog * log) : TextureConverter(l,log)
{
}

TextureConverterPNG::~TextureConverterPNG()
{
}

bool TextureConverterPNG::Run()
{
	/*!!!*/ // navratove hodnoty u obrazku
	unsigned char buf[4];
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 

	assert(m_loader);
	LOG(m_log)->Log("Load PNG with libpng"PNG_LIBPNG_VER_STRING);

	m_loader->GetData(buf, 4);
	
	if (png_sig_cmp(buf, (png_size_t)0, 4))
	{
		LOG(m_log)->Log("error check header");
		return false;
	}

	png_debug(3, "Msg: ");

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,m_log,Err,Warn);

	if (png_ptr == NULL)
	{
		LOG(m_log)->Log("png_create_read_struct return NULL");
		return false;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return false;
	} 

	png_set_read_fn(png_ptr, (void *)m_loader, ReadPng); 

	png_set_sig_bytes(png_ptr, 4);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, int_p_NULL, int_p_NULL); 

	m_width = width;
	m_height = height;

	m_format = GetSourceFormat();

	GetConfig()->CheckTexture(m_width,m_height,m_format);

	return true;
}

bool TextureConverterPNG::Get(byte * p,dword pitch)
{
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 
	int number_passes = png_set_interlace_handling(png_ptr);
	size_t rb = png_get_rowbytes(png_ptr, info_ptr); 
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, int_p_NULL, int_p_NULL); 
	HFConvert conv(width, GetSourceFormat(),m_format);
	for (int pass = 0; pass < number_passes; pass++)
	{
		byte * ps = p;
		for (dword y = 0; y < m_height; y++)
		{
			png_read_row(png_ptr, conv.GetPointer(p), png_bytep_NULL);
			conv.Make();
			p += pitch;
		}
	}

	/* read rest of file, and get additional chunks in info_ptr - REQUIRED */
	png_read_end(png_ptr, info_ptr);

	return true;
}

HOEFORMAT TextureConverterPNG::GetSourceFormat()
{
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, int_p_NULL, int_p_NULL); 
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		if (bit_depth != 8)
			return HOE_UNKNOWN;
		return HOE_R8G8B8;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		if (bit_depth != 8)
			return HOE_UNKNOWN;
		return HOE_R8G8B8A8;
		break;
	default:
	case PNG_COLOR_TYPE_PALETTE:
	   return HOE_UNKNOWN;
	};
}

void TextureConverterPNG::Destroy()
{
	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL); 
	delete this;
}

