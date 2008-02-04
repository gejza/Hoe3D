
#include "StdAfx.h"
#include <png.h>
#include "hoe_png.h"


namespace HoeRes {

static void Err(png_structp, png_const_charp msg)
{
}

static void Warn(png_structp, png_const_charp msg)
{
}

static void ReadPng(png_structp p, png_bytep buff, png_size_t s)
{
	reinterpret_cast<HoeCore::ReadStream*>(p->io_ptr)->Read(buff, s);
}

PNGDecoder::PNGDecoder(HoeCore::ReadStream& stream) 
	: PicCodec(stream) 
{
	/*!!!*/ // navratove hodnoty u obrazku
	unsigned char buf[4];
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 

	stream.Read(buf,4);
	//LOG(m_log)->Log("Load PNG with libpng"PNG_LIBPNG_VER_STRING);	stream.Read(buf, 4);
	if (png_sig_cmp(buf, (png_size_t)0, 4))
	{
		//LOG(m_log)->Log("error check header");
		return;
	}

	//png_debug(3, "Msg: ");

	m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,Err,Warn);

	if (m_png == NULL)
	{
		//LOG(m_log)->Log("png_create_read_struct return NULL");
		return;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	m_info = png_create_info_struct(m_png);
	if (m_info == NULL)
	{
		png_destroy_read_struct(&m_png, png_infopp_NULL, png_infopp_NULL);
		return;
	} 

	png_set_read_fn(m_png, (void *)&stream, ReadPng); 

	png_set_sig_bytes(m_png, 4);

	png_read_info(m_png, m_info);

	png_get_IHDR(m_png, m_info, &width, &height, &bit_depth, &color_type,
		&interlace_type, int_p_NULL, int_p_NULL); 

	m_width = width;
	m_height = height;
	m_pitch = png_get_rowbytes(m_png, m_info); 

	m_format = GetSourceFormat();
	m_readline = 0;

	// palette
}

PNGDecoder::~PNGDecoder()
{
	png_destroy_read_struct(&m_png, &m_info, png_infopp_NULL); 
}

HOEFORMAT PNGDecoder::GetSourceFormat()
{
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 
	png_get_IHDR(m_png, m_info, &width, &height, &bit_depth, &color_type,
		&interlace_type, int_p_NULL, int_p_NULL); 
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		if (bit_depth != 8)
			return HOE_UNKNOWN;
		return HOE_R8G8B8;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		if (bit_depth != 8)
			return HOE_UNKNOWN;
		return HOE_R8G8B8A8;
	case PNG_COLOR_TYPE_GRAY:
	   return HOE_UNKNOWN;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
	   return HOE_UNKNOWN;
	case PNG_COLOR_TYPE_PALETTE:
		switch(bit_depth)
		{
		case 1:
			return HOE_P1;
		case 2:
			return HOE_P2;
		case 4:
			return HOE_P4;
		case 8:
			return HOE_P8;
		default:	
			return HOE_UNKNOWN;
		};
	default:
	   return HOE_UNKNOWN;
	};
/*
PNG_COLOR_TYPE_GRAY
                        (bit depths 1, 2, 4, 8, 16)
                     PNG_COLOR_TYPE_GRAY_ALPHA
                        (bit depths 8, 16)
                     PNG_COLOR_TYPE_RGB
                        (bit_depths 8, 16)
                     PNG_COLOR_TYPE_RGB_ALPHA
                        (bit_depths 8, 16)
*/
}

void PNGDecoder::GetSize(THoeSizeu* size)
{
	hoe_assert(size);
	size->width = m_width;
	size->height = m_height;
}

uint PNGDecoder::GetRow(byte* ptr)
{
	/*png_uint_32 width, height;
	int bit_depth, color_type, interlace_type; 
	int number_passes = png_set_interlace_handling(png_ptr);
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
	}*/
	if (m_readline >= m_height) return 0;

	png_read_row(m_png, ptr, png_bytep_NULL);
	m_readline++;
	/* read rest of file, and get additional chunks in info_ptr - REQUIRED */
	if (m_readline >= m_height) 
		png_read_end(m_png, m_info);

	return m_width;
}

uint PNGDecoder::GetPalette(HOECOLOR * palette)
{
	png_colorp palet;
	int num_palet,num_alphas;
	png_bytep alphas;
	if (!png_get_PLTE(m_png, m_info, &palet, &num_palet))
		return 0;
	if (!png_get_tRNS(m_png, m_info, &alphas, &num_alphas, 0))
		num_alphas = 0;
	for (int i=0;i < num_palet;i++)
	{
		palette[i].r = palet[i].red;
		palette[i].g = palet[i].green;
		palette[i].b = palet[i].blue;
		palette[i].a = i < num_alphas ? alphas[i]:0xff;
	}
	return (uint)num_palet;
}

} // namespace HoeRes


