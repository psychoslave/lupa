/*
** $Id: llex.c,v 2.96 2016/05/02 14:02:12 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#define llex_c
#define LUA_CORE

#include "lprefix.h"


#include <locale.h>
#include <string.h>

#include "lua.h"

#include "utf8proc/utf8proc.h"
#include "lctype.h"
#include "ldebug.h"
#include "ldo.h"
#include "lgc.h"
#include "llex.h"
#include "lobject.h"
#include "lparser.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lzio.h"



#define next(ls) (ls->current = zgetc(ls->z))



#define currIsNewline(ls)    (ls->current == '\n' || ls->current == '\r')


/* ORDER RESERVED */

static const char *const luaX_tokens [] = {
  "and", "break",       "do",   "else", "elseif",
  "end",  "false", "for", "function", "goto",  "if",
  "in", "local",  "nil",  "not", "or", "repeat",
  "return",  "then", "true", "until", "while",
  "//", "..",   "...", "==",     ">=",         "<=", "~=",
  "<nombra>", "<indukta>", "<noma>", "<ĉena>"
};

static const struct {
  const char *name;
  int token;
} aliases [] = {
  { "kaj", TK_AND },            // and
  { "eksterŝalte", TK_BREAK }, // break
  { "ekstersxalte", TK_BREAK }, // break
  { "rompe", TK_BREAK },        // break
  { "fare", TK_DO },            // do
  { "alie", TK_ELSE },          // else
  { "alise", TK_ELSEIF },       // elseif
  { "hop", TK_END },            // end
  { "falsa", TK_FALSE },        // false
  { "por", TK_FOR },            // for
  { "funkcie", TK_FUNCTION },   // function
  { "tie", TK_FUNCTION },       // function
  { "verbe", TK_FUNCTION },     // function
  { "age", TK_FUNCTION },       // function
  { "ŝalte", TK_GOTO },        // goto
  { "sxalte", TK_GOTO },        // goto
  { "se", TK_IF },              // if
  { "el", TK_IN },              // in
  { "loka", TK_LOCAL },         // local
  { "loke", TK_LOCAL },         // local
  { "ja", TK_LOCAL },           // local
  { "nenio", TK_NIL },          // nil
  { "neo", TK_NIL },            // nil
  { "ne", TK_NOT },             // not
  { "aŭ", TK_OR },              // or
  { "aux", TK_OR },             // or
  { "cikle", TK_REPEAT },       // repeat
  { "reŝalte", TK_RETURN },     // return
  { "resxalte", TK_RETURN },    // return
  { "ren", TK_RETURN },         // return
  { "tiam", TK_THEN },          // then
  { "vera", TK_TRUE },          // true
  { "ĝis", TK_UNTIL },         // until
  { "gxis", TK_UNTIL },         // until
  { "dum", TK_WHILE },          // while
  { "onige",  TK_IDIV },        // //
  { "lige", TK_CONCAT },         // ..
  { "ktp", TK_DOTS },           // ...
  { "egalas",   TK_EQ },        // ==
  { "almenaŭas", TK_GE },       // >=
  { "almenauxas", TK_GE },      // >=
  { "maksimumas", TK_LE },      // <=
  { "malegalas", TK_NE },       // ~=
  // end of luaX_tokens aliases

  // pli da sinonimoj
  { "nee",     TK_BNOT },
  { "disaŭe", TK_BXOR },
  { "disauxe", TK_BXOR },
  { "sup", TK_GT },
  { "super", TK_GT },
  { "superas", TK_GT },
  { "malinfraas", TK_GT },
  { "sur", TK_GE },
  { "suras", TK_GE },
  { "malsubas", TK_GE },
  { "baŭ",   TK_EQ },
  { "baux",   TK_EQ },
  { "samas",   TK_EQ },
  { "zaŭ",     TK_NE },
  { "zaux",    TK_NE },
  { "malsamas",TK_NE },
  { "neegalas",TK_NE },
  { "nesamas",TK_NE },
  { "en", TK_LT },
  { "suf", TK_LT },
  { "infraas", TK_LT },
  { "malsuperas", TK_LT },
  { "sub", TK_LE },
  { "subas", TK_LE },
  { "malsuras", TK_LE },
  { "malalmenaŭas", TK_LE },
  { "malalmenauxas", TK_LE },
  { "kaje", TK_BAND },
  { "aŭe", TK_BOR },
  { "auxe", TK_BOR },
  { "sobŝove", TK_SHR },
  { "sobsxove", TK_SHR },
  { "sob", TK_SHR },
  { "sorŝove", TK_SHL },
  { "sorsxove", TK_SHL },
  { "sor", TK_SHL },
  { "plus", TK_ADD },
  { "oble", TK_MUL },
  { "multiplike", TK_MUL },
  { "mal", TK_MINUS },
  { "kontraŭ", TK_MINUS },
  { "kontraux", TK_MINUS },
  { "minus", TK_SUB },
  { "disige", TK_DIV },
  { "divide", TK_DIV },
  { "ozle", TK_DIV },
  { "parte", TK_IDIV },
  { "pece", TK_IDIV },
  { "kvociente", TK_IDIV },
  { "laŭ", TK_MOD },
  { "laux", TK_MOD },
  { "module", TK_MOD },
  { "kongrue", TK_MOD },
  { "alt", TK_POW },
  { "potencige", TK_POW },
  { "kroĉe",TK_CONCAT }, // ..
  { "krocxe",TK_CONCAT }, //
  { "sin", TK_COLON }, // :
  { "ke", TK_COLON }, // :
  { "ho", TK_DBCOLON }, // ::
};

#define save_and_next(ls) (save(ls, ls->current), next(ls))


static void init_aliases (lua_State *L) {
  int i;
  int n = sizeof(aliases)/sizeof(aliases[0]);
  for (i=0; i<n; i++) {
    TString *ts = luaS_new(L, aliases[i].name);
    luaC_fix(L, obj2gco(ts));
    ts->extra = cast_byte(aliases[i].token);
  }
}

static l_noret lexerror (LexState *ls, const char *msg, int token);


static void save (LexState *ls, int c) {
  Mbuffer *b = ls->buff;
  if (luaZ_bufflen(b) + 1 > luaZ_sizebuffer(b)) {
    size_t newsize;
    if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
      lexerror(ls, "lexical element too long", 0);
    newsize = luaZ_sizebuffer(b) * 2;
    luaZ_resizebuffer(ls->L, b, newsize);
  }
  b->buffer[luaZ_bufflen(b)++] = cast(char, c);
}


