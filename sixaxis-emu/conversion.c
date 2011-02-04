/*
 * conversion.c
 *
 *  Created on: 8 janv. 2011
 *      Author: matlo
 */

#include "conversion.h"
#include "sixaxis.h"
#include <string.h>

typedef struct {
    char bname[MAX_NAME_LENGTH];
    e_sixaxis_button_index bindex;
} s_button_name_index;

s_button_name_index button_name_index[SB_MAX] =
{
    {"up",       sb_up},
    {"down",     sb_down},
    {"right",    sb_right},
    {"left",     sb_left},
    {"r1",       sb_r1},
    {"r2",       sb_r2},
    {"r3",       sb_r3},
    {"l1",       sb_l1},
    {"l2",       sb_l2},
    {"l3",       sb_l3},
    {"circle",   sb_circle},
    {"square",   sb_square},
    {"cross",    sb_cross},
    {"triangle", sb_triangle},
    {"start",    sb_start},
    {"select",   sb_select},
    {"PS",       sb_ps}
};

int get_button_index_from_name(const char* name)
{
  int i;
  for(i=0; i<SB_MAX; ++i)
  {
    if(!strncmp(button_name_index[i].bname, name, MAX_NAME_LENGTH))
    {
      return button_name_index[i].bindex;
    }
  }
  return -1;
}

typedef struct {
    char bname[MAX_NAME_LENGTH];
    s_axis_index aindex;
} s_axis_name_index;

s_axis_name_index axis_name_index[] =
{
    {"rstick x",     {-1,            -1,  1,  0}},
    {"rstick y",     {-1,            -1,  1,  1}},
    {"lstick x",     {-1,            -1,  0,  0}},
    {"lstick y",     {-1,            -1,  0,  1}},
    {"rstick left",  {-1,          -127,  1,  0}},
    {"rstick right", {-1,           127,  1,  0}},
    {"rstick up",    {-1,          -127,  1,  1}},
    {"rstick down",  {-1,           127,  1,  1}},
    {"lstick left",  {-1,          -127,  0,  0}},
    {"lstick right", {-1,           127,  0,  0}},
    {"lstick up",    {-1,          -127,  0,  1}},
    {"lstick down",  {-1,           127,  0,  1}},
    {"up",           {sb_up,         -1, -1, -1}},
    {"down",         {sb_down,       -1, -1, -1}},
    {"right",        {sb_right,      -1, -1, -1}},
    {"left",         {sb_left,       -1, -1, -1}},
    {"r1",           {sb_r1,         -1, -1, -1}},
    {"r2",           {sb_r2,         -1, -1, -1}},
    {"l1",           {sb_l1,         -1, -1, -1}},
    {"l2",           {sb_l2,         -1, -1, -1}},
    {"circle",       {sb_circle,     -1, -1, -1}},
    {"square",       {sb_square,     -1, -1, -1}},
    {"cross",        {sb_cross,      -1, -1, -1}},
    {"triangle",     {sb_triangle,   -1, -1, -1}},
};

s_axis_index get_axis_index_from_name(const char* name)
{
  int i;
  s_axis_index none = {-1, -1, -1 -1};
  for(i=0; i<sizeof(axis_name_index)/sizeof(s_axis_name_index); ++i)
  {
    if(!strncmp(axis_name_index[i].bname, name, MAX_NAME_LENGTH))
    {
      return axis_name_index[i].aindex;
    }
  }
  return none;
}

typedef struct
{
  char arg[MAX_NAME_LENGTH];
  SDLKey key;
} s_arg_key;

/*
 * This table gives a correspondence between a char string and a key code.
 */
