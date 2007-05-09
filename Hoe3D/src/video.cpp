
#include "StdAfx.h"
#include "ref.h"
#include "video.h"
#include "config.h"
#include "shared.h"


#include <xvid.h>

FILE * in_file;
static void *dec_handle = NULL;
unsigned char *mp4_buffer = NULL;
   unsigned char *mp4_ptr    = NULL;
   unsigned char *out_buffer = NULL;
   int useful_bytes;
xvid_dec_stats_t xvid_dec_stats;
static int XDIM = 576;
static int YDIM = 432;
static int CSP = XVID_CSP_BGRA;

#define BUFFER_SIZE (1024*1024*1)

HoeVideoPlayer::HoeVideoPlayer()
{
	m_tex = NULL;
}

bool HoeVideoPlayer::Load(const char * fname)
{
	in_file = fopen(fname, "rb");
	
	// create texture
	dword w=XDIM,h=YDIM;
	HOEFORMAT f=HOE_R8G8B8;
	m_tex = new HoeVideoTexture();
	GetConfig()->CheckTexture(w,h,f);
	m_tex->Create(w,h,f);

	// xvid init
	int ret;

   xvid_gbl_init_t   xvid_gbl_init;
   xvid_dec_create_t xvid_dec_create;

   /* Reset the structure with zeros */
   memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init_t));
   memset(&xvid_dec_create, 0, sizeof(xvid_dec_create_t));

   /*------------------------------------------------------------------------
    * XviD core initialization
    *----------------------------------------------------------------------*/

   /* Version */
   xvid_gbl_init.version = XVID_VERSION;

   /* Assembly setting */
   //if(use_assembler)
#ifdef ARCH_IS_IA64
      xvid_gbl_init.cpu_flags = XVID_CPU_FORCE | XVID_CPU_IA64;
#else
   xvid_gbl_init.cpu_flags = 0;
#endif
   //else
   //   xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;

   xvid_gbl_init.debug = 0;//debug_level;

   xvid_global(NULL, 0, &xvid_gbl_init, NULL);

   /*------------------------------------------------------------------------
    * XviD encoder initialization
    *----------------------------------------------------------------------*/

   /* Version */
   xvid_dec_create.version = XVID_VERSION;

   /*
    * Image dimensions -- set to 0, xvidcore will resize when ever it is
    * needed
    */
   xvid_dec_create.width = 0;
   xvid_dec_create.height = 0;

   ret = xvid_decore(NULL, XVID_DEC_CREATE, &xvid_dec_create, NULL);

   dec_handle = xvid_dec_create.handle;

   mp4_buffer = (unsigned char *) malloc(BUFFER_SIZE);
   mp4_ptr = mp4_buffer;

   useful_bytes = fread(mp4_buffer, 1, BUFFER_SIZE, in_file);
   //totaldectime = 0;
   //totalsize = 0;
   //filenr = 0;
   //mp4_ptr = mp4_buffer;
   //chunk = 0;

	return true;
}

/* decode one frame  */
static int
dec_main(unsigned char *istream,
       unsigned char *ostream,
       int istream_size,
	   int stride,
       xvid_dec_stats_t *xvid_dec_stats)
{

   int ret;

   xvid_dec_frame_t xvid_dec_frame;

   /* Reset all structures */
   memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));
   memset(xvid_dec_stats, 0, sizeof(xvid_dec_stats_t));

   /* Set version */
   xvid_dec_frame.version = XVID_VERSION;
   xvid_dec_stats->version = XVID_VERSION;

   /* No general flags to set */
   xvid_dec_frame.general          = 0;

   /* Input stream */
   xvid_dec_frame.bitstream        = istream;
   xvid_dec_frame.length           = istream_size;

   /* Output frame structure */
   xvid_dec_frame.output.plane[0]  = ostream;
   xvid_dec_frame.output.stride[0] = stride;
   xvid_dec_frame.output.csp = CSP;

   ret = xvid_decore(dec_handle, XVID_DEC_DECODE, &xvid_dec_frame, xvid_dec_stats);

   return(ret);
}