void luaX_init (lua_State *L) {
  int i;
  TString *e = luaS_newliteral(L, LUA_ENV);  /* create env name */
  luaC_fix(L, obj2gco(e));  /* never collect this name */
  for (i=0; i<NUM_RESERVED; i++) {
    TString *ts = luaS_new(L, luaX_tokens[i]);
    luaC_fix(L, obj2gco(ts));  /* reserved words are never collected */
    ts->extra = cast_byte(i+1);  /* reserved word */
  }

    TString *ts = luaS_new(L, "egaligxas");
    luaC_fix(L, obj2gco(ts));  /* reserved words are never collected */
    ts->extra = cast_byte(TK_EQ+1-FIRST_RESERVED);  /* reserved word */
    init_aliases(L);
}


const char *luaX_token2str (LexState *ls, int token) {
  if (token < FIRST_RESERVED) {  /* single-byte symbols? */
    lua_assert(token == cast_uchar(token));
    return luaO_pushfstring(ls->L, "'%c'", token);
  }
  else {
    const char *s = luaX_tokens[token - FIRST_RESERVED];
    if (token < TK_EOS)  /* fixed format (symbols and reserved words)? */
      return luaO_pushfstring(ls->L, "'%s'", s);
    else  /* names, strings, and numerals */
      return s;
  }
}


static const char *txtToken (LexState *ls, int token) {
  switch (token) {
    case TK_NAME: case TK_STRING:
    case TK_FLT: case TK_INT:
      save(ls, '\0');
      return luaO_pushfstring(ls->L, "'%s'", luaZ_buffer(ls->buff));
    default:
      return luaX_token2str(ls, token);
  }
}


static l_noret lexerror (LexState *ls, const char *msg, int token) {
  msg = luaG_addinfo(ls->L, msg, ls->source, ls->linenumber);
  if (token)
    luaO_pushfstring(ls->L, "%s near %s", msg, txtToken(ls, token));
  luaD_throw(ls->L, LUA_ERRSYNTAX);
}


l_noret luaX_syntaxerror (LexState *ls, const char *msg) {
  lexerror(ls, msg, ls->t.token);
}


/*
** creates a new string and anchors it in scanner's table so that
** it will not be collected until the end of the compilation
** (by that time it should be anchored somewhere)
*/
TString *luaX_newstring (LexState *ls, const char *str, size_t l) {
  lua_State *L = ls->L;
  TValue *o;  /* entry for 'str' */
  TString *ts = luaS_newlstr(L, str, l);  /* create new string */
  setsvalue2s(L, L->top++, ts);  /* temporarily anchor it in stack */
  o = luaH_set(L, ls->h, L->top - 1);
  if (ttisnil(o)) {  /* not in use yet? */
    /* boolean value does not need GC barrier;
       table has no metatable, so it does not need to invalidate cache */
    setbvalue(o, 1);  /* t[string] = true */
    luaC_checkGC(L);
  }
  else {  /* string already present */
    ts = tsvalue(keyfromval(o));  /* re-use value previously stored */
  }
  L->top--;  /* remove string from stack */
  return ts;
}


/*
** increment line number and skips newline sequence (any of
** \n, \r, \n\r, or \r\n)
*/
static void inclinenumber (LexState *ls) {
  int old = ls->current;
  lua_assert(currIsNewline(ls));
  next(ls);  /* skip '\n' or '\r' */
  if (currIsNewline(ls) && ls->current != old)
    next(ls);  /* skip '\n\r' or '\r\n' */
  if (++ls->linenumber >= MAX_INT)
    lexerror(ls, "chunk has too many lines", 0);
}


void luaX_setinput (lua_State *L, LexState *ls, ZIO *z, TString *source,
                    int firstchar) {
  ls->t.token = 0;
  ls->L = L;
  ls->current = firstchar;
  ls->lookahead.token = TK_EOS;  /* no look-ahead token */
  ls->z = z;
  ls->fs = NULL;
  ls->linenumber = 1;
  ls->lastline = 1;
  ls->source = source;
  ls->envn = luaS_newliteral(L, LUA_ENV);  /* get env name */
  ls->pending_alias_ts = NULL;
  ls->pending_alias_pos = 0;
  ls->pending_extra_token = 0;
  ls->bracket_alias_depth = 0;
  luaZ_resizebuffer(ls->L, ls->buff, LUA_MINBUFFER);  /* initialize buffer */
}



/*
** =======================================================
** LEXICAL ANALYZER
** =======================================================
*/


static int check_next1 (LexState *ls, int c) {
  if (ls->current == c) {
    next(ls);
    return 1;
  }
  else return 0;
}


/*
** Check whether current char is in set 'set' (with two chars) and
** saves it
*/
static int check_next2 (LexState *ls, const char *set) {
  lua_assert(set[2] == '\0');
  if (ls->current == set[0] || ls->current == set[1]) {
    save_and_next(ls);
    return 1;
  }
  else return 0;
}


/* LUA_NUMBER */
/*
** this function is quite liberal in what it accepts, as 'luaO_str2num'
** will reject ill-formed numerals.
*/
static int read_numeral (LexState *ls, SemInfo *seminfo) {
  TValue obj;
  const char *expo = "Ee";
  int first = ls->current;
  lua_assert(lisdigit(ls->current));
  save_and_next(ls);
  if (first == '0' && check_next2(ls, "xX"))  /* hexadecimal? */
    expo = "Pp";
  for (;;) {
    if (check_next2(ls, expo))  /* exponent part? */
      check_next2(ls, "-+");  /* optional exponent sign */
    if (lisxdigit(ls->current))
      save_and_next(ls);
    else if (ls->current == '.')
      save_and_next(ls);
    else break;
  }
  save(ls, '\0');
  if (luaO_str2num(luaZ_buffer(ls->buff), &obj) == 0)  /* format error? */
    lexerror(ls, "malformed number", TK_FLT);
  if (ttisinteger(&obj)) {
    seminfo->i = ivalue(&obj);
    return TK_INT;
  }
  else {
    lua_assert(ttisfloat(&obj));
    seminfo->r = fltvalue(&obj);
    return TK_FLT;
  }
}


/*
** skip a sequence '[=*[' or ']=*]'; if sequence is well formed, return
** its number of '='s; otherwise, return a negative number (-1 iff there
** are no '='s after initial bracket)
*/
static int skip_sep (LexState *ls) {
  int count = 0;
  int s = ls->current;
  lua_assert(s == '[' || s == ']');
  save_and_next(ls);
  while (ls->current == '=') {
    save_and_next(ls);
    count++;
  }
  return (ls->current == s) ? count : (-count) - 1;
}


