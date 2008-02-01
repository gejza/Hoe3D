
#include "StdAfx.h"
#include "hoe_jpg.h"

extern "C" {
#define XMD_H // provizorne
#include <jpeglib.h>
}

namespace HoeRes {

static struct jpeg_error_mgr jerr; // error manager
const int buff_size = 1024;

struct jpeg_source : public jpeg_source_mgr
{
	HoeCore::ReadStream* stream;
	byte buffer[buff_size];
};

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
	jpeg_source * src = (jpeg_source *) cinfo->src;
}

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{
  /* no work necessary here */
}

METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
	jpeg_source * src = (jpeg_source *) cinfo->src;
		
	// read data
	size_t read = src->stream->Read(src->buffer, sizeof(src->buffer));
	if (read < (sizeof(src->buffer)-2))
	{
		//WARNMS(cinfo, JWRN_JPEG_EOF);
		src->buffer[read++] = (JOCTET) 0xFF;
		src->buffer[read++] = (JOCTET) JPEG_EOI;
	}

	cinfo->src->next_input_byte = src->buffer;
	cinfo->src->bytes_in_buffer = read;
	return 1;
}

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	/* Just a dumb implementation for now.  Could use fseek() except
	* it doesn't work on pipes.  Not clear that being smart is worth
	* any trouble anyway --- large skips are infrequent.
	*/
	jpeg_source * src = (jpeg_source *) cinfo->src;
	// skip loaded data
	size_t skipfb = num_bytes;
	if (skipfb > src->bytes_in_buffer)
		skipfb = src->bytes_in_buffer;
	if (skipfb)
	{
		src->next_input_byte += skipfb;
		src->bytes_in_buffer -= skipfb;
		num_bytes -= skipfb;
	}

	if (num_bytes > 0) 
	{
		/*if (src->stream->CanSeek())
		{
			src->stream->
		}*/
		while (num_bytes > src->bytes_in_buffer) {
		  num_bytes -= src->bytes_in_buffer;
		  (void) fill_input_buffer(cinfo);
		}
		src->next_input_byte += (size_t) num_bytes;
		src->bytes_in_buffer -= (size_t) num_bytes;
	}
}

JPEGDecoder::JPEGDecoder(HoeCore::ReadStream& stream) 
	: PicCodec(stream) 
{
	jpeg_decompress_struct *cinfo = new jpeg_decompress_struct; 
	memset(cinfo,0,sizeof(jpeg_decompress_struct));
	cinfo->err = jpeg_std_error(&jerr);
	
	jpeg_create_decompress(cinfo);
	jpginfo = cinfo;

	/* The source object and input buffer are made permanent so that a series
	* of JPEG images can be read from the same file by calling jpeg_stdio_src
	* only before the first one.  (If we discarded the buffer at the end of
	* one image, we'd likely lose the start of the next one.)
	* This makes it unsafe to use this manager and a different source
	* manager serially with the same JPEG object.  Caveat programmer.
	*/
	jpeg_source * src = new jpeg_source;
	src->stream = &stream;
	cinfo->src = src;

	cinfo->src->init_source = init_source;
	cinfo->src->fill_input_buffer = fill_input_buffer;
	cinfo->src->skip_input_data = skip_input_data;
	cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	cinfo->src->term_source = term_source;
	cinfo->src->bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	cinfo->src->next_input_byte = NULL; /* until buffer loaded */
	// end set src

	jpeg_read_header(cinfo, TRUE);
	jpeg_start_decompress(cinfo);

	m_pitch = cinfo->output_width * cinfo->output_components;
	m_width = cinfo->output_width;
	m_height = cinfo->output_height;
	hoe_assert(cinfo->output_components == 3);
	m_format = HOE_R8G8B8;
}

JPEGDecoder::~JPEGDecoder()
{
	jpeg_decompress_struct *cinfo = (jpeg_decompress_struct*) jpginfo; 
	jpeg_source * src = (jpeg_source *) cinfo->src;
	if (src)
		delete src;
	delete cinfo;
}

void JPEGDecoder::GetSize(THoeSizeu* size)
{
	hoe_assert(size);
	size->width = m_width;
	size->height = m_height;
}

uint JPEGDecoder::GetRow(byte* ptr)
{
	jpeg_decompress_struct *cinfo = (jpeg_decompress_struct*) jpginfo; 
	if (cinfo->output_scanline >= m_height)
		return 0;
	
	jpeg_read_scanlines(cinfo, &ptr, 1);

	if (cinfo->output_scanline >= m_height)
	{
		jpeg_finish_decompress(cinfo);
		jpeg_destroy_decompress(cinfo);
	}

	return m_width;
}

} // namespace HoeRes

#if 0

bool TextureConverterJPG::Run()
{
	jpeg_decompress_struct * cinfo = (jpeg_decompress_struct *)jpginfo;

	jpeg_set_src(cinfo,m_loader);

	jpeg_read_header(cinfo, TRUE);
	jpeg_start_decompress(cinfo);
	row_stride = cinfo->output_width * cinfo->output_components;

	m_width = cinfo->output_width;
	m_height = cinfo->output_height;
	m_format = HOE_R8G8B8;

	GetConfig()->CheckTexture(m_width,m_height,m_format);
	return true;
}

bool TextureConverterJPG::Get(byte * p,dword pitch)
{
	jpeg_decompress_struct * cinfo = (jpeg_decompress_struct *)jpginfo;

	HOEFORMAT from = HOE_R8G8B8;
	switch (cinfo->output_components)
	{
	case 1:
		from = HOE_L8;
		break;
	case 3:
		from = HOE_R8G8B8;
		break;
	default:
		Con_Print("Error: JPEG format is not in 1 or 3 color components.");
		return false;
	};
	HFConvert conv(m_width, from, m_format);
	while (cinfo->output_scanline < m_height) {
		byte * pd = conv.GetPointer(p + (pitch * cinfo->output_scanline));
		jpeg_read_scanlines(cinfo, &pd, 1);
		conv.Make();
	}

	jpeg_finish_decompress(cinfo);
	jpeg_destroy_decompress(cinfo);
	return true;
}

void TextureConverterJPG::Destroy()
{
	delete this;
}

//////////////////////////////////////////



#endif



