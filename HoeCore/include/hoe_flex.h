
// FlexLexer.h -- define interfaces for lexical analyzer classes generated
//		  by flex

// predpis pro generovani scaneru
// obsahuje i zakladni tridy pro prepsani

//
//	#undef yyFlexLexer
//	#define yyFlexLexer xxFlexLexer
//	#include <FlexLexer.h>
//
//	#undef yyFlexLexer
//	#define yyFlexLexer zzFlexLexer
//	#include <FlexLexer.h>
//	...

#ifndef _HOE_FLEX_H_
#define _HOE_FLEX_H_


/* Define the YY_CHAR type. */
typedef char YY_CHAR;
/* Promotes a YY_CHAR to an unsigned integer for use as an array index. */
#define YY_SC_TO_UI(c) ((unsigned int) (unsigned short) c)
#define YY_END_OF_BUFFER_CHAR 0

typedef int yy_state_type;

/* Translate the current start state into a value that can be later handed
 * to BEGIN to return to the state.  The YYSTATE alias is for lex
 * compatibility.
 */
#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START

/* Action number for EOF rule of a given start state. */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define YY_NEW_FILE /*yyrestart( stdin )*/

extern int yyleng;
#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2


typedef int flex_int32_t;
typedef short flex_int16_t;

typedef class HoeFlexBuffer * YY_BUFFER_STATE;

#define YY_G(a) (a)


#define yyconst const


/* Returned upon end-of-file. */
#define YY_NULL 0
#define INITIAL 0
/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN yy_start = 1 + 2 *



namespace HoeCore {

class HoeFlexBuffer
{
	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	bool m_bol;
public:
    bool IsBol() { return m_bol; }
    void SetBol(bool bol) { m_bol = bol; }

	FILE *yy_input_file;

	YY_CHAR *yy_ch_buf;		/* input buffer */
	YY_CHAR *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	size_t yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	 * and can realloc() it to grow it, and should free() it to
	 * delete it.
	 */
	int yy_is_our_buffer;


	/* Whether to try to fill the input buffer when we reach the
	 * end of it.
	 */
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	 * then we mark the buffer as YY_EOF_PENDING, to indicate that we
	 * shouldn't try reading from the input source any more.  We might
	 * still have a bunch of tokens to match, though, because of
	 * possible backing-up.
	 *
	 * When we actually see the EOF, we change the status to "new"
	 * (via yyrestart()), so that the user can continue scanning by
	 * just pointing yyin at a new input file.
	 */
#define YY_BUFFER_EOF_PENDING 2

    void Flush()
    {

        this->yy_n_chars = 0;

        /* We always need two end-of-buffer characters.  The first causes
         * a transition to the end-of-buffer state.  The second causes
         * a jam in that state.
         */
        this->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
        this->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

        this->yy_buf_pos = &this->yy_ch_buf[0];

        this->m_bol = true;
        this->yy_buffer_status = YY_BUFFER_NEW;

        /*if ( b == m_buffer )
            yy_load_buffer_state();
        }*/
    }
};

class HoeFlexFile : public HoeFlexBuffer
{
public:
    HoeFlexFile(FILE * f);
};

class HoeFlex
{
protected:
	HoeFlexBuffer * m_buffer;
	YY_CHAR yy_hold_char; /* yy_hold_char holds the character lost when yytext is formed. */
	/* Points to current character in buffer. */
	YY_CHAR *yy_c_buf_p;
	int yy_init;		/* whether we need to initialize */
	int yy_start;	/* start state number */
	/* Flag which is used to allow yywrap()'s to do buffer switches
	 * instead of setting up a fresh yyin.  A bit of a hack ...
	 */
	int yy_did_buffer_switch_on_eof;
	int yy_n_chars;		/* number of characters read into yy_ch_buf */
	YY_CHAR * yytext_ptr;
public:
	HoeFlex()
	{
		m_buffer = NULL;
		yy_c_buf_p = (YY_CHAR *) 0;
		yy_init = 1;		/* whether we need to initialize */
		yy_start = 0;	/* start state number */
	}
	
	virtual int Echo(const char * buff, size_t size)
	{
		return 0;
	}
	void Switch(HoeFlexBuffer& buff);

	#define YY_FLUSH_BUFFER yy_flush_buffer( m_buffer )
	int yy_get_next_buffer ( void );
	void yyunput ( int c, YY_CHAR *buf_ptr );
	int yyinput ( void );
	void yy_fatal_error ( const char msg[] );
};

#define DECLARE_FLEX_FUNCTIONS(param) int Lex(param); \
	 private: \
	 yy_state_type yy_get_previous_state(); \
	 yy_state_type yy_try_NUL_trans ( yy_state_type current_state );


} // namespace HoeCore

#endif // _HOE_FLEX_H_