static void read_long_string (LexState *ls, SemInfo *seminfo, int sep) {
  int line = ls->linenumber;  /* initial line (for error message) */
  save_and_next(ls);  /* skip 2nd '[' */
  if (currIsNewline(ls))  /* string starts with a newline? */
    inclinenumber(ls);  /* skip it */
  for (;;) {
    switch (ls->current) {
      case EOZ: {  /* error */
        const char *what = (seminfo ? "string" : "comment");
        const char *msg = luaO_pushfstring(ls->L,
                     "unfinished long %s (starting at line %d)", what, line);
        lexerror(ls, msg, TK_EOS);
        break;  /* to avoid warnings */
      }
      case ']': {
        if (skip_sep(ls) == sep) {
          save_and_next(ls);  /* skip 2nd ']' */
          goto endloop;
        }
        break;
      }
      case '\n': case '\r': {
        save(ls, '\n');
        inclinenumber(ls);
        if (!seminfo) luaZ_resetbuffer(ls->buff);  /* avoid wasting space */
        break;
      }
      default: {
        if (seminfo) save_and_next(ls);
        else next(ls);
      }
    }
  } endloop:
  if (seminfo)
    seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + (2 + sep),
                                     luaZ_bufflen(ls->buff) - 2*(2 + sep));
}


static void esccheck (LexState *ls, int c, const char *msg) {
  if (!c) {
    if (ls->current != EOZ)
      save_and_next(ls);  /* add current to buffer for error message */
    lexerror(ls, msg, TK_STRING);
  }
}


static int gethexa (LexState *ls) {
  save_and_next(ls);
  esccheck (ls, lisxdigit(ls->current), "hexadecimal digit expected");
  return luaO_hexavalue(ls->current);
}


static int readhexaesc (LexState *ls) {
  int r = gethexa(ls);
  r = (r << 4) + gethexa(ls);
  luaZ_buffremove(ls->buff, 2);  /* remove saved chars from buffer */
  return r;
}


static unsigned long readutf8esc (LexState *ls) {
  unsigned long r;
  int i = 4;  /* chars to be removed: '\\', 'u', '{', and first digit */
  save_and_next(ls);  /* skip 'u' */
  esccheck(ls, ls->current == '{', "missing '{'");
  r = gethexa(ls);  /* must have at least one digit */
  while ((save_and_next(ls), lisxdigit(ls->current))) {
    i++;
    r = (r << 4) + luaO_hexavalue(ls->current);
    esccheck(ls, r <= 0x10FFFF, "UTF-8 value too large");
  }
  esccheck(ls, ls->current == '}', "missing '}'");
  next(ls);  /* skip '}' */
  luaZ_buffremove(ls->buff, i);  /* remove saved chars from buffer */
  return r;
}


static void utf8esc (LexState *ls) {
  char buff[UTF8BUFFSZ];
  int n = luaO_utf8esc(buff, readutf8esc(ls));
  for (; n > 0; n--)  /* add 'buff' to string */
    save(ls, buff[UTF8BUFFSZ - n]);
}


static int readdecesc (LexState *ls) {
  int i;
  int r = 0;  /* result accumulator */
  for (i = 0; i < 3 && lisdigit(ls->current); i++) {  /* read up to 3 digits */
    r = 10*r + ls->current - '0';
    save_and_next(ls);
  }
  esccheck(ls, r <= UCHAR_MAX, "decimal escape too large");
  luaZ_buffremove(ls->buff, i);  /* remove read digits from buffer */
  return r;
}


static void read_string (LexState *ls, int del, SemInfo *seminfo) {
  save_and_next(ls);  /* keep delimiter (for error messages) */
  while (ls->current != del) {
    switch (ls->current) {
      case EOZ:
        lexerror(ls, "unfinished string", TK_EOS);
        break;  /* to avoid warnings */
      case '\n':
      case '\r':
        lexerror(ls, "unfinished string", TK_STRING);
        break;  /* to avoid warnings */
      case '\\': {  /* escape sequences */
        int c;  /* final character to be saved */
        save_and_next(ls);  /* keep '\\' for error messages */
        switch (ls->current) {
          case 'a': c = '\a'; goto read_save;
          case 'b': c = '\b'; goto read_save;
          case 'f': c = '\f'; goto read_save;
          case 'n': c = '\n'; goto read_save;
          case 'r': c = '\r'; goto read_save;
          case 't': c = '\t'; goto read_save;
          case 'v': c = '\v'; goto read_save;
          case 'x': c = readhexaesc(ls); goto read_save;
          case 'u': utf8esc(ls);  goto no_save;
          case '\n': case '\r':
            inclinenumber(ls); c = '\n'; goto only_save;
          case '\\': case '"': case '\'': 
            c = ls->current; goto read_save;
          case EOZ: goto no_save;  /* will raise an error next loop */
          case 'z': {  /* zap following span of spaces */
            luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
            next(ls);  /* skip the 'z' */
            while (lisspace(ls->current)) {
              if (currIsNewline(ls)) inclinenumber(ls);
              else next(ls);
            }
            goto no_save;
          }
          default: {
            esccheck(ls, lisdigit(ls->current), "invalid escape sequence");
            c = readdecesc(ls);  /* digital escape '\\ddd' */
            goto only_save;
          }
        }
       read_save:
         next(ls);
         /* go through */
       only_save:
         luaZ_buffremove(ls->buff, 1);  /* remove '\\' */
         save(ls, c);
         /* go through */
       no_save: break;
      }
      default:
        save_and_next(ls);
    }
  }
  save_and_next(ls);  /* skip delimiter */
  seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + 1,
                                   luaZ_bufflen(ls->buff) - 2);
}

static size_t utf8seqlen (unsigned char c1) {
  if (c1 < 0x80) return 1;
  if ((c1 & 0xE0) == 0xC0) return 2;
  if ((c1 & 0xF0) == 0xE0) return 3;
  if ((c1 & 0xF8) == 0xF0) return 4;
  return 1;  /* invalid lead byte; handled by decoder */
}

/* Decode one UTF-8 codepoint from up to 4 already-peeked bytes. */
static utf8proc_int32_t utf8tocodepoint (unsigned char c1, unsigned char c2,
                                         unsigned char c3, unsigned char c4) {
  unsigned char bytes[4];
  utf8proc_int32_t codepoint;
  utf8proc_ssize_t nread;
  size_t len = utf8seqlen(c1);
  bytes[0] = c1;
  bytes[1] = c2;
  bytes[2] = c3;
  bytes[3] = c4;
  nread = utf8proc_iterate(bytes, (utf8proc_ssize_t)len, &codepoint);
  if (nread < 0)
    return -1;
  return codepoint;
}