s_arg_key arg_key_table[SDLK_LAST] =
{
  {"UNKNOWN",         SDLK_UNKNOWN },
  {"FIRST",           SDLK_FIRST },
  {"BACKSPACE",       SDLK_BACKSPACE },
  {"TAB",             SDLK_TAB },
  {"CLEAR",           SDLK_CLEAR },
  {"RETURN",          SDLK_RETURN },
  {"PAUSE",           SDLK_PAUSE },
  {"ESCAPE",          SDLK_ESCAPE },
  {"SPACE",           SDLK_SPACE },
  {"EXCLAIM",         SDLK_EXCLAIM },
  {"QUOTEDBL",        SDLK_QUOTEDBL },
  {"HASH",            SDLK_HASH },
  {"DOLLAR",          SDLK_DOLLAR },
  {"AMPERSAND",       SDLK_AMPERSAND },
  {"QUOTE",           SDLK_QUOTE },
  {"LEFTPAREN",       SDLK_LEFTPAREN },
  {"RIGHTPAREN",      SDLK_RIGHTPAREN },
  {"ASTERISK",        SDLK_ASTERISK },
  {"PLUS",            SDLK_PLUS },
  {"COMMA",           SDLK_COMMA },
  {"MINUS",           SDLK_MINUS },
  {"PERIOD",          SDLK_PERIOD },
  {"SLASH",           SDLK_SLASH },
  {"0",               SDLK_0 },
  {"1",               SDLK_1 },
  {"2",               SDLK_2 },
  {"3",               SDLK_3 },
  {"4",               SDLK_4 },
  {"5",               SDLK_5 },
  {"6",               SDLK_6 },
  {"7",               SDLK_7 },
  {"8",               SDLK_8 },
  {"9",               SDLK_9 },
  {"COLON",           SDLK_COLON },
  {"SEMICOLON",       SDLK_SEMICOLON },
  {"LESS",            SDLK_LESS },
  {"EQUALS",          SDLK_EQUALS },
  {"GREATER",         SDLK_GREATER },
  {"QUESTION",        SDLK_QUESTION },
  {"AT",              SDLK_AT },
        /*
           Skip uppercase letters
         */
  {"LEFTBRACKET",     SDLK_LEFTBRACKET },
  {"BACKSLASH",       SDLK_BACKSLASH },
  {"RIGHTBRACKET",    SDLK_RIGHTBRACKET },
  {"CARET",           SDLK_CARET },
  {"UNDERSCORE",      SDLK_UNDERSCORE },
  {"BACKQUOTE",       SDLK_BACKQUOTE },
  {"a",               SDLK_a },
  {"b",               SDLK_b },
  {"c",               SDLK_c },
  {"d",               SDLK_d },
  {"e",               SDLK_e },
  {"f",               SDLK_f },
  {"g",               SDLK_g },
  {"h",               SDLK_h },
  {"i",               SDLK_i },
  {"j",               SDLK_j },
  {"k",               SDLK_k },
  {"l",               SDLK_l },
  {"m",               SDLK_m },
  {"n",               SDLK_n },
  {"o",               SDLK_o },
  {"p",               SDLK_p },
  {"q",               SDLK_q },
  {"r",               SDLK_r },
  {"s",               SDLK_s },
  {"t",               SDLK_t },
  {"u",               SDLK_u },
  {"v",               SDLK_v },
  {"w",               SDLK_w },
  {"x",               SDLK_x },
  {"y",               SDLK_y },
  {"z",               SDLK_z },
  {"DELETE",          SDLK_DELETE },
        /* End of ASCII mapped keysyms */

        /* International keyboard syms */
  {"WORLD_0",         SDLK_WORLD_0 },
  {"WORLD_1",         SDLK_WORLD_1 },
  {"WORLD_2",         SDLK_WORLD_2 },
  {"WORLD_3",         SDLK_WORLD_3 },
  {"WORLD_4",         SDLK_WORLD_4 },
  {"WORLD_5",         SDLK_WORLD_5 },
  {"WORLD_6",         SDLK_WORLD_6 },
  {"WORLD_7",         SDLK_WORLD_7 },
  {"WORLD_8",         SDLK_WORLD_8 },
  {"WORLD_9",         SDLK_WORLD_9 },
  {"WORLD_10",        SDLK_WORLD_10 },
  {"WORLD_11",        SDLK_WORLD_11 },
  {"WORLD_12",        SDLK_WORLD_12 },
  {"WORLD_13",        SDLK_WORLD_13 },
  {"WORLD_14",        SDLK_WORLD_14 },
  {"WORLD_15",        SDLK_WORLD_15 },
  {"WORLD_16",        SDLK_WORLD_16 },
  {"WORLD_17",        SDLK_WORLD_17 },
  {"WORLD_18",        SDLK_WORLD_18 },
  {"WORLD_19",        SDLK_WORLD_19 },
  {"WORLD_20",        SDLK_WORLD_20 },
  {"WORLD_21",        SDLK_WORLD_21 },
  {"WORLD_22",        SDLK_WORLD_22 },
  {"WORLD_23",        SDLK_WORLD_23 },
  {"WORLD_24",        SDLK_WORLD_24 },
  {"WORLD_25",        SDLK_WORLD_25 },
  {"WORLD_26",        SDLK_WORLD_26 },
  {"WORLD_27",        SDLK_WORLD_27 },
  {"WORLD_28",        SDLK_WORLD_28 },
  {"WORLD_29",        SDLK_WORLD_29 },
  {"WORLD_30",        SDLK_WORLD_30 },
  {"WORLD_31",        SDLK_WORLD_31 },
  {"WORLD_32",        SDLK_WORLD_32 },
  {"WORLD_33",        SDLK_WORLD_33 },
  {"WORLD_34",        SDLK_WORLD_34 },
  {"WORLD_35",        SDLK_WORLD_35 },
  {"WORLD_36",        SDLK_WORLD_36 },
  {"WORLD_37",        SDLK_WORLD_37 },
  {"WORLD_38",        SDLK_WORLD_38 },
  {"WORLD_39",        SDLK_WORLD_39 },
  {"WORLD_40",        SDLK_WORLD_40 },
  {"WORLD_41",        SDLK_WORLD_41 },
  {"WORLD_42",        SDLK_WORLD_42 },
  {"WORLD_43",        SDLK_WORLD_43 },
  {"WORLD_44",        SDLK_WORLD_44 },
  {"WORLD_45",        SDLK_WORLD_45 },
  {"WORLD_46",        SDLK_WORLD_46 },
  {"WORLD_47",        SDLK_WORLD_47 },
  {"WORLD_48",        SDLK_WORLD_48 },
  {"WORLD_49",        SDLK_WORLD_49 },
  {"WORLD_50",        SDLK_WORLD_50 },
  {"WORLD_51",        SDLK_WORLD_51 },
  {"WORLD_52",        SDLK_WORLD_52 },
  {"WORLD_53",        SDLK_WORLD_53 },
  {"WORLD_54",        SDLK_WORLD_54 },
  {"WORLD_55",        SDLK_WORLD_55 },
  {"WORLD_56",        SDLK_WORLD_56 },
  {"WORLD_57",        SDLK_WORLD_57 },
  {"WORLD_58",        SDLK_WORLD_58 },
  {"WORLD_59",        SDLK_WORLD_59 },
  {"WORLD_60",        SDLK_WORLD_60 },
  {"WORLD_61",        SDLK_WORLD_61 },
  {"WORLD_62",        SDLK_WORLD_62 },
  {"WORLD_63",        SDLK_WORLD_63 },
  {"WORLD_64",        SDLK_WORLD_64 },
  {"WORLD_65",        SDLK_WORLD_65 },
  {"WORLD_66",        SDLK_WORLD_66 },
  {"WORLD_67",        SDLK_WORLD_67 },
  {"WORLD_68",        SDLK_WORLD_68 },
  {"WORLD_69",        SDLK_WORLD_69 },
  {"WORLD_70",        SDLK_WORLD_70 },
  {"WORLD_71",        SDLK_WORLD_71 },
  {"WORLD_72",        SDLK_WORLD_72 },
  {"WORLD_73",        SDLK_WORLD_73 },
  {"WORLD_74",        SDLK_WORLD_74 },
  {"WORLD_75",        SDLK_WORLD_75 },
  {"WORLD_76",        SDLK_WORLD_76 },
  {"WORLD_77",        SDLK_WORLD_77 },
  {"WORLD_78",        SDLK_WORLD_78 },
  {"WORLD_79",        SDLK_WORLD_79 },
  {"WORLD_80",        SDLK_WORLD_80 },
  {"WORLD_81",        SDLK_WORLD_81 },
  {"WORLD_82",        SDLK_WORLD_82 },
  {"WORLD_83",        SDLK_WORLD_83 },
  {"WORLD_84",        SDLK_WORLD_84 },
  {"WORLD_85",        SDLK_WORLD_85 },
  {"WORLD_86",        SDLK_WORLD_86 },
  {"WORLD_87",        SDLK_WORLD_87 },
  {"WORLD_88",        SDLK_WORLD_88 },
  {"WORLD_89",        SDLK_WORLD_89 },
  {"WORLD_90",        SDLK_WORLD_90 },
  {"WORLD_91",        SDLK_WORLD_91 },
  {"WORLD_92",        SDLK_WORLD_92 },
  {"WORLD_93",        SDLK_WORLD_93 },
  {"WORLD_94",        SDLK_WORLD_94 },
  {"WORLD_95",        SDLK_WORLD_95 },

        /* Numeric keypad */
  {"KP0",             SDLK_KP0 },
  {"KP1",             SDLK_KP1 },
  {"KP2",             SDLK_KP2 },
  {"KP3",             SDLK_KP3 },
  {"KP4",             SDLK_KP4 },
  {"KP5",             SDLK_KP5 },
  {"KP6",             SDLK_KP6 },
  {"KP7",             SDLK_KP7 },
  {"KP8",             SDLK_KP8 },
  {"KP9",             SDLK_KP9 },
  {"KP_PERIOD",       SDLK_KP_PERIOD },
  {"KP_DIVIDE",       SDLK_KP_DIVIDE },
  {"KP_MULTIPLY",     SDLK_KP_MULTIPLY },
  {"KP_MINUS",        SDLK_KP_MINUS },
  {"KP_PLUS",         SDLK_KP_PLUS },
  {"KP_ENTER",        SDLK_KP_ENTER },
  {"KP_EQUALS",       SDLK_KP_EQUALS },

        /* Arrows + Home/End pad */
  {"UP",              SDLK_UP },
  {"DOWN",            SDLK_DOWN },
  {"RIGHT",           SDLK_RIGHT },
  {"LEFT",            SDLK_LEFT },
  {"INSERT",          SDLK_INSERT },
  {"HOME",            SDLK_HOME },
  {"END",             SDLK_END },
  {"PAGEUP",          SDLK_PAGEUP },
  {"PAGEDOWN",        SDLK_PAGEDOWN },

        /* Function keys */
  {"F1",              SDLK_F1 },
  {"F2",              SDLK_F2 },
  {"F3",              SDLK_F3 },
  {"F4",              SDLK_F4 },
  {"F5",              SDLK_F5 },
  {"F6",              SDLK_F6 },
  {"F7",              SDLK_F7 },
  {"F8",              SDLK_F8 },
  {"F9",              SDLK_F9 },
  {"F10",             SDLK_F10 },
  {"F11",             SDLK_F11 },
  {"F12",             SDLK_F12 },
  {"F13",             SDLK_F13 },
  {"F14",             SDLK_F14 },
  {"F15",             SDLK_F15 },

        /* Key state modifier keys */
  {"NUMLOCK",         SDLK_NUMLOCK },
  {"CAPSLOCK",        SDLK_CAPSLOCK },
  {"SCROLLOCK",       SDLK_SCROLLOCK },
  {"RSHIFT",          SDLK_RSHIFT },
  {"LSHIFT",          SDLK_LSHIFT },
  {"RCTRL",           SDLK_RCTRL },
  {"LCTRL",           SDLK_LCTRL },
  {"RALT",            SDLK_RALT },
  {"LALT",            SDLK_LALT },
  {"RMETA",           SDLK_RMETA },
  {"LMETA",           SDLK_LMETA },
  {"LSUPER",          SDLK_LSUPER },        /* Left "Windows" key */
  {"RSUPER",          SDLK_RSUPER },        /* Right "Windows" key */
  {"MODE",            SDLK_MODE },        /* "Alt Gr" key */
  {"COMPOSE",         SDLK_COMPOSE },        /* Multi-key compose key */

        /* Miscellaneous function keys */
  {"HELP",            SDLK_HELP },
  {"PRINT",           SDLK_PRINT },
  {"SYSREQ",          SDLK_SYSREQ },
  {"BREAK",           SDLK_BREAK },
  {"MENU",            SDLK_MENU },
  {"POWER",           SDLK_POWER },        /* Power Macintosh power key */
  {"EURO",            SDLK_EURO },        /* Some european keyboards */
  {"UNDO",            SDLK_UNDO },        /* Atari keyboard has Undo */

        /* Add any other keys here */

  {"LAST",            SDLK_LAST },
};

