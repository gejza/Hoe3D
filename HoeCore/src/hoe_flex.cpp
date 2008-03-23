
#include "StdAfx.h"
#include "../include/HoeCore/hoe_flex.h"
#include "../include/HoeCore/hoe_platform.h"
#include "../include/HoeCore/hoe_unicode.h"

////////////////////////////////
// scaner class
namespace HoeCore {

// buffers
HoeFlexBuffer::HoeFlexBuffer()
{
	m_line = 1;
}

const String& HoeFlexBuffer::GetIdentifier() const 
{
	return String::Empty;
}

// flex file

HoeFlexFile::HoeFlexFile(HoeCore::ReadStream& stream) : m_in(stream)
{
    this->yy_buf_size = 1000;

    //	/* yy_ch_buf has to be 2 characters longer than the size given because
    //	 * we need to put in 2 end-of-buffer characters.
    //	 */

	this->yy_ch_buf = (tchar *) malloc(
		( this->yy_buf_size + 2 ) * sizeof( tchar ) );

    this->yy_is_our_buffer = 1;

    Flush();
    this->yy_fill_buffer = 1;

}

size_t HoeFlexFile::Input(tchar * buff, size_t max)
{
	// pokud unicode tak nacitat i z utf
#ifdef _UNICODE
	if (!m_in.GetDataFormat().IsUnicode())
	{
		char cb[1024];
		if (max > 1000) max = 1000;
		size_t r = m_in.Read(cb, max);
		size_t c = string::utf8cont(cb, r);
		if (c) r += m_in.Read(cb+r, c);
		return string::utf2w(buff, cb, r);
	}
#endif
	return m_in.Read(buff, max * sizeof(tchar));
}

HoeFlexMem::HoeFlexMem(const tchar* buff)
{
	size_t l = HoeCore::string::len(buff);
	m_buff = new tchar[l+2];
	if (l)
		HoeCore::string::copy(m_buff, buff, l);
    m_buff[l+0] = YY_END_OF_BUFFER_CHAR;
    m_buff[l+1] = YY_END_OF_BUFFER_CHAR;
	this->yy_buf_size = l+2;

    //	/* yy_ch_buf has to be 2 characters longer than the size given because
    //	 * we need to put in 2 end-of-buffer characters.
    //	 */

	this->yy_ch_buf = m_buff;
    this->yy_is_our_buffer = 1;
    this->yy_n_chars = l;
    this->yy_buf_pos = &this->yy_ch_buf[0];
    this->m_bol = true;
    this->yy_buffer_status = YY_BUFFER_NEW;
    this->yy_fill_buffer = 0;

}

////////////////////////////////

int HoeFlex::yy_get_next_buffer()
{
	register tchar *dest = m_buffer->yy_ch_buf;
	register tchar *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &m_buffer->yy_ch_buf[yy_n_chars + 1] )
		yy_fatal_error(
		T("fatal flex scanner internal error--end of buffer missed") );

	if ( m_buffer->yy_fill_buffer == 0 )
		{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
			{
			/* We matched a single character, the EOB, so
			 * treat this as a final EOF.
			 */
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			/* We matched some text prior to the EOB, first
			 * process it.
			 */
			return EOB_ACT_LAST_MATCH;
			}
		}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( m_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		 * just force an EOF
		 */
		m_buffer->yy_n_chars = yy_n_chars = 0;

	else
		{
		size_t num_to_read =
			m_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ /* Not enough room in the buffer - grow it. */
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			YY_BUFFER_STATE b = m_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
				{
				size_t new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (tchar *)
					/* Include room in for 2 EOB chars. */
					realloc( (void *) b->yy_ch_buf,
							( b->yy_buf_size + 2 ) *
							sizeof( tchar ) );
				}
			else
				/* Can't grow it, we don't own it. */
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				yy_fatal_error(
				T("fatal error - scanner input buffer overflow") );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = m_buffer->yy_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > 1000 )
			num_to_read = 1000;

		/* Read in more data. */
		yy_n_chars = m_buffer->Input( (&m_buffer->yy_ch_buf[number_to_move]),
			num_to_read );

		m_buffer->yy_n_chars = yy_n_chars;
		}