/* Single source of truth for Unicode identifier continuation policy. */
static int iswordcodepoint (utf8proc_int32_t codepoint) {
  utf8proc_category_t cat;
  if (codepoint == 0x00B7)  /* U+00B7 MIDDLE DOT kept for existing identifiers */
    return 1;
  if (codepoint == 0x02C7)  /* U+02C7 CARON used as separator in cit framing */
    return 0;
  cat = utf8proc_category(codepoint);
  if (cat >= UTF8PROC_CATEGORY_LU && cat <= UTF8PROC_CATEGORY_LO) return 1;  /* letters */
  if (cat >= UTF8PROC_CATEGORY_MN && cat <= UTF8PROC_CATEGORY_ME) return 1;  /* combining marks */
  if (cat == UTF8PROC_CATEGORY_ND) return 1;                                  /* decimal digits */
  if (cat == UTF8PROC_CATEGORY_PC) return 1;                                  /* connector punct */
  return 0;
}

/* Unicode word delimiter = not an identifier continuation char. */
static int isworddelimiterutf8 (unsigned char c1, unsigned char c2,
                                unsigned char c3, unsigned char c4) {
  utf8proc_int32_t codepoint;
  if (c1 < 0x80)
    return (lisspace(c1) || (!lislalnum(c1) && c1 != '_'));
  codepoint = utf8tocodepoint(c1, c2, c3, c4);
  if (codepoint < 0)
    return 1;  /* invalid UTF-8 treated as separator */
  return !iswordcodepoint(codepoint);
}

static void skiponeutf8char (LexState *ls) {
  unsigned char c1 = cast_uchar(ls->current);
  next(ls);
  if (c1 >= 0xE0) {
    if (luai_isutf8cont(cast_uchar(ls->current)))
      next(ls);
    if (luai_isutf8cont(cast_uchar(ls->current)))
      next(ls);
  }
  else if (c1 >= 0xC0) {
    if (luai_isutf8cont(cast_uchar(ls->current)))
      next(ls);
  }
}

static int currentisseparator (LexState *ls) {
  unsigned char c1, c2 = 0, c3 = 0, c4 = 0;
  if (ls->current == EOZ)
    return 0;
  c1 = cast_uchar(ls->current);
  if (ls->z->n > 0)
    c2 = cast_uchar(ls->z->p[0]);
  if (ls->z->n > 1)
    c3 = cast_uchar(ls->z->p[1]);
  if (ls->z->n > 2)
    c4 = cast_uchar(ls->z->p[2]);
  return isworddelimiterutf8(c1, c2, c3, c4);
}

/* Find the start of a UTF-8 character at or before a buffer position */
static size_t find_utf8_start(const unsigned char *buffer, size_t pos) {
  while (pos > 0 && luai_isutf8cont(buffer[pos]))
    pos--;
  return pos;
}

/* Check if byte at buffer position (possibly mid-UTF-8) is a word delimiter */
static int bufferposisworddelim(const unsigned char *buffer, size_t buflen, size_t pos) {
  unsigned char c4 = 0;
  if (pos >= buflen)
    return 1;
  size_t start = find_utf8_start(buffer, pos);
  unsigned char c1 = buffer[start];
  unsigned char c2 = (start + 1 < buflen) ? buffer[start + 1] : 0;
  unsigned char c3 = (start + 2 < buflen) ? buffer[start + 2] : 0;
  if (start + 3 < buflen)
    c4 = buffer[start + 3];
  return isworddelimiterutf8(c1, c2, c3, c4);
}

/* Check if current input position is a word delimiter */
static int currentposisworddelim(LexState *ls) {
  unsigned char c1, c2 = 0, c3 = 0, c4 = 0;
  if (ls->current == EOZ)
    return 1;
  c1 = (unsigned char)ls->current;
  if (ls->z->n > 0)
    c2 = (unsigned char)ls->z->p[0];
  if (ls->z->n > 1)
    c3 = (unsigned char)ls->z->p[1];
  if (ls->z->n > 2)
    c4 = (unsigned char)ls->z->p[2];
  return isworddelimiterutf8(c1, c2, c3, c4);
}

static size_t trailingseparatorsize (Mbuffer *b) {
  size_t n = luaZ_bufflen(b);
  size_t start;
  unsigned char c1, c2 = 0, c3 = 0, c4 = 0;
  if (n == 0)
    return 0;
  start = n - 1;
  while (start > 0 && luai_isutf8cont(cast_uchar(luaZ_buffer(b)[start])))
    start--;
  c1 = cast_uchar(luaZ_buffer(b)[start]);
  if (start + 1 < n)
    c2 = cast_uchar(luaZ_buffer(b)[start + 1]);
  if (start + 2 < n)
    c3 = cast_uchar(luaZ_buffer(b)[start + 2]);
  if (start + 3 < n)
    c4 = cast_uchar(luaZ_buffer(b)[start + 3]);
  if (c1 == '\n' || c1 == '\r')
    return 0;
  if (isworddelimiterutf8(c1, c2, c3, c4))
    return n - start;
  return 0;
}

static size_t updatematch (size_t current, int c, const unsigned char *word) {
  if ((unsigned char)c == word[current]) return current + 1;
  if ((unsigned char)c == word[0]) return 1;
  return 0;
}

static void citescapeerror (LexState *ls) {
  const char *msg = luaO_pushfstring(ls->L,
                   "invalid escape sequence near 'ĥap'");
  lexerror(ls, msg, TK_STRING);
}

static void citskipspaces (LexState *ls) {
  while (lisspace(ls->current)) {
    if (currIsNewline(ls))
      inclinenumber(ls);
    else
      next(ls);
  }
}

static void saveutf8codepoint (LexState *ls, unsigned long cp) {
  char buff[UTF8BUFFSZ];
  int n = luaO_utf8esc(buff, cp);
  for (; n > 0; n--)
    save(ls, buff[UTF8BUFFSZ - n]);
}

static int parsehexvalue (const char *s, size_t len, unsigned long *out) {
  size_t i;
  unsigned long v = 0;
  if (len == 0)
    return 0;
  for (i = 0; i < len; i++) {
    int c = cast_uchar(s[i]);
    if (!lisxdigit(c))
      return 0;
    v = (v << 4) + luaO_hexavalue(c);
  }
  *out = v;
  return 1;
}

static int parsedecvalue (const char *s, size_t len, unsigned long *out) {
  size_t i;
  unsigned long v = 0;
  if (len == 0 || len > 3)
    return 0;
  for (i = 0; i < len; i++) {
    int c = cast_uchar(s[i]);
    if (!lisdigit(c))
      return 0;
    v = v * 10 + (unsigned long)(c - '0');
  }
  if (v > UCHAR_MAX)
    return 0;
  *out = v;
  return 1;
}

