/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "SF Mono:size=12", "FontAwesome:size=10", "DejaVu Mono:size=12"};
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      								instance                  title       						tags mask     isfloating   monitor */
	{ "term",                     NULL,                     NULL,       						0,            1,           -1 },
	{ "Gimp",                     NULL,                     NULL,                   0,            1,           -1 },
	{ "firefoxdeveloperedition",  NULL,                     NULL,                   1,            0,           -1 },
	{ "Brave-browser",  					NULL,                     NULL,                   1,            0,           -1 },
	{ "jetbrains-idea",           "jetbrains-idea",         NULL,                   1 << 2,       0,           -1 },
	{ "Postman",                  NULL,                     NULL,                   1 << 3,       0,           -1 },
	{ "Slack",                    NULL,                     NULL,                   1 << 1,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/fish", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-p", "", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *dropcmd[] = { "termdrop", NULL };
static const char *lock[]  = { "slock", NULL };
static const char *layout[]  = { "layout-selector", NULL };
static const char *redshift[]  = { "pkill", "-USR1", "redshift", NULL };

static const char *browsercmd[]  = { "brave", NULL };
static const char *slackcmd[]  = { "slack", NULL };
static const char *intellijcmd[]  = { "intellij-idea-ultimate-edition", NULL };
static const char *postmancmd[]  = { "postman", NULL };
static const char *pulsemixercmd[]  = { "st", "pulsemixer", NULL };

static const char *clippick[]  = { "cmpick", NULL };
static const char *clipclear[]  = { "cmdel", NULL };

static const char *screenselect[]  = { "scrsel", NULL };

static const char *upvol[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",     NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",     NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle",  NULL };
static const char *mutemic[] = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle",  NULL };

static const char *brightup[] = { "brightnessctl", "-d", "intel_backlight", "s", "+9%", NULL };
static const char *brightfull[] = { "brightnessctl", "-d", "intel_backlight", "s", "100%", NULL };
static const char *brightdown[] = { "brightnessctl", "-d", "intel_backlight", "s", "9%-", NULL };
static const char *brightmin[] = { "brightnessctl", "-d", "intel_backlight", "s", "1%", NULL };

static Key keys[] = {
	/* modifier                     key                            function        argument */
	{ 0     ,                       XK_F1,                         spawn,          {.v = dropcmd } },
	{ MODKEY,                       XK_space,                      spawn,          {.v = dmenucmd } },
	{ MODKEY|Mod1Mask,              XK_Return,                     spawn,          {.v = termcmd } },
	{ Mod1Mask,                     XK_Tab,                        focusstack,     {.i = +1 } },
	{ Mod1Mask|ShiftMask,           XK_Tab,                        focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_k,                          incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_j,                          incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,                          setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,                          setmfact,       {.f = +0.05} },
	{ MODKEY,             					XK_Return,                     zoom,           {0} },
	{ MODKEY,                       XK_Tab,                        view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,                          killclient,     {0} },
	{ MODKEY,                       XK_t,                          setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                          setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,                      togglefloating, {0} },
	{ MODKEY,                       XK_0,                          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                          tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                     tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                                          0)
	TAGKEYS(                        XK_2,                                          1)
	TAGKEYS(                        XK_3,                                          2)
	TAGKEYS(                        XK_4,                                          3)
	TAGKEYS(                        XK_5,                                          4)
	TAGKEYS(                        XK_6,                                          5)
	TAGKEYS(                        XK_7,                                          6)
	TAGKEYS(                        XK_8,                                          7)
	TAGKEYS(                        XK_9,                                          8)
	{ MODKEY,              					XK_l,                          spawn,          {.v = lock } },
	{ MODKEY,                       XK_F3,                         spawn,          {.v = layout } },
	{ MODKEY|Mod1Mask,              XK_r,                          spawn,          {.v = redshift } },
	{ MODKEY|Mod1Mask,              XK_b,                          spawn,          {.v = browsercmd } },
	{ MODKEY|Mod1Mask,              XK_s,                          spawn,          {.v = slackcmd } },
	{ MODKEY|Mod1Mask,              XK_i,                          spawn,          {.v = intellijcmd } },
	{ MODKEY|Mod1Mask,              XK_p,                          spawn,          {.v = postmancmd } },
	{ MODKEY|Mod1Mask,              XK_v,                          spawn,          {.v = pulsemixercmd } },
	{ MODKEY,                       XK_z,                          spawn,          {.v = clippick } },
	{ MODKEY|ShiftMask,             XK_z,                          spawn,          {.v = clipclear } },
  { 0,                            XF86XK_AudioLowerVolume,       spawn, 				 {.v = downvol } },
  { 0,                            XF86XK_AudioRaiseVolume,       spawn, 				 {.v = upvol   } },
  { 0,                            XF86XK_AudioMute,              spawn, 				 {.v = mutevol } },
  { ShiftMask,                    XF86XK_AudioMute,              spawn, 				 {.v = mutemic } },
  { 0,                            XF86XK_MonBrightnessUp,        spawn, 				 {.v = brightup } },
  { ShiftMask,                    XF86XK_MonBrightnessUp,        spawn, 				 {.v = brightfull } },
  { 0,                            XF86XK_MonBrightnessDown,      spawn, 				 {.v = brightdown } },
  { ShiftMask,                    XF86XK_MonBrightnessDown,      spawn, 				 {.v = brightmin } },
  { 0,                            XK_Print,                      spawn, 				 {.v = screenselect } },
	{ MODKEY|ShiftMask|ControlMask, XK_BackSpace,                  quit,  				 {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};