/*
 * This function gives a key code from a char string.
 */
SDLKey get_key_from_buffer(const char* buffer)
{
  int i;

  for (i = 0; i < SDLK_LAST; i++)
  {
    if (!strncmp(arg_key_table[i].arg, buffer, MAX_NAME_LENGTH))
    {
      return arg_key_table[i].key;
    }
  }

  return SDLK_UNKNOWN;
}

/*
 * This function gives a char string from a key code.
 */
const char* get_chars_from_key(SDLKey key)
{
  int i;

  for (i = 0; i < SDLK_LAST; i++)
  {
    if (arg_key_table[i].key == key)
    {
      return arg_key_table[i].arg;
    }
  }

  return arg_key_table[0].arg;
}

#define MOUSE_AXIS_X "x"
#define MOUSE_AXIS_Y "y"
#define MOUSE_BUTTON_LEFT "BUTTON_LEFT"
#define MOUSE_BUTTON_RIGHT "BUTTON_RIGHT"
#define MOUSE_BUTTON_MIDDLE "BUTTON_MIDDLE"
#define MOUSE_BUTTON_WHEELUP "BUTTON_WHEELUP"
#define MOUSE_BUTTON_WHEELDOWN "BUTTON_WHEELDOWN"
#define MOUSE_BUTTON_X1 "BUTTON_X1"
#define MOUSE_BUTTON_X2 "BUTTON_X2"
#define MOUSE_BUTTON_X3 "BUTTON_X3"

