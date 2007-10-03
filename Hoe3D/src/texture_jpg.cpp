
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "hoe_format.h"
#include "config.h"
#include "../include/hfmt/htex_file.h"
#include "texture_loader.h"
#include "texture_converter.h"

extern "C" {
#define XMD_H // provizorne
#include <jpeglib.h>
}

#if !defined(_DFMT_D3D9_) && !defined(_DFMT_OPENGL_)
#error Data format not defined.
#endif

#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */
static unsigned char buffer[INPUT_BUF_SIZE];		/* start of buffer */

struct jpeg_source : public jpeg_source_mgr
{
	TextureLoader * loader;
};

struct jpeg_error_mgr jerr;

void jpeg_set_src(j_decompress_ptr cinfo, TextureLoader *);

TextureConverterJPG::TextureConverterJPG(TextureLoader * l,HoeLog * log) : TextureConverter(l,log)
{
	jpeg_decompress_struct *cinfo = new jpeg_decompress_struct; 
	memset(cinfo,0,sizeof(jpeg_decompress_struct));
	cinfo->err = jpeg_std_error(&jerr);
	
	jpeg_create_decompress(cinfo);
	jpginfo = cinfo;

	udata = NULL;
}

TextureConverterJPG::~TextureConverterJPG()
{
	jpeg_decompress_struct *cinfo = (jpeg_decompress_struct*) jpginfo; 
	jpeg_source * src = (jpeg_source *) cinfo->src;
	if (src)
		delete src;
	delete cinfo;
	if (udata)
		delete [] udata;
}

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
	dword nbytes;

	jpeg_source * src = (jpeg_source *) cinfo->src;
	
	nbytes = INPUT_BUF_SIZE;
	// read data
	if (! src->loader->GetData(buffer,&nbytes)) 
	{

		//WARNMS(cinfo, JWRN_JPEG_EOF);
		buffer[0] = (JOCTET) 0xFF;
		buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	cinfo->src->next_input_byte = buffer;
	cinfo->src->bytes_in_buffer = nbytes;

	return 1;
}

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) cinfo->src->bytes_in_buffer) {
      num_bytes -= (long) cinfo->src->bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    cinfo->src->next_input_byte += (size_t) num_bytes;
    cinfo->src->bytes_in_buffer -= (size_t) num_bytes;
  }
}

void jpeg_set_src (j_decompress_ptr cinfo, TextureLoader * l)
{

	/* The source object and input buffer are made permanent so that a series
	* of JPEG images can be read from the same file by calling jpeg_stdio_src
	* only before the first one.  (If we discarded the buffer at the end of
	* one image, we'd likely lose the start of the next one.)
	* This makes it unsafe to use this manager and a different source
	* manager serially with the same JPEG object.  Caveat programmer.
	*/
	if (cinfo->src == NULL) {	/* first time for this JPEG object? */
		jpeg_source * src = new jpeg_source;
		src->loader = l;
		cinfo->src = src;
	}

	cinfo->src->init_source = init_source;
	cinfo->src->fill_input_buffer = fill_input_buffer;
	cinfo->src->skip_input_data = skip_input_data;
	cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
	cinfo->src->term_source = term_source;
	cinfo->src->bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	cinfo->src->next_input_byte = NULL; /* until buffer loaded */
}