static void readcitescapefield (LexState *ls, char *out, size_t outsz) {
  size_t i = 0;
  while (ls->current != '-') {
    if (ls->current == EOZ || currIsNewline(ls))
      citescapeerror(ls);
    if (i + 1 >= outsz)
      citescapeerror(ls);
    out[i++] = cast(char, ls->current);
    next(ls);
  }
  out[i] = '\0';
  next(ls);  /* swallow '-' */
}

static void applycitescape (LexState *ls, const char *cmd, const char *p1) {
  unsigned long val = 0;
  size_t p1len = (p1 != NULL) ? strlen(p1) : 0;
  if (strcmp(cmd, "a") == 0 || strcmp(cmd, "alarme") == 0) { save(ls, '\a'); return; }
  if (strcmp(cmd, "b") == 0 || strcmp(cmd, "retropaŝe") == 0) { save(ls, '\b'); return; }
  if (strcmp(cmd, "f") == 0 || strcmp(cmd, "paĝosalte") == 0) { save(ls, '\f'); return; }
  if (strcmp(cmd, "n") == 0 || strcmp(cmd, "novlinie") == 0) { save(ls, '\n'); return; }
  if (strcmp(cmd, "r") == 0 || strcmp(cmd, "ĉaretrevene") == 0) { save(ls, '\r'); return; }
  if (strcmp(cmd, "t") == 0 || strcmp(cmd, "tabe") == 0) { save(ls, '\t'); return; }
  if (strcmp(cmd, "v") == 0 || strcmp(cmd, "vertikalatabe") == 0) { save(ls, '\v'); return; }
  if (strcmp(cmd, "\\") == 0 || strcmp(cmd, "retrostreko") == 0) { save(ls, '\\'); return; }
  if (strcmp(cmd, "\"") == 0 || strcmp(cmd, "citilo") == 0) { save(ls, '"'); return; }
  if (strcmp(cmd, "'") == 0 || strcmp(cmd, "apostrofo") == 0) { save(ls, '\''); return; }
  if (strcmp(cmd, "z") == 0 || strcmp(cmd, "spacglute") == 0) { citskipspaces(ls); return; }
  if (strcmp(cmd, "x") == 0 || strcmp(cmd, "deksesume") == 0) {
    if (p1 == NULL || p1len != 2 || !parsehexvalue(p1, p1len, &val))
      citescapeerror(ls);
    save(ls, cast(char, val));
    return;
  }
  if (strcmp(cmd, "u") == 0 || strcmp(cmd, "unikodpunkte") == 0) {
    if (p1 == NULL || !parsehexvalue(p1, p1len, &val))
      citescapeerror(ls);
    if (val > 0x10FFFF)
      citescapeerror(ls);
    saveutf8codepoint(ls, val);
    return;
  }
  if (strcmp(cmd, "dekume") == 0) {
    if (p1 == NULL || !parsedecvalue(p1, p1len, &val))
      citescapeerror(ls);
    save(ls, cast(char, val));
    return;
  }
  citescapeerror(ls);
}

static int citescapehasparam (const char *cmd) {
  return (strcmp(cmd, "x") == 0 || strcmp(cmd, "deksesume") == 0 ||
          strcmp(cmd, "u") == 0 || strcmp(cmd, "unikodpunkte") == 0 ||
          strcmp(cmd, "dekume") == 0);
}

static void readcitescapedirectparam (LexState *ls, const char *cmd, char *out, size_t outsz) {
  size_t i = 0;
  if (strcmp(cmd, "x") == 0 || strcmp(cmd, "deksesume") == 0) {
    while (lisxdigit(ls->current) && i < 2) {
      out[i++] = cast(char, ls->current);
      next(ls);
    }
    if (i != 2)
      citescapeerror(ls);
  }
  else if (strcmp(cmd, "u") == 0 || strcmp(cmd, "unikodpunkte") == 0) {
    while (lisxdigit(ls->current)) {
      if (i + 1 >= outsz)
        citescapeerror(ls);
      out[i++] = cast(char, ls->current);
      next(ls);
    }
    if (i == 0)
      citescapeerror(ls);
  }
  else if (strcmp(cmd, "dekume") == 0) {
    while (lisdigit(ls->current) && i < 3) {
      out[i++] = cast(char, ls->current);
      next(ls);
    }
    if (i == 0)
      citescapeerror(ls);
  }
  else
    citescapeerror(ls);
  out[i] = '\0';
}

static void readcitescape_directlong (LexState *ls) {
  char cmd[64];
  char p1[64];
  size_t i = 0;
  while (lislalpha(ls->current)) {
    if (i + 1 >= sizeof(cmd))
      citescapeerror(ls);
    cmd[i++] = cast(char, ls->current);
    next(ls);
  }
  cmd[i] = '\0';
  if (i <= 1)  /* monoletter direct commands are intentionally invalid */
    citescapeerror(ls);

  if (citescapehasparam(cmd)) {
    if (ls->current != '-')
      citescapeerror(ls);
    next(ls);  /* skip '-' */
    readcitescapedirectparam(ls, cmd, p1, sizeof(p1));
    applycitescape(ls, cmd, p1);
    return;
  }
  applycitescape(ls, cmd, NULL);
}

static void readcitescape_extended (LexState *ls) {
  char cmd[64];
  char p1[64];
  next(ls);  /* skip 'e' */
  if (ls->current != '-')
    citescapeerror(ls);
  next(ls);  /* skip '-' */
  readcitescapefield(ls, cmd, sizeof(cmd));  /* includes trailing '-' swallow */
  if (citescapehasparam(cmd)) {
    readcitescapefield(ls, p1, sizeof(p1));
    applycitescape(ls, cmd, p1);
    return;
  }
  applycitescape(ls, cmd, NULL);
}

static void readcitescape (LexState *ls) {
  if (ls->current == 'e' && ls->z->n > 0 && ls->z->p[0] == '-')
    readcitescape_extended(ls);
  else
    readcitescape_directlong(ls);
}

