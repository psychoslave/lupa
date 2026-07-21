/*
** $Id: llex.c,v 2.96 2016/05/02 14:02:12 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#define llex_c
#define LUA_CORE

#include "lprefix.h"


#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"

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

#define LEXTERM_RELATIVE	"./terminaro/llex.terms"
#define LEXTERM_INSTALLED	LUA_LDIR "terminaro/llex.terms"
#define LEXTERM_COUNT		(TK_EOS - FIRST_RESERVED + 1)
#define LEXERROR_COUNT		17

static TString *luaX_tokens[LEXTERM_COUNT];
static TString *luaX_envname;
static TString *luaX_messages[LEXERROR_COUNT];

static const int luaX_alias_tokens[] = {
 TK_AND, TK_BREAK, TK_BREAK, TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE,
 TK_FOR, TK_FUNCTION, TK_FUNCTION, TK_GOTO, TK_GOTO, TK_IF, TK_IN, TK_LOCAL,
 TK_LOCAL, TK_LOCAL, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_OR, TK_REPEAT,
 TK_RETURN, TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_UNTIL, TK_WHILE,
 TK_IDIV, TK_CONCAT, TK_DOTS, TK_EQ, TK_EQ, TK_GE, TK_GE, TK_LE, TK_NE,
 TK_BNOT, TK_BXOR, TK_BXOR, TK_GT, TK_GT, TK_GE, TK_GE, TK_EQ, TK_NE,
 TK_NE, TK_NE, TK_LT, TK_LT, TK_LE, TK_LE, TK_LE, TK_LE, TK_BAND, TK_BOR,
 TK_BOR, TK_SHR, TK_SHR, TK_SHL, TK_SHL, TK_ADD, TK_MINUS, TK_MINUS,
 TK_MINUS, TK_SUB, TK_DIV, TK_DIV, TK_DIV, TK_IDIV, TK_IDIV, TK_IDIV,
 TK_MOD, TK_MOD, TK_POW, TK_POW, TK_CONCAT, TK_CONCAT, TK_COLON
};

enum LexMsg {
 LXM_QUOTED_CHAR,
 LXM_QUOTED_STRING,
 LXM_NEAR,
 LXM_LEX_TOO_LONG,
 LXM_TOO_MANY_LINES,
 LXM_MALFORMED_NUMBER,
 LXM_HEXA_DIGIT,
 LXM_UTF8_TOO_LARGE,
 LXM_MISSING_LBRACE,
 LXM_MISSING_RBRACE,
 LXM_UNFINISHED_STRING,
 LXM_INVALID_ESCAPE,
 LXM_DEC_ESCAPE_TOO_LARGE,
 LXM_INVALID_LONG_DELIM,
 LXM_UNFINISHED_LONG,
 LXM_COMMENT,
 LXM_STRING
};

static int load_terminario_terms (lua_State *L, const char *path) {
 FILE *f = fopen(path, "r");
 char line[1024];
 int tok = 0;
 int ali = 0;
 int err = 0;
 if (f == NULL)
   return 0;
 while (fgets(line, sizeof(line), f) != NULL) {
   char *p = line;
   char *end;
   while (*p != '\0' && isspace((unsigned char)*p)) p++;
   if (*p == '\0' || *p == '#')
     continue;
   end = p + strlen(p);
   while (end > p && (end[-1] == '\n' || end[-1] == '\r' || isspace((unsigned char)end[-1])))
     *--end = '\0';
   if (strncmp(p, "ENV ", 4) == 0) {
     const char *s = p + 4;
     luaX_envname = luaS_newlstr(L, s, strlen(s));
     luaC_fix(L, obj2gco(luaX_envname));
   }
   else if (strncmp(p, "TOK ", 4) == 0) {
     const char *s = p + 4;
     TString *ts = luaS_newlstr(L, s, strlen(s));
     luaC_fix(L, obj2gco(ts));
     ts->extra = cast_byte(tok + 1);
     luaX_tokens[tok++] = ts;
   }
   else if (strncmp(p, "ALIAS ", 6) == 0) {
     const char *s = p + 6;
     TString *ts;
     if (ali >= (int)(sizeof(luaX_alias_tokens) / sizeof(luaX_alias_tokens[0])))
       luaL_error(L, "too many terminario aliases");
     ts = luaS_newlstr(L, s, strlen(s));
     luaC_fix(L, obj2gco(ts));
     ts->extra = cast_byte(luaX_alias_tokens[ali++] - FIRST_RESERVED + 1);
   }
   else if (strncmp(p, "ERR ", 4) == 0) {
     const char *s = p + 4;
     TString *ts;
     if (err >= LEXERROR_COUNT)
       luaL_error(L, "too many terminario errors");
     ts = luaS_newlstr(L, s, strlen(s));
     luaC_fix(L, obj2gco(ts));
     luaX_messages[err++] = ts;
   }
   else
     luaL_error(L, "bad terminario line: %s", p);
 }
 fclose(f);
 if (tok != LEXTERM_COUNT || ali != (int)(sizeof(luaX_alias_tokens) / sizeof(luaX_alias_tokens[0])) ||
     err != LEXERROR_COUNT || luaX_envname == NULL)
   luaL_error(L, "incomplete terminario terms from %s", path);
 return 1;
}

static void load_terminario_lex (lua_State *L) {
 if (!load_terminario_terms(L, LEXTERM_RELATIVE))
   if (!load_terminario_terms(L, LEXTERM_INSTALLED))
     luaL_error(L, "cannot load terminario lex terms");
}

#define save_and_next(ls) (save(ls, ls->current), next(ls))

static l_noret lexerror (LexState *ls, const char *msg, int token);

#define termmsg(i)	getstr(luaX_messages[(i)])


static void save (LexState *ls, int c) {
  Mbuffer *b = ls->buff;
  if (luaZ_bufflen(b) + 1 > luaZ_sizebuffer(b)) {
    size_t newsize;
    if (luaZ_sizebuffer(b) >= MAX_SIZE/2)
    lexerror(ls, termmsg(LXM_LEX_TOO_LONG), 0);
    newsize = luaZ_sizebuffer(b) * 2;
    luaZ_resizebuffer(ls->L, b, newsize);
  }
  b->buffer[luaZ_bufflen(b)++] = cast(char, c);
}


void luaX_init (lua_State *L) {
  int i;
  load_terminario_lex(L);
  for (i=0; i<LEXTERM_COUNT; i++) {
    if (luaX_tokens[i] == NULL)
      luaL_error(L, "missing terminario token #%d", i + 1);
  }
}


const char *luaX_token2str (LexState *ls, int token) {
  if (token < FIRST_RESERVED) {  /* single-byte symbols? */
    lua_assert(token == cast_uchar(token));
    return luaO_pushfstring(ls->L, termmsg(LXM_QUOTED_CHAR), token);
  }
  else {
    const char *s = getstr(luaX_tokens[token - FIRST_RESERVED]);
    if (token < TK_EOS)  /* fixed format (symbols and reserved words)? */
      return luaO_pushfstring(ls->L, termmsg(LXM_QUOTED_STRING), s);
    else  /* names, strings, and numerals */
      return s;
  }
}


