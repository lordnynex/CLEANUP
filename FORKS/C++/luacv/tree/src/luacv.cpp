extern "C" 
{
  #include "lua.h"
  #include "lauxlib.h"
  #include <csignal>
}

#include "luacvaux.h"

#include "lua_core.h"
#include "lua_imgproc.h"
#include "lua_calib3d.h"
#include "lua_highgui.h"
#include "lua_legacy.h"
#include "lua_objdetect.h"
#include "lua_features2d.h"
#include "lua_video.h"

static int luacv_tostring(lua_State *L)
{
  lua_pushfstring(L,"LuaCV %d.%d.%d - Lua wrapper for OpenCV library. See http://sourceforge.net/projects/luacv for more details.\nCompiled with:\n\tOpenCV "CV_VERSION"\n\t"LUA_VERSION"\n\tC++ compiler "__VERSION__,LUACV_VERSION_MAJOR,LUACV_VERSION_MINOR,LUACV_VERSION_SUBMINOR);
  return 1;
}

static int luacv_newindex(lua_State *L)
{
  luaL_error(L,"You are trying rewrite read-only table.");
  return 0;
}

static const luaL_Reg luacv_m[]=
{
  {"__tostring",luacv_tostring},
  {"__newindex",luacv_newindex},
  {NULL,NULL}
};


static const luaL_Reg luacv[]=
{
  {NULL,NULL}
};

static lua_Debug luacv_debug;
static lua_State *luacv_globalstack;

#if defined(WIN32) || defined(WIN64)
static LONG __stdcall luacv_sighandler(LPEXCEPTION_POINTERS exceptionPtrs)
{
#else
static void luacv_sighandler(int signum)
{
	  signum=signum;//to avoid warnings
#endif
    lua_getinfo(luacv_globalstack,"nS",&luacv_debug);
    char line[255];
    char sep[255];

    sprintf(line,"LuaCV catched OpenCV error in %s function "LIBNAME".%s()",luacv_debug.short_src,luacv_debug.name);
    int len=strlen(line);
    for(int i=0;i<len;i++)
      sep[i]='=';
    fprintf(stderr,"%s\n%s\n%s\nReference in ",sep,line,sep);

    lua_gc(luacv_globalstack,LUA_GCCOLLECT,0);

    lua_getglobal(luacv_globalstack,"package");
    lua_getfield(luacv_globalstack,-1,"loaded");
    lua_getfield(luacv_globalstack,-1,LIBNAME);
    lua_getfield(luacv_globalstack,-1,luacv_debug.name);
    lua_call(luacv_globalstack,0,1);
#if defined(WIN32) || defined(WIN64)
	return EXCEPTION_EXECUTE_HANDLER;
#endif
}
  
static void luacv_hook(lua_State *L,lua_Debug *ar)
{
  luacv_globalstack=L;
  luacv_debug=*ar;
}

extern "C" {
#if defined(WIN32) || defined(WIN64)
__declspec(dllexport) 
#endif
int luaopen_luacv(lua_State *L)
{

  //**************** OpenCV Types registration **************//
  objectReg(LIBNAME,luacv_m);

  coreReg(); 
  imgprocReg();
  highguiReg();
  calib3dReg()
  legacyReg();
  objdetectReg(); 
  features2dReg();
  videoReg();

  //**************** luacv registration ****************//
  #ifndef LUA_VERSION_MINOR   
    luaL_register(L,LIBNAME,luacv);
  #else 
    luaL_newlib(L,luacv);
  #endif
  //****************** core registration ****************//
  luaL_register(L,NULL,core);
//  luaL_setfuncs(L,core,0);
  luacv_objTableReg(L,core_object);
  luacv_tableConvFuncReg(L,core_matconv);
  luacv_tableConvFuncReg(L,core_imageconv);
  luacv_constReg(L,core_var);

  //***************** imgproc registratoin ************//
  luaL_register(L,NULL,imgproc);
  //luaL_setfuncs(L,imgproc,0);
  luacv_objTableReg(L,imgproc_object);
  luacv_constReg(L,imgproc_var);

  //***************** objdetect registratoin ************//
  luaL_register(L,NULL,objdetect);
  //luaL_setfuncs(L,objdetect,0);
  luacv_objTableReg(L,objdetect_object);
  luacv_constReg(L,objdetect_var);

  //***************** features2d registratoin ************//
  luaL_register(L,NULL,features2d);
  //luaL_setfuncs(L,features2d,0);
  luacv_objTableReg(L,features2d_object);
  luacv_constReg(L,features2d_var);

  //***************** legacy registratoin ************//
  luaL_register(L,NULL,legacy);
  //luaL_setfuncs(L,legacy,0);
  luacv_constReg(L,legacy_var);

  //***************** calib3d registratoin ************//
  luaL_register(L,NULL,calib3d);
  //luaL_setfuncs(L,tracking,0);
  luacv_objTableReg(L,calib3d_object);
  luacv_constReg(L,calib3d_var);

  //***************** highgui registratoin ************//
  luaL_register(L,NULL,highgui);
  //luaL_setfuncs(L,highgui,0);
  luacv_constReg(L,highgui_var);

  lua_pushstring(L,LIBCALLBACKS);
  lua_newtable(L);
  lua_rawset(L,-3);

  //***************** video registratoin ************//
  luaL_register(L,NULL,video);
//  luaL_setfuncs(L,video,0);
  luacv_objTableReg(L,video_object);
  luacv_constReg(L,video_var);

  //**************** luacv methods registration ****************//
  
  luaL_getmetatable(L,LIBNAME);
  lua_setmetatable(L,-2);


  //**************** OpenCV SIGABRT catching ************//
  lua_sethook(L,luacv_hook,LUA_MASKCALL,0);

#if defined(WIN32) || defined(WIN64)
  SetUnhandledExceptionFilter(luacv_sighandler);
#else
  struct sigaction sa;
  sigset_t set;
  sigemptyset(&(sa.sa_mask));
  sa.sa_flags=0;
  sa.sa_handler=luacv_sighandler;
  sigaction(SIGABRT,&sa,NULL);

  sigemptyset(&set);
  sigaddset(&set,SIGABRT);
  sigprocmask(SIG_BLOCK,&set,NULL);
#endif
  return 1;

}
}