	if ( yy_n_chars == 0 )
		{
		m_buffer->yy_fill_buffer = 0;
		if ( number_to_move == YY_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			//yyrestart( stdin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			m_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	m_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	m_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &m_buffer->yy_ch_buf[0];

	return ret_val;
}

void HoeFlex::Switch(HoeFlexBuffer& buff)
{
	if ( m_buffer == &buff )
		return;

	if ( m_buffer )
    {
        /* Flush out information for old buffer. */
        *yy_c_buf_p = yy_hold_char;
        m_buffer->yy_buf_pos = yy_c_buf_p;
        m_buffer->yy_n_chars = yy_n_chars;
    }

	m_buffer = &buff;
	yy_n_chars = m_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = m_buffer->yy_buf_pos;
	//stdin = m_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;

	/* We don't actually know whether we did this switch during
	 * EOF (yywrap()) processing, but the only time this flag
	 * is looked at is after yywrap() is called, so it's safe
	 * to go ahead and always set it.
	 */
	yy_did_buffer_switch_on_eof = 1;
}

void HoeFlex::yy_fatal_error ( const tchar msg[] )
{
	throw msg;
}

int HoeFlex::yyinput()
{
	int c;

	*yy_c_buf_p = yy_hold_char;

#if 0
	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
		{
		/* yy_c_buf_p now points to the character we want to return.
		 * If this occurs *before* the EOB characters, then it's a
		 * valid NUL; if not, then we've hit the end of the buffer.
		 */
		if ( yy_c_buf_p < &m_buffer->yy_ch_buf[yy_n_chars] )
			/* This was really a NUL. */
			*yy_c_buf_p = (YY_CHAR) '\0';

		else
			{ /* need more input */
			int offset = yy_c_buf_p - yytext_ptr;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					/* This happens because yy_g_n_b()
					 * sees that we've accumulated a
					 * token and flags that we need to
					 * try matching the token before
					 * proceeding.  But for input(),
					 * there's no matching to consider.
					 * So convert the EOB_ACT_LAST_MATCH
					 * to EOB_ACT_END_OF_FILE.
					 */

					/* Reset buffer status. */
					//yyrestart( stdin );

					/* fall through */

				case EOB_ACT_END_OF_FILE:
					{
					if ( yywrap() )
						return EOF;

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					return yyinput();
					}

				case EOB_ACT_CONTINUE_SCAN:
					yy_c_buf_p = yytext_ptr + offset;
					break;
				}
			}
		}
#endif
	c = YY_SC_TO_UI(*yy_c_buf_p);
	*yy_c_buf_p = '\0';	/* preserve yytext */
	yy_hold_char = *++yy_c_buf_p;

	m_buffer->SetBol(c == '\n');
	/*if ( m_buffer->IsBol() )
		M4_YY_INCR_LINENO();*/

	return c;
}

void HoeFlex::yyunput( int c, register tchar *yy_bp )
{
	register tchar *yy_cp = yy_c_buf_p;

	/* undo effects of setting up yytext */
	*yy_cp = yy_hold_char;

	if ( yy_cp < m_buffer->yy_ch_buf + 2 )
		{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register size_t number_to_move = yy_n_chars + 2;
		register tchar *dest = &m_buffer->yy_ch_buf[
					m_buffer->yy_buf_size + 2];
		register tchar *source =
				&m_buffer->yy_ch_buf[number_to_move];

		while ( source > m_buffer->yy_ch_buf )
			*--dest = *--source;

		yy_cp += (int) (dest - source);
		yy_bp += (int) (dest - source);
		m_buffer->yy_n_chars =
			yy_n_chars = m_buffer->yy_buf_size;

		if ( yy_cp < m_buffer->yy_ch_buf + 2 )
			this->yy_fatal_error( T("flex scanner push-back overflow") );
		}

	*--yy_cp = (tchar) c;

	// %% update yylineno here

	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
}


//%-
//void yy_delete_buffer( YY_BUFFER_STATE b )
//	{
//	if ( ! b )
//		return;
//
//	if ( b == m_buffer )
//		m_buffer = (YY_BUFFER_STATE) 0;
//
//	if ( b->yy_is_our_buffer )
//		yy_flex_free( (void *) b->yy_ch_buf );
//
//	yy_flex_free( (void *) b );
//	}
//
//




//YY_BUFFER_STATE yy_scan_buffer( YY_CHAR *base, yy_size_t size )
//	{
//	YY_BUFFER_STATE b;
//
//	if ( size < 2 ||
//	     base[size-2] != YY_END_OF_BUFFER_CHAR ||
//	     base[size-1] != YY_END_OF_BUFFER_CHAR )
//		/* They forgot to leave room for the EOB's. */
//		return 0;
//
//	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
//	if ( ! b )
//		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_buffer()" );
//
//	b->yy_buf_size = size - 2;	/* "- 2" to take care of EOB's */
//	b->yy_buf_pos = b->yy_ch_buf = base;
//	b->yy_is_our_buffer = 0;
//	b->yy_input_file = 0;
//	b->yy_n_chars = b->yy_buf_size;
//	b->yy_at_bol = 1;
//	b->yy_fill_buffer = 0;
//	b->yy_buffer_status = YY_BUFFER_NEW;
//
//	yy_switch_to_buffer( b );
//
//	return b;
//	}


//#ifndef YY_NO_SCAN_STRING
////%-
//YY_BUFFER_STATE yy_scan_string( const YY_CHAR *yy_str )
//	{
//	int len;
//	for ( len = 0; yy_str[len]; ++len )
//		;
//
//	return yy_scan_chars( yy_str, len );
//	}
////%*
//#endif


//#ifndef YY_NO_SCAN_BYTES
////%-
//YY_BUFFER_STATE yy_scan_chars( const YY_CHAR *chars, int len )
//	{
//	YY_BUFFER_STATE b;
//	YY_CHAR *buf;
//	yy_size_t n;
//	int i;
//
//	/* Get memory for full buffer, including space for trailing EOB's. */
//	n = len + 2;
//	buf = (YY_CHAR *) yy_flex_alloc( n * sizeof( YY_CHAR ) );
//	if ( ! buf )
//		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_chars()" );
//
//	for ( i = 0; i < len; ++i )
//		buf[i] = chars[i];
//
//	buf[len] = buf[len+1] = YY_END_OF_BUFFER_CHAR;
//
//	b = yy_scan_buffer( buf, n );
//	if ( ! b )
//		YY_FATAL_ERROR( "bad buffer in yy_scan_chars()" );
//
//	/* It's okay to grow etc. this buffer, and we should throw it
//	 * away when we're done.
//	 */
//	b->yy_is_our_buffer = 1;
//
//	return b;
//	}
////%*
//#endif


//#ifndef YY_NO_PUSH_STATE
////%-
//static void yy_push_state( int new_state )
////%+
//void yyFlexLexer::yy_push_state( int new_state )
////%*
//	{
//	if ( yy_start_stack_ptr >= yy_start_stack_depth )
//		{
//		yy_size_t new_size;
//
//		yy_start_stack_depth += YY_START_STACK_INCR;
//		new_size = yy_start_stack_depth * sizeof( int );
//
//		if ( ! yy_start_stack )
//			yy_start_stack = (int *) yy_flex_alloc( new_size );
//
//		else
//			yy_start_stack = (int *) yy_flex_realloc(
//					(void *) yy_start_stack, new_size );
//
//		if ( ! yy_start_stack )
//			YY_FATAL_ERROR(
//			"out of memory expanding start-condition stack" );
//		}
//
//	yy_start_stack[yy_start_stack_ptr++] = YY_START;
//
//	BEGIN(new_state);
//	}
//#endif


//#ifndef YY_NO_POP_STATE
//static void yy_pop_state()
//	{
//	if ( --yy_start_stack_ptr < 0 )
//		YY_FATAL_ERROR( "start-condition stack underflow" );
//
//	BEGIN(yy_start_stack[yy_start_stack_ptr]);
//	}
//#endif
//
//
//#ifndef YY_NO_TOP_STATE
////%-
//static int yy_top_state()
//	{
//	return yy_start_stack[yy_start_stack_ptr - 1];
//	}
//#endif


/* Internal utility routines. */

//#ifndef yytext_ptr
//static void yy_flex_strncpy( YY_CHAR *s1, const YY_CHAR *s2, int n )
//	{
//	register int i;
//	for ( i = 0; i < n; ++i )
//		s1[i] = s2[i];
//	}
//#endif
//
//#ifdef YY_NEED_STRLEN
//static int yy_flex_strlen( const YY_CHAR *s )
//	{
//	register int n;
//	for ( n = 0; s[n]; ++n )
//		;
//
//	return n;
//	}
//#endif

} // namespace HoeCore