/* close decoder to release resources */
static int
dec_stop()
{
   int ret;

   ret = xvid_decore(dec_handle, XVID_DEC_DESTROY, NULL, NULL);

   return(ret);
}

void HoeVideoPlayer::NextFrame()
{
	// convert next frame to texture
      int used_bytes = 0;
      double dectime;

		HoeTexture::LOCKRECT lr;
		if (!m_tex->Lock(&lr))
			return;

      /*
       * If the buffer is half empty or there are no more bytes in it
       * then fill it.
       */
      while (mp4_ptr > mp4_buffer + BUFFER_SIZE/2) {
         int already_in_buffer = (mp4_buffer + BUFFER_SIZE - mp4_ptr);

         /* Move data if needed */
         if (already_in_buffer > 0)
            memcpy(mp4_buffer, mp4_ptr, already_in_buffer);

         /* Update mp4_ptr */
         mp4_ptr = mp4_buffer; 

         /* read new data */
            //if(feof(in_file))
            //return;

         useful_bytes += fread(mp4_buffer + already_in_buffer,
                          1, BUFFER_SIZE - already_in_buffer,
                          in_file);
      }


      /* This loop is needed to handle VOL/NVOP reading */
      do {

         /* Decode frame */
         used_bytes = dec_main(mp4_ptr, lr.data, useful_bytes, lr.pitch, &xvid_dec_stats);

         /* Resize image buffer if needed */
         if(xvid_dec_stats.type == XVID_TYPE_VOL) {

            /* Check if old buffer is smaller */
            if(XDIM*YDIM < xvid_dec_stats.data.vol.width*xvid_dec_stats.data.vol.height) {

               /* Copy new witdh and new height from the vol structure */
               XDIM = xvid_dec_stats.data.vol.width;
               YDIM = xvid_dec_stats.data.vol.height;

			}
         }

         /* Update buffer pointers */
         if(used_bytes > 0) {
            mp4_ptr += used_bytes;
            useful_bytes -= used_bytes;

            /* Total size */
            //totalsize += used_bytes;
		 }

         //if (display_buffer_bytes) {
         //   printf("Data chunk %d: %d bytes consumed, %d bytes in buffer\n", chunk++, used_bytes, useful_bytes);
         //}
      } while (xvid_dec_stats.type <= 0 && useful_bytes > 1);

      /* Check if there is a negative number of useful bytes left in buffer
       * This means we went too far */
        if(useful_bytes < 0)
            return;
      
       /* Updated data - Count only usefull decode time */
      //totaldectime += dectime;

         
      /*if (!display_buffer_bytes) {
         printf("Frame %5d: type = %s, dectime(ms) =%6.1f, length(bytes) =%7d\n",
               filenr, type2str(xvid_dec_stats.type), dectime, used_bytes);
      }*/

      /* Save individual mpeg4 stream if required */
      /*if(ARG_SAVEMPEGSTREAM) {
         FILE *filehandle = NULL;

         sprintf(filename, "%sframe%05d.m4v", filepath, filenr);
         filehandle = fopen(filename, "wb");
         if(!filehandle) {
            fprintf(stderr,
                  "Error writing single mpeg4 stream to file %s\n",
                  filename);
         }
         else {
            fwrite(mp4_ptr-used_bytes, 1, used_bytes, filehandle);
            fclose(filehandle);
         }
      }
            
      /* Save output frame if required */
      /*if (ARG_SAVEDECOUTPUT) {
         sprintf(filename, "%sdec%05d", filepath, filenr);
         if(write_image(filename, out_buffer)) {
            fprintf(stderr,
                  "Error writing decoded frame %s\n",
                  filename);
         }
      }*/
		// konvert

      //filenr++;

   //} while (useful_bytes>MIN_USEFUL_BYTES || !feof(in_file));

	// copy to texture
	
	m_tex->Unlock();
	  }


// endfile