#define SDL_BUTTON_X3 8

unsigned int get_mouse_event_id_from_buffer(const char* event_id)
{
  unsigned int r_event_id = 0;

  if (!strncmp(event_id, MOUSE_AXIS_X, sizeof(MOUSE_AXIS_X)))
  {
    r_event_id = 0;
  }
  else if (!strncmp(event_id, MOUSE_AXIS_Y, sizeof(MOUSE_AXIS_Y)))
  {
    r_event_id = 1;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_LEFT, sizeof(MOUSE_BUTTON_LEFT)))
  {
    r_event_id = SDL_BUTTON_LEFT;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_RIGHT, sizeof(MOUSE_BUTTON_RIGHT)))
  {
    r_event_id = SDL_BUTTON_RIGHT;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_MIDDLE, sizeof(MOUSE_BUTTON_MIDDLE)))
  {
    r_event_id = SDL_BUTTON_MIDDLE;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_WHEELUP, sizeof(MOUSE_BUTTON_WHEELUP)))
  {
    r_event_id = SDL_BUTTON_WHEELUP;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_WHEELDOWN, sizeof(MOUSE_BUTTON_WHEELDOWN)))
  {
    r_event_id = SDL_BUTTON_WHEELDOWN;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_X1, sizeof(MOUSE_BUTTON_X1)))
  {
    r_event_id = SDL_BUTTON_X1;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_X2, sizeof(MOUSE_BUTTON_X2)))
  {
    r_event_id = SDL_BUTTON_X2;
  }
  else if (!strncmp(event_id, MOUSE_BUTTON_X3, sizeof(MOUSE_BUTTON_X3)))
  {
    r_event_id = SDL_BUTTON_X3;
  }

  return r_event_id;
}