static void read_cit_string (LexState *ls, SemInfo *seminfo) {
  int line = ls->linenumber;  /* initial line (for error message) */
  static const unsigned char close_malcit[] = "malcit";
  static const unsigned char close_cxit[] = {0xC4, 0x89, 'i', 't'};
  static const unsigned char esc_hxaux[] = {0xC4, 0xA5, 'a', 0xC5, 0xAD};  /* ĥaŭ */
  static const unsigned char esc_hxap[] = {0xC4, 0xA5, 'a', 'p'};   /* ĥap */
  size_t m_malcit = 0;
  size_t m_cxit = 0;
  size_t m_esc_literal = 0;
  size_t m_esc_special = 0;
  int escaped_next = 0;
  int protect_trailing_sep = 0;

  /* Post 'cit', ignore one initial separator character if present. */
  if (currentisseparator(ls)) {
    if (currIsNewline(ls))
      inclinenumber(ls);
    else if (cast_uchar(ls->current) >= 0x80)
      skiponeutf8char(ls);
    else
      next(ls);
  }

  luaZ_resetbuffer(ls->buff);
  for (;;) {
    if (ls->current == EOZ) {
      const char *msg = luaO_pushfstring(ls->L,
                   "unfinished cit string (starting at line %d) near <eof>", line);
      lexerror(ls, msg, 0);
    }

    if (currIsNewline(ls)) {
      save(ls, '\n');
      inclinenumber(ls);
    }
    else
      save_and_next(ls);
    protect_trailing_sep = 0;

    {
      int c = (unsigned char)luaZ_buffer(ls->buff)[luaZ_bufflen(ls->buff) - 1];
      if (escaped_next) {
        escaped_next = 0;
        protect_trailing_sep = 1;
        m_esc_literal = 0;
        m_esc_special = 0;
        m_malcit = 0;
        m_cxit = 0;
        continue;
      }

      m_esc_literal = updatematch(m_esc_literal, c, esc_hxaux);
      if (m_esc_literal == sizeof(esc_hxaux)) {
        luaZ_buffremove(ls->buff, sizeof(esc_hxaux));
        escaped_next = 1;
        m_esc_literal = 0;
        m_esc_special = 0;
        m_malcit = 0;
        m_cxit = 0;
        continue;
      }

      m_esc_special = updatematch(m_esc_special, c, esc_hxap);
      if (m_esc_special == sizeof(esc_hxap)) {
        luaZ_buffremove(ls->buff, sizeof(esc_hxap));
        readcitescape(ls);
        protect_trailing_sep = 1;
        m_esc_literal = 0;
        m_esc_special = 0;
        m_malcit = 0;
        m_cxit = 0;
        continue;
      }

      m_malcit = updatematch(m_malcit, c, close_malcit);
      m_cxit = updatematch(m_cxit, c, close_cxit);
    }

    if (m_malcit == sizeof(close_malcit) - 1 || m_cxit == sizeof(close_cxit)) {
      size_t close_len = (m_malcit == sizeof(close_malcit) - 1)
                       ? (sizeof(close_malcit) - 1)
                       : sizeof(close_cxit);
      size_t start = luaZ_bufflen(ls->buff) - close_len;
      int before_is_delim = (start == 0) ? 1 : bufferposisworddelim(
          cast(unsigned char *, luaZ_buffer(ls->buff)), luaZ_bufflen(ls->buff), start - 1);
      int current_is_delim = currentposisworddelim(ls);

      if (before_is_delim && current_is_delim) {
        size_t sepbytes;
        luaZ_buffremove(ls->buff, close_len);
        if (!protect_trailing_sep) {
          sepbytes = trailingseparatorsize(ls->buff);
          if (sepbytes > 0)
            luaZ_buffremove(ls->buff, sepbytes);
        }
        seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff), luaZ_bufflen(ls->buff));
        return;
      }
    }
  }
}

/*
** Check if current position starts a valid identifier continuation in UTF-8
** Handles multi-byte sequences transparently
*/
static int isidentifiercont(LexState *ls) {
  unsigned char c1;
  unsigned char c2 = 0, c3 = 0, c4 = 0;
  utf8proc_int32_t codepoint;
  /* ponytail: EOZ is -1; casting to unsigned makes it 255 and would
     incorrectly look like UTF-8 data at end-of-file. */
  if (ls->current == EOZ)
    return 0;
  c1 = (unsigned char)ls->current;
  /* ASCII identifier continuation. */
  if (lislalnum(c1))
    return 1;
  /* Non-ASCII: accept UTF-8 letters and U+00B7 (middle dot), used in
     existing test names. This lets punctuation like '¡' be separators. */
  if (c1 < 0xC0)
    return 0;
  if (ls->z->n > 0)
    c2 = (unsigned char)ls->z->p[0];
  if (ls->z->n > 1)
    c3 = (unsigned char)ls->z->p[1];
  if (ls->z->n > 2)
    c4 = (unsigned char)ls->z->p[2];
  codepoint = utf8tocodepoint(c1, c2, c3, c4);
  if (codepoint < 0)
    return 0;
  return iswordcodepoint(codepoint);
}

static void saveutf8seq(LexState *ls) {
  unsigned char c1 = (unsigned char)ls->current;
  save_and_next(ls);  /* save leading byte */

  if (c1 >= 0xE0) {  /* 3-byte sequence */
    if (luai_isutf8cont((unsigned char)ls->current)) {
      save_and_next(ls);
      if (luai_isutf8cont((unsigned char)ls->current))
        save_and_next(ls);
    }
  }
  else if (c1 >= 0xC0) {  /* 2-byte sequence */
    if (luai_isutf8cont((unsigned char)ls->current))
      save_and_next(ls);
  }
}

static int isassignalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return ((len == 4 && memcmp(name, "iĝu", 4) == 0) ||
          (len == 4 && memcmp(name, "igxu", 4) == 0) ||
          (len == 4 && memcmp(name, "iĝe", 4) == 0) ||
          (len == 4 && memcmp(name, "igxe", 4) == 0));
}

static int iscommaalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return ((len == 3 && memcmp(name, "tuj", 3) == 0) ||
          (len == 4 && memcmp(name, "plie", 4) == 0));
}

static int issemicolonalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return (len == 2 && memcmp(name, "nu", 2) == 0);
}

static int islenalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return ((len == 4 && memcmp(name, "pese", 4) == 0) ||
          (len == 4 && memcmp(name, "kiom", 4) == 0) ||
          (len == 6 && memcmp(name, "kvante", 6) == 0) ||
          (len == 8 && memcmp(name, "amplekse", 8) == 0));
}

static int isdotalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return ((len == 2 && memcmp(name, "ie", 2) == 0) ||
          (len == 4 && memcmp(name, "ties", 4) == 0) ||
          (len == 6 && memcmp(name, "propra", 6) == 0));
}

static int isselfalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return (len == 3 && memcmp(name, "sia", 3) == 0);
}

static int iscitopenalias (TString *ts) {
  size_t len = tsslen(ts);
  const char *name = getstr(ts);
  return (len == 3 && memcmp(name, "cit", 3) == 0);
}

