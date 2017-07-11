#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "nini_parser.h"

#define REPORT_PARSE_ERROR(parser,format, args...) \
        nini_errmsg_write(&(parser)->errmsg, (parser)->line_index, (parser)->line.str, (format), ##args)

enum state_t
{
    STATE_BEGIN,
    STATE_INDENT,
    STATE_COMMENT,
    STATE_PRE_SECTION,
    STATE_SECTION,
    STATE_KEY,
    STATE_PRE_VALUE,
    STATE_QUOTED_VALUE,
    STATE_UNQUOTED_VALUE,
    STATE_VALUE_ESCAPE,
    STATE_VALUE_ESCAPE_X1,
    STATE_VALUE_ESCAPE_X2,
    STATE_TRAILING_SPACES,
};

typedef nini_parser_text_t  text_t;
typedef nini_parser_stack_t stack_t;

//------------------------------------------------------------------------------
static
void text_clear(text_t *self)
{
    memset(self, 0, sizeof(*self));
}
//------------------------------------------------------------------------------
static
bool text_is_empty(const text_t *self)
{
    return !self->len;
}
//------------------------------------------------------------------------------
static
void text_remove_trailing_spaces(text_t *self)
{
    while( self->len && self->str[ self->len - 1 ] == ' ' )
        self->str[ -- self->len ] = 0;
}
//------------------------------------------------------------------------------
static
bool text_push_ch(text_t *self, char ch)
{
    if( self->len >= NINI_MAX_LINE_CHARS ) return false;

    self->str[ self->len ++ ] = ch;
    return true;
}
//------------------------------------------------------------------------------
static
void text_pop_last_ch(text_t *self)
{
    if( self->len )
        self->str[ -- self->len ] = 0;
}
//------------------------------------------------------------------------------
static
void stack_clear(stack_t *self)
{
    self->count = 0;
}
//------------------------------------------------------------------------------
static
bool stack_push(stack_t *self, void *item)
{
    if( self->count >= NINI_MAX_PARSE_LEVEL ) return false;

    self->items[ self->count ++ ] = item;
    return true;
}
//------------------------------------------------------------------------------
static
void stack_pop(stack_t *self)
{
    if( self->count )
        -- self->count;
}
//------------------------------------------------------------------------------
static
void* stack_get_curr(stack_t *self)
{
    return self->count ? self->items[ self->count - 1 ] : NULL;
}
//------------------------------------------------------------------------------
static
void* on_item_default(void                *userarg,
                      void                *parent,
                      int                  level,
                      nini_type_t          type,
                      const char          *name,
                      nini_parser_value_t *value)
{
    return (void*) 1;
}
//------------------------------------------------------------------------------
void nini_parser_init(nini_parser_t         *self,
                      const nini_format_t   *format,
                      void                  *userarg,
                      nini_parser_on_item_t  on_item)
{
    self->format = *format;
    memset(&self->errmsg, 0, sizeof(self->errmsg));
    self->userarg = userarg;
    self->on_item = on_item ? on_item : on_item_default;
}
//------------------------------------------------------------------------------
static
void nini_parser_clear_line_state(nini_parser_t *self)
{
    self->state = STATE_BEGIN;

    self->indents = 0;
    text_clear(&self->line);
    text_clear(&self->section);
    text_clear(&self->key);
    text_clear(&self->value);

    memset(self->escape_x_chars, 0, sizeof(self->escape_x_chars));
}
//------------------------------------------------------------------------------
static
void nini_parser_reset(nini_parser_t *self)
{
    memset(&self->errmsg, 0, sizeof(self->errmsg));

    self->line_index = 1;
    self->last_level = 0;
    stack_clear(&self->stack);

    nini_parser_clear_line_state(self);
}
//------------------------------------------------------------------------------
static
bool strtobool(const char *str, bool *res)
{
    size_t len = strlen(str);
    if( len > 5 ) return false;

    char lostr[len+1];
    for(size_t i=0; i<len; ++i)
        lostr[i] = tolower(str[i]);
    lostr[len] = 0;

    if( 0 == strcmp(lostr, "1") ||
        0 == strcmp(lostr, "true") ||
        0 == strcmp(lostr, "yes") )
    {
        *res = true;
        return true;
    }

    if( 0 == strcmp(lostr, "0") ||
        0 == strcmp(lostr, "false") ||
        0 == strcmp(lostr, "no") )
    {
        *res = false;
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
static
nini_type_t read_value_and_type(text_t *text, nini_parser_value_t *value)
{
    if( text_is_empty(text) )
        return NINI_NULL;

    if( text->str[0] == '\"' )
    {
        assert( text->len >= 2 );
        text_pop_last_ch(text);
        value->string = text->str + 1;
        return NINI_STRING;
    }

    char *endpos;
    value->integer = strtol(text->str, &endpos, 10);
    if( *endpos == 0 ) return NINI_DECIMAL;

    value->integer = strtol(text->str, &endpos, 16);
    if( *endpos == 0 ) return NINI_HEXA;

    value->floating = strtod(text->str, &endpos);
    if( *endpos == 0 ) return NINI_FLOAT;

    if( strtobool(text->str, &value->boolean) )
        return NINI_BOOL;

    value->string = text->str;
    return NINI_STRING;
}
//------------------------------------------------------------------------------
static
bool process_newline_event(nini_parser_t *self)
{
    // Calculate level.
    int level;
    if( self->format.indent )
        level = self->indents / self->format.indent;
    else
        level = text_is_empty(&self->section) ? 1 : 0;

    // Decide who is parent?
    if( level <= self->last_level )
        stack_pop(&self->stack);
    if( level < self->last_level )
        stack_pop(&self->stack);
    void *parent = stack_get_curr(&self->stack);

    // Detect type.
    nini_parser_value_t value;
    nini_type_t type = !text_is_empty(&self->section) ?
                       NINI_SECTION : read_value_and_type(&self->value, &value);

    // Select name.
    const char *name = type == NINI_SECTION ? self->section.str : self->key.str;

    // Process user event.
    void *item = self->on_item(self->userarg,
                               parent,
                               level,
                               type,
                               name,
                               ( type == NINI_SECTION ? NULL : &value ));
    if( !item )
    {
        REPORT_PARSE_ERROR(self, "Item event error!");
        return false;
    }

    // Save information for next item.

    if( !stack_push(&self->stack, item) )
    {
        REPORT_PARSE_ERROR(self, "Indent level too deep!");
        return false;
    }

    self->last_level = level;

    return true;
}
//------------------------------------------------------------------------------
static
bool state_to_next_line(nini_parser_t *self)
{
    text_remove_trailing_spaces(&self->section);
    text_remove_trailing_spaces(&self->key);
    text_remove_trailing_spaces(&self->value);

    if( !text_is_empty(&self->section) || !text_is_empty(&self->key) )
    {
        if( !process_newline_event(self) )
            return false;
    }

    nini_parser_clear_line_state(self);
    ++ self->line_index;

    return true;
}
//------------------------------------------------------------------------------
static
bool check_if_indents_available(nini_parser_t *self, int indents)
{
    const nini_format_t *format = &self->format;

    if( format->indent && indents % format->indent )
    {
        REPORT_PARSE_ERROR(self, "Invalid indents!");
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
static
bool read_char_for_indents(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_INDENT );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;

    if( ch == ' ' )
    {
        ++ self->indents;
        return true;
    }
    else if( ch == '\n' )
    {
        return state_to_next_line(self);
    }
    else if( format->comment && ch == format->comment )
    {
        self->state = STATE_COMMENT;
        return true;
    }
    else if( format->sec_head && ch == format->sec_head )
    {
        if( !check_if_indents_available(self, self->indents) )
            return false;

        self->state = STATE_PRE_SECTION;
        return true;
    }
    else
    {
        if( !check_if_indents_available(self, self->indents) )
            return false;

        bool succ = text_push_ch(&self->key, ch);
        assert( succ );  // Because the line buffer will overflow first!

        self->state = STATE_KEY;
        return true;
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_begin(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_BEGIN );

    self->state = STATE_INDENT;
    return read_char_for_indents(self, ch);
}
//------------------------------------------------------------------------------
static
bool read_char_for_comments(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_COMMENT );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;
    assert( format->comment );

    if( ch == '\n' )
    {
        if( !state_to_next_line(self) )
            return false;
    }

    return true;
}
//------------------------------------------------------------------------------
static
bool read_char_for_section(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_SECTION );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;
    assert( format->sec_head );

    if( ch == '\n' )
    {
        return state_to_next_line(self);
    }
    else if( format->comment && ch == format->comment )
    {
        if( format->sec_tail )
        {
            REPORT_PARSE_ERROR(self, "Invalid character in section name!");
            return false;
        }
        else
        {
            self->state = STATE_COMMENT;
            return true;
        }
    }
    else if( ch == format->sec_head )
    {
        REPORT_PARSE_ERROR(self, "Invalid character in section name!");
        return false;
    }
    else if( format->sec_tail && ch == format->sec_tail )
    {
        self->state = STATE_TRAILING_SPACES;
        return true;
    }
    else if( ch == format->keymark )
    {
        REPORT_PARSE_ERROR(self, "Invalid character in section name!");
        return false;
    }
    else
    {
        bool succ = text_push_ch(&self->section, ch);
        assert( succ );  // Because the line buffer will overflow first!
        return true;
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_pre_section(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_PRE_SECTION );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;
    assert( format->sec_head );

    if( ch == ' ' )
    {
        return true;
    }
    else if( ch == '\n' )
    {
        REPORT_PARSE_ERROR(self, "Section name empty!");
        return false;
    }
    else if( format->comment && ch == format->comment )
    {
        REPORT_PARSE_ERROR(self, "Section name empty!");
        return false;
    }
    else if( format->sec_tail && ch == format->sec_tail )
    {
        REPORT_PARSE_ERROR(self, "Section name empty!");
        return false;
    }
    else
    {
        text_pop_last_ch(&self->line);

        self->state = STATE_SECTION;
        return read_char_for_section(self, ch);
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_key(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_KEY );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;
    assert( format->keymark );

    if( ch == '\n' )
    {
        REPORT_PARSE_ERROR(self, "The key-value delimiter is missing!");
        return false;
    }
    else if( format->comment && ch == format->comment )
    {
        REPORT_PARSE_ERROR(self, "Invalid character in key!");
        return false;
    }
    else if( format->sec_head && ch == format->sec_head )
    {
        REPORT_PARSE_ERROR(self, "Invalid character in key!");
        return false;
    }
    else if( format->sec_tail && ch == format->sec_tail )
    {
        REPORT_PARSE_ERROR(self, "Invalid character in key!");
        return false;
    }
    else if( ch == format->keymark )
    {
        self->state = STATE_PRE_VALUE;
        return true;
    }
    else
    {
        bool succ = text_push_ch(&self->key, ch);
        assert( succ );  // Because the line buffer will overflow first!

        return true;
    }
}
//------------------------------------------------------------------------------
static
bool get_ch_from_hex_digits(nini_parser_t *self, const char *str, char *ch)
{
    char *endpos;
    long value = strtol(str, &endpos, 16);
    if( *endpos )
    {
        REPORT_PARSE_ERROR(self, "Invalid value format!");
        return false;
    }

    *ch = value;
    return true;
}
//------------------------------------------------------------------------------
static
bool read_char_for_value_escape(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_VALUE_ESCAPE    ||
            self->state == STATE_VALUE_ESCAPE_X1 ||
            self->state == STATE_VALUE_ESCAPE_X2 );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    if( ch == '\n' )
    {
        REPORT_PARSE_ERROR(self, "Invalid value format!");
        return false;
    }
    else if( self->state == STATE_VALUE_ESCAPE && ch == 'x' )
    {
        self->state = STATE_VALUE_ESCAPE_X1;
        return true;
    }
    else if( self->state == STATE_VALUE_ESCAPE_X1 )
    {
        self->escape_x_chars[0] = ch;

        self->state = STATE_VALUE_ESCAPE_X2;
        return true;
    }
    else if( self->state == STATE_VALUE_ESCAPE_X2 )
    {
        self->escape_x_chars[1] = ch;
        if( !get_ch_from_hex_digits(self, self->escape_x_chars, &ch) )
            return false;

        memset(self->escape_x_chars, 0, sizeof(self->escape_x_chars));
    }
    else
    {
        switch( ch )
        {
        case 'a':
            ch = 0x07;
            break;

        case 'b':
            ch = 0x08;
            break;

        case 'f':
            ch = 0x0C;
            break;

        case 'n':
            ch = 0x0A;
            break;

        case 'r':
            ch = 0x0D;
            break;

        case 't':
            ch = 0x09;
            break;

        case 'v':
            ch = 0x0B;
            break;
        }
    }

    bool succ = text_push_ch(&self->value, ch);
    assert( succ );  // Because the line buffer will overflow first!

    self->state = STATE_QUOTED_VALUE;
    return true;
}
//------------------------------------------------------------------------------
static
bool read_char_for_quoted_value(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_QUOTED_VALUE );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    if( ch == '\n' )
    {
        return state_to_next_line(self);
    }
    else if( ch == '\"' )
    {
        bool succ = text_push_ch(&self->value, ch);
        assert( succ );  // Because the line buffer will overflow first!

        self->state = STATE_TRAILING_SPACES;
        return true;
    }
    else if( ch == '\\' )
    {
        self->state = STATE_VALUE_ESCAPE;
        return true;
    }
    else
    {
        bool succ = text_push_ch(&self->value, ch);
        assert( succ );  // Because the line buffer will overflow first!

        return true;
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_unquoted_value(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_UNQUOTED_VALUE );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;
    assert( format->keymark );

    if( ch == '\n' )
    {
        return state_to_next_line(self);
    }
    else if( format->comment && ch == format->comment )
    {
        self->state = STATE_COMMENT;
        return true;
    }
    else
    {
        bool succ = text_push_ch(&self->value, ch);
        assert( succ );  // Because the line buffer will overflow first!

        return true;
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_pre_value(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_PRE_VALUE );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    if( ch == ' ' )
    {
        return true;
    }
    else if( ch == '\"' )
    {
        bool succ = text_push_ch(&self->value, ch);
        assert( succ );  // Because the line buffer will overflow first!

        self->state = STATE_QUOTED_VALUE;
        return true;
    }
    else
    {
        text_pop_last_ch(&self->line);

        self->state = STATE_UNQUOTED_VALUE;
        return read_char_for_unquoted_value(self, ch);
    }
}
//------------------------------------------------------------------------------
static
bool read_char_for_trailing_spaces(nini_parser_t *self, char ch)
{
    assert( self->state == STATE_TRAILING_SPACES );

    if( !text_push_ch(&self->line, ch) )
    {
        REPORT_PARSE_ERROR(self, "This line is too long!");
        return false;
    }

    const nini_format_t *format = &self->format;

    if( ch == ' ' )
    {
        return true;
    }
    else if( ch == '\n' )
    {
        return state_to_next_line(self);
    }
    else if( format->comment && ch == format->comment )
    {
        self->state = STATE_COMMENT;
        return true;
    }
    else
    {
        REPORT_PARSE_ERROR(self, "Unexpected trailing characters!");
        return false;
    }
}
//------------------------------------------------------------------------------
static
bool read_char(nini_parser_t *self, char ch)
{
    switch( self->state )
    {
    case STATE_BEGIN:
        return read_char_for_begin(self, ch);

    case STATE_INDENT:
        return read_char_for_indents(self, ch);

    case STATE_COMMENT:
        return read_char_for_comments(self, ch);

    case STATE_PRE_SECTION:
        return read_char_for_pre_section(self, ch);

    case STATE_SECTION:
        return read_char_for_section(self, ch);

    case STATE_KEY:
        return read_char_for_key(self, ch);

    case STATE_PRE_VALUE:
        return read_char_for_pre_value(self, ch);

    case STATE_QUOTED_VALUE:
        return read_char_for_quoted_value(self, ch);

    case STATE_UNQUOTED_VALUE:
        return read_char_for_unquoted_value(self, ch);

    case STATE_VALUE_ESCAPE:
    case STATE_VALUE_ESCAPE_X1:
    case STATE_VALUE_ESCAPE_X2:
        return read_char_for_value_escape(self, ch);

    case STATE_TRAILING_SPACES:
        return read_char_for_trailing_spaces(self, ch);
    }

    REPORT_PARSE_ERROR(self, "Internal error!");
    return false;
}
//------------------------------------------------------------------------------
bool nini_parser_parse(nini_parser_t *self, const void *data, size_t size)
{
    nini_parser_reset(self);

    for(const uint8_t *pos = data; size; ++pos, --size)
    {
        if( !read_char(self, *pos) )
            return false;
    }

    if( self->state != STATE_BEGIN )
    {
        REPORT_PARSE_ERROR(self, "Incomplete file!");
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void nini_parser_get_errmsg(const nini_parser_t *self, nini_errmsg_t *errmsg)
{
    *errmsg = self->errmsg;
}
//------------------------------------------------------------------------------
