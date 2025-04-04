/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=11:style=Bold" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, "#2E3440", col_gray2 },
	[SchemeSel]  = { col_gray4, "#485768",  "#bbbbbb"  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
   //   { "><>",      NULL },    /* no layout function means floating behavior */
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
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "kitty", "-T", scratchpadname, NULL };
//static const char *scratchpadcmd[] = {"kitty", "--class", scratchpadname, "--config", "/home/ayoub/.config/kitty/scratchpad-kitty.conf", NULL };
static const Key keys[] = {
	/* modifier                     key        function        argument */
        { MODKEY,                       XK_Return, spawn,          SHCMD("kitty") },
     //	{ MODKEY,                       XK_Return, spawn,          SHCMD("tabbed -r 2 st -w ''") },    
        { MODKEY,                       XK_e,      spawn,          SHCMD("nautilus") },
        { MODKEY,                       XK_b,      spawn,          SHCMD("brave") },
        { MODKEY,                       XK_r,      spawn,          SHCMD("/home/ayoub/.config/rofi/launchers/type-6/launcher.sh") }, 
        { MODKEY,                       XK_l,      spawn,          SHCMD("/home/ayoub/.config/rofi/custom/power-dwm/powermenu.sh") },  
    //	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("flameshot screen -p Pictures/") },
    	{ MODKEY,                       XK_p,      spawn,          SHCMD("/home/ayoub/.dwm/run-recent.sh") },
        { MODKEY,                       XK_z,      spawn,          SHCMD("emacsclient -c -a 'emacs'") },
	
   //switch focus window	
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } }, 
        { MODKEY,                       XK_Down,   focusstack,     {.i = +1 } }, 

   //split
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	
  //resize window
	{ MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = +0.05} },

 	
	{ MODKEY,                       XK_m,      zoom,           {0} }, // make window master 
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
//	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
//	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	
	// foating
      	{ MODKEY,             XK_v,  togglefloating, {0} }, 
	
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	 { MODKEY|ShiftMask|ControlMask, XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	//	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },


       	{ 0,            XF86XK_AudioLowerVolume,   spawn,          SHCMD(" wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-") },
	{ 0,            XF86XK_AudioRaiseVolume,   spawn,          SHCMD(" wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+") },
	{ 0,            XF86XK_AudioMute,          spawn,          SHCMD(" wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle") },

	{ 0,            XF86XK_AudioNext,          spawn,          SHCMD("playerctl next") },
        { 0,            XF86XK_AudioPlay,          spawn,          SHCMD("playerctl play-pause") },
        { 0,            XF86XK_AudioPause,         spawn,          SHCMD("playerctl play-pause") },
        { 0,            XF86XK_AudioPrev,          spawn,          SHCMD("playerctl previous") },


	//patches
 
	//fulgaps
    { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } }, // Decrease gaps
    { MODKEY,                       XK_equal,  setgaps,        {.i = +1 } }, // Increase gaps
    { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } }, // Reset gaps to 0

    // dwm-scratchpad


       	{ MODKEY,                       XK_s,  togglescratch,  {.v = scratchpadcmd } },

	// dwm-actualfullscreen
	{ MODKEY,             XK_f,      togglefullscr,  {0} },

};


/* If you use pipewire add somewhere in your constants definition section. Use "wpctl status" to
   find out the real sink ID, 0 is a placeholder here. */

	




/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};





/* patches */

// dwm-scratchpad