static const char *txtToken (LexState *ls, int token) {
  switch (token) {
    case TK_NAME: case TK_STRING:
    case TK_FLT: case TK_INT:
      save(ls, '\0');
      return luaO_pushfstring(ls->L, termmsg(LXM_QUOTED_STRING), luaZ_buffer(ls->buff));
    default:
      return luaX_token2str(ls, token);
  }
}


static l_noret lexerror (LexState *ls, const char *msg, int token) {
  msg = luaG_addinfo(ls->L, msg, ls->source, ls->linenumber);
  if (token)
    luaO_pushfstring(ls->L, termmsg(LXM_NEAR), msg, txtToken(ls, token));
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
    lexerror(ls, termmsg(LXM_TOO_MANY_LINES), 0);
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
  ls->envn = luaX_envname;  /* get env name */
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
    lexerror(ls, termmsg(LXM_MALFORMED_NUMBER), TK_FLT);
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
        const char *what = (seminfo ? termmsg(LXM_STRING) : termmsg(LXM_COMMENT));
        const char *msg = luaO_pushfstring(ls->L,
                     termmsg(LXM_UNFINISHED_LONG), what, line);
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
  esccheck (ls, lisxdigit(ls->current), termmsg(LXM_HEXA_DIGIT));
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
  esccheck(ls, ls->current == '{', termmsg(LXM_MISSING_LBRACE));
  r = gethexa(ls);  /* must have at least one digit */
  while ((save_and_next(ls), lisxdigit(ls->current))) {
    i++;
    r = (r << 4) + luaO_hexavalue(ls->current);
    esccheck(ls, r <= 0x10FFFF, termmsg(LXM_UTF8_TOO_LARGE));
  }
  esccheck(ls, ls->current == '}', termmsg(LXM_MISSING_RBRACE));
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
  esccheck(ls, r <= UCHAR_MAX, termmsg(LXM_DEC_ESCAPE_TOO_LARGE));
  luaZ_buffremove(ls->buff, i);  /* remove read digits from buffer */
  return r;
}


static void read_string (LexState *ls, int del, SemInfo *seminfo) {
  save_and_next(ls);  /* keep delimiter (for error messages) */
  while (ls->current != del) {
    switch (ls->current) {
      case EOZ:
        lexerror(ls, termmsg(LXM_UNFINISHED_STRING), TK_EOS);
        break;  /* to avoid warnings */
      case '\n':
      case '\r':
        lexerror(ls, termmsg(LXM_UNFINISHED_STRING), TK_STRING);
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
            esccheck(ls, lisdigit(ls->current), termmsg(LXM_INVALID_ESCAPE));
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

/*
** Check if current position starts a valid identifier continuation in UTF-8
** Handles multi-byte sequences transparently
*/
static int isidentifiercont(LexState *ls) {
  /* ponytail: EOZ is -1; casting to unsigned makes it 255 and would
     incorrectly look like a UTF-8 continuation byte at end-of-file. */
  if (ls->current == EOZ)
    return 0;
  unsigned char c1 = (unsigned char)ls->current;
  /* ASCII identifier continuation or any non-ASCII UTF-8 byte. */
  return lislalnum(c1) || c1 >= 0x80;
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

static int llex (LexState *ls, SemInfo *seminfo) {
  luaZ_resetbuffer(ls->buff);
  for (;;) {
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
          lexerror(ls, termmsg(LXM_INVALID_LONG_DELIM), TK_STRING);
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
          seminfo->ts = ts;
          if (isreserved(ts))  /* reserved word? */
            return ts->extra - 1 + FIRST_RESERVED;
          else {
            return TK_NAME;
          }
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
          seminfo->ts = ts;
          if (isreserved(ts))  /* reserved word? */
            return ts->extra - 1 + FIRST_RESERVED;
          else {
            return TK_NAME;
          }
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