static int readbracketaliasunit (const char *name, size_t namelen, size_t pos,
                                 size_t *nextpos, int *token, int *nexttoken) {
  static const struct {
    const char *name;
    size_t len;
    int token;
    int nexttoken;
  } units[] = {
    { "cxe", 3, '[', 0 },
    { "cxi", 3, ']', 0 },
    { "cxa", 3, '{', 0 },
    { "cxo", 3, '}', 0 },
    { "are", 3, '}', 0 },
    { "ere", 3, ']', 0 },
    { "pri", 3, '{', 0 },
    { "je", 2, '(', 0 },
    { "ek", 2, ')', 0 },
    { "lo", 2, '(', ')' },
    { "ĉe", 3, '[', 0 },
    { "ĉi", 3, ']', 0 },
    { "ĉa", 3, '{', 0 },
    { "ĉo", 3, '}', 0 },
  };
  size_t i;
  for (i = 0; i < sizeof(units) / sizeof(units[0]); i++) {
    size_t unitlen = units[i].len;
    if (pos + unitlen > namelen)
      continue;
    if (memcmp(name + pos, units[i].name, unitlen) != 0)
      continue;
    *nextpos = pos + unitlen;
    *token = units[i].token;
    *nexttoken = units[i].nexttoken;
    return 1;
  }
  return 0;
}

static void skiponeseparator (LexState *ls) {
  if (!currentisseparator(ls))
    return;
  if (currIsNewline(ls))
    inclinenumber(ls);
  else if (cast_uchar(ls->current) >= 0x80)
    skiponeutf8char(ls);
  else
    next(ls);
}

static int shouldskipseparatorbeforeclosealias (LexState *ls) {
  const unsigned char *look;
  size_t lookn;
  size_t seqlen;
  size_t nextpos = 0;
  size_t checkpos;
  int token = 0;
  int nexttoken = 0;
  if (ls->current == EOZ || !currentisseparator(ls))
    return 0;
  /* Only process if we have open bracket aliases that haven't been closed */
  lua_assert(ls->bracket_alias_depth >= 0);
  if (ls->bracket_alias_depth <= 0)
    return 0;
  seqlen = utf8seqlen(cast_uchar(ls->current));
  if (seqlen == 1) {
    look = cast(const unsigned char *, ls->z->p);
    lookn = ls->z->n;
  }
  else {
    size_t skip = seqlen - 1;
    if (ls->z->n <= skip)
      return 0;
    look = cast(const unsigned char *, ls->z->p + skip);
    lookn = ls->z->n - skip;
  }
  if (!readbracketaliasunit(cast(const char *, look), lookn, 0, &nextpos, &token, &nexttoken))
    return 0;
  if (!(token == ')' || token == ']' || token == '}'))
    return 0;
  checkpos = nextpos;
  if (checkpos >= lookn)
    return 1;
  if (readbracketaliasunit(cast(const char *, look), lookn, checkpos, &nextpos, &token, &nexttoken))
    return 1;
  {
    unsigned char c1 = look[checkpos];
    unsigned char c2 = (checkpos + 1 < lookn) ? look[checkpos + 1] : 0;
    unsigned char c3 = (checkpos + 2 < lookn) ? look[checkpos + 2] : 0;
    unsigned char c4 = (checkpos + 3 < lookn) ? look[checkpos + 3] : 0;
    return isworddelimiterutf8(c1, c2, c3, c4);
  }
}

static void updatebracketaliasdepth (LexState *ls, int token) {
  if (token == '(' || token == '[' || token == '{') {
    /* Prevent integer overflow - if at max, next increment would wrap */
    if (ls->bracket_alias_depth == MAX_INT)
      lexerror(ls, "bracket nesting too deep", 0);
    ls->bracket_alias_depth++;
  }
  else if ((token == ')' || token == ']' || token == '}') &&
           ls->bracket_alias_depth > 0)
    ls->bracket_alias_depth--;
}

static int isbracketaliasagglutination (LexState *ls, TString *ts, int *firsttoken) {
  size_t namelen = tsslen(ts);
  const char *name = getstr(ts);
  size_t firstnextpos = 0;
  size_t pos = 0;
  size_t nextpos = 0;
  int token = 0;
  int nexttoken = 0;

  if (!readbracketaliasunit(name, namelen, pos, &firstnextpos, &token, &nexttoken))
    return 0;

  pos = firstnextpos;
  while (pos < namelen) {
    int ignored_token;
    int ignored_nexttoken;
    if (!readbracketaliasunit(name, namelen, pos, &nextpos,
                              &ignored_token, &ignored_nexttoken))
      return 0;
    pos = nextpos;
  }

  ls->pending_alias_ts = NULL;
  ls->pending_alias_pos = 0;
  ls->pending_extra_token = 0;
  if (nexttoken != 0)
    ls->pending_extra_token = nexttoken;
  if (firstnextpos < namelen) {
    ls->pending_alias_ts = ts;
    ls->pending_alias_pos = firstnextpos;
  }
  *firsttoken = token;
  return 1;
}

static int nametotoken (LexState *ls, TString *ts) {
  int token;
  if (!currentposisworddelim(ls))
    return TK_NAME;
  if (isbracketaliasagglutination(ls, ts, &token))
    return token;
  if (isreserved(ts)) {
    int token = ts->extra - 1 + FIRST_RESERVED;
    return token;
  }
  if (isassignalias(ts))
    return '=';
  if (iscommaalias(ts))
    return ',';
  if (issemicolonalias(ts))
    return ';';
  if (islenalias(ts))
    return '#';
  if (isdotalias(ts))
    return '.';
  return TK_NAME;
}

static int finishnametoken (LexState *ls, TString *ts, SemInfo *seminfo) {
  int token;
  if (!currentposisworddelim(ls)) {
    seminfo->ts = ts;
    return TK_NAME;
  }
  if (iscitopenalias(ts)) {
    read_cit_string(ls, seminfo);
    return TK_STRING;
  }
  if (isselfalias(ts))
    ts = luaS_newliteral(ls->L, "self");
  seminfo->ts = ts;
  token = nametotoken(ls, ts);
  updatebracketaliasdepth(ls, token);
  if ((token == '(' || token == '[' || token == '{') &&
      ls->pending_alias_ts == NULL && ls->pending_extra_token == 0)
    skiponeseparator(ls);
  return token;
}

static int llex (LexState *ls, SemInfo *seminfo) {
  if (ls->pending_extra_token != 0) {
    int token = ls->pending_extra_token;
    ls->pending_extra_token = 0;
    updatebracketaliasdepth(ls, token);
    return token;
  }
  if (ls->pending_alias_ts != NULL) {
    size_t namelen = tsslen(ls->pending_alias_ts);
    const char *name = getstr(ls->pending_alias_ts);
    size_t nextpos = 0;
    int token = 0;
    int nexttoken = 0;
    if (!readbracketaliasunit(name, namelen, ls->pending_alias_pos,
                              &nextpos, &token, &nexttoken)) {
      ls->pending_alias_ts = NULL;
      ls->pending_alias_pos = 0;
      ls->pending_extra_token = 0;
      return TK_NAME;
    }
    ls->pending_alias_pos = nextpos;
    if (ls->pending_alias_pos >= namelen) {
      ls->pending_alias_ts = NULL;
      ls->pending_alias_pos = 0;
    }
    if (nexttoken != 0)
      ls->pending_extra_token = nexttoken;
    updatebracketaliasdepth(ls, token);
    return token;
  }
  luaZ_resetbuffer(ls->buff);
  for (;;) {
    if (shouldskipseparatorbeforeclosealias(ls)) {
      skiponeseparator(ls);
      continue;
    }
    switch (ls->current) {
      case '\n': case '\r': {  /* line breaks */
        inclinenumber(ls);
        break;
      }
      case ' ': case '\f': case '\t': case '\v': {  /* spaces */
        next(ls);
        break;
      }
      case '-': {  /* '-' or '--' (comment) */
        next(ls);
        if (ls->current != '-') return '-';
        /* else is a comment */
        next(ls);
        if (ls->current == '[') {  /* long comment? */
          int sep = skip_sep(ls);
          luaZ_resetbuffer(ls->buff);  /* 'skip_sep' may dirty the buffer */
          if (sep >= 0) {
            read_long_string(ls, NULL, sep);  /* skip long comment */
            luaZ_resetbuffer(ls->buff);  /* previous call may dirty the buff. */
            break;
          }
        }
        /* else short comment */
        while (!currIsNewline(ls) && ls->current != EOZ)
          next(ls);  /* skip until end of line (or end of file) */
        break;
      }
      case '[': {  /* long string or simply '[' */
        int sep = skip_sep(ls);
        if (sep >= 0) {
          read_long_string(ls, seminfo, sep);
          return TK_STRING;
        }
        else if (sep != -1)  /* '[=...' missing second bracket */
          lexerror(ls, "invalid long string delimiter", TK_STRING);
        return '[';
      }
      case '=': {
        next(ls);
        if (check_next1(ls, '=')) return TK_EQ;
        else return '=';
      }
      case '<': {
        next(ls);
        if (check_next1(ls, '=')) return TK_LE;
        else if (check_next1(ls, '<')) return TK_SHL;
        else return '<';
      }
      case '>': {
        next(ls);
        if (check_next1(ls, '=')) return TK_GE;
        else if (check_next1(ls, '>')) return TK_SHR;
        else return '>';
      }
      case '/': {
        next(ls);
        if (check_next1(ls, '/')) return TK_IDIV;
        else return '/';
      }
      case '*': {
        next(ls);
        return TK_MUL;
      }
      case '~': {
        next(ls);
        if (check_next1(ls, '=')) return TK_NE;
        else return '~';
      }
      case ':': {
        next(ls);
        if (check_next1(ls, ':')) return TK_DBCOLON;
        else return TK_COLON;
      }
      case '"': case '\'': {  /* short literal strings */
        read_string(ls, ls->current, seminfo);
        return TK_STRING;
      }
      case '.': {  /* '.', '..', '...', or number */
        save_and_next(ls);
        if (check_next1(ls, '.')) {
          if (check_next1(ls, '.'))
            return TK_DOTS;   /* '...' */
          else return TK_CONCAT;   /* '..' */
        }
        else if (!lisdigit(ls->current)) return '.';
        else return read_numeral(ls, seminfo);
      }
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9': {
        return read_numeral(ls, seminfo);
      }
      case EOZ: {
        return TK_EOS;
      }
      default: {
        if (lislalpha(ls->current)) {  /* ASCII identifier or reserved word? */
          TString *ts;
          while (isidentifiercont(ls)) {
            if ((unsigned char)ls->current >= 0x80)
              saveutf8seq(ls);
            else
              save_and_next(ls);
          }
          ts = luaX_newstring(ls, luaZ_buffer(ls->buff),
                                  luaZ_bufflen(ls->buff));
          return finishnametoken(ls, ts, seminfo);
        }
        /* UTF-8 identifier start (non-ASCII) */
        else if ((unsigned char)ls->current >= 0xC0) {
          unsigned char c1, c2 = 0, c3 = 0;
          TString *ts;
          c1 = (unsigned char)ls->current;
          
          /* Peek ahead for multi-byte sequence */
          if (c1 >= 0xE0) {  /* 3-byte sequence */
            save_and_next(ls);  /* save c1 */
            if (luai_isutf8cont((unsigned char)ls->current)) {
              c2 = (unsigned char)ls->current;
              save_and_next(ls);  /* save c2 */
              if (luai_isutf8cont((unsigned char)ls->current)) {
                c3 = (unsigned char)ls->current;
                save_and_next(ls);  /* save c3 */
              }
            }
          } else if (c1 >= 0xC0) {  /* 2-byte sequence */
            save_and_next(ls);  /* save c1 */
            if (luai_isutf8cont((unsigned char)ls->current)) {
              c2 = (unsigned char)ls->current;
              save_and_next(ls);  /* save c2 */
            }
          }
          
          /* Validate it's a valid UTF-8 letter */
          if (!luai_isutf8alpha(c1, c2, c3)) {
            /* Not a valid letter, treat as single-char token */
            int ch = ls->current;
            next(ls);
            return ch;
          }
          
          /* Continue reading the identifier */
          while (isidentifiercont(ls)) {
            if ((unsigned char)ls->current >= 0xC0) {
              saveutf8seq(ls);
            } else if (lislalnum(ls->current)) {
              save_and_next(ls);
            } else if (luai_isutf8cont((unsigned char)ls->current)) {
              /* Continuation byte (shouldn't happen, but handle it) */
              save_and_next(ls);
            } else {
              break;
            }
          }
          
          ts = luaX_newstring(ls, luaZ_buffer(ls->buff),
                                  luaZ_bufflen(ls->buff));
          return finishnametoken(ls, ts, seminfo);
        }
        else {  /* single-char tokens (+ - / ...) */
          int c = ls->current;
          next(ls);
          return c;
        }
      }
    }
  }
}


void luaX_next (LexState *ls) {
  ls->lastline = ls->linenumber;
  if (ls->lookahead.token != TK_EOS) {  /* is there a look-ahead token? */
    ls->t = ls->lookahead;  /* use this one */
    ls->lookahead.token = TK_EOS;  /* and discharge it */
  }
  else
    ls->t.token = llex(ls, &ls->t.seminfo);  /* read next token */
}


int luaX_lookahead (LexState *ls) {
  lua_assert(ls->lookahead.token == TK_EOS);
  ls->lookahead.token = llex(ls, &ls->lookahead.seminfo);
  return ls->lookahead.token;
}
