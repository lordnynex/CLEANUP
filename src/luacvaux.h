#ifndef LUACVAUX
#define LUACVAUX
extern "C" 
{
#include "lua.h"
#include "lauxlib.h"
}

#include <string.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"

#define LIBNAME "luacv"
#define LIBCALLBACKS "Callbacks"
#define MAX_CALLBACKS 100

#ifdef _MSC_VER
	#pragma warning ( disable : 4706 )
	#pragma warning ( disable : 4610 )
	#pragma warning ( disable : 4512 )
	#pragma warning ( disable : 4510 )
#endif

//Lua 5.2 changed API so this is compatibility measures
#ifdef LUA_VERSION_MINOR
  #define luaL_register(L,str,table) luaL_setfuncs(L,table,0)
  #define lua_objlen lua_rawlen
  #define LUAI_MAXVARS 500
#endif

struct luacv_var
{
  const char *name;
  const double value;
};

struct luacv_method
{
  const char *name;
  unsigned int len;
  lua_CFunction newindex; 
  lua_CFunction index;
};

template <typename cvtype>
struct luacv_obj
{
  cvtype *data;
  bool dealloc_data;
  int ref;
};

struct luacv_callback
{
  int pos;
  lua_State *stack;
  char wname[256];
};

typedef struct luacv_method luaL_ConvFunc;

void luacv_constReg (lua_State *L,const luacv_var* var);
void luacv_tableConvFuncReg (lua_State *L,const luaL_ConvFunc* var);
void luacv_objTableReg (lua_State *L,const luacv_var* var);
int luacv_argerror (lua_State *L, int narg, const char *extramsg,const char *funcmsg);
const char* luacv_checkstring(lua_State *L,int narg,const char *funcparam);
lua_Integer luacv_checkint(lua_State *L,int narg,const char *funcparam);
lua_Number luacv_checknumber(lua_State *L,int narg,const char *funcparam);
void *luacv_checkudata (lua_State *L, int ud, const char *tname,const char *funcparam, int metatable);

int luacv_checkmetaname(char **name,unsigned len);
int luacv_methodSearch(const char *pattern,const struct luacv_method * table);

#define UNIMPL(L)\
  luaL_error(L,"Function %s isn't implemented yet.\n",__FUNCTION__)

#define checkudata(L,narg,metaname)\
  luacv_checkudata(L,narg,metaname,f_msg,1)

#define checkldata(L,narg)\
  luacv_checkudata(L,narg,"",f_msg,0)

#define luacv_tag_error(L,narg,tname,funcparam)\
  luacv_argerror(L, narg, lua_pushfstring(L, "%s expected, got %s",lua_typename(L,tname), luaL_typename(L, narg)),funcparam)

#define checkstring(L,narg)\
  luacv_checkstring(L,narg,f_msg)

#define checkint(L,narg)\
  luacv_checkint(L,narg,f_msg)

#define checknumber(L,narg)\
  luacv_checknumber(L,narg,f_msg)

#define makeObjectCallback(name)\
const luaL_Reg name##_m[]=\
{\
  {"__index",name##__index},\
  {"__newindex",name##__newindex},\
  {"__tostring",name##_tostring},\
  {"__gc",name##_gc},\
  {NULL,NULL}\
}

#define objectReg(name,table)\
  luaL_newmetatable(L,name);\
  lua_pushvalue(L,-1);\
  lua_setfield(L,-2,"__index");\
  luaL_register(L,NULL,table);\
  lua_pop(L,1)

#define methodReg(name,object)\
  {#name,sizeof(#name)-1,object##_n##name,object##_##name}

#define funcReg(name)\
  {#name,luacv_cv##name}

#define varReg(name)\
  {#name,name}

#define imTableReg(name)\
  {#name,name,luacv_TableTo##name,luacv_##name##ToTable}

#define matTableReg(name)\
  {#name,name,luacv_TableTo##name,luacv_##name##ToTable},\
  {#name"C1",name##C1,luacv_TableTo##name,luacv_##name##ToTable},\
  {#name"C2",name##C2,luacv_TableTo##name,luacv_##name##ToTable},\
  {#name"C3",name##C3,luacv_TableTo##name,luacv_##name##ToTable},\
  {#name"C4",name##C4,luacv_TableTo##name,luacv_##name##ToTable}

#define makeIndexFunctions(name)\
static int name##__index(lua_State *L)\
{\
  int ret;\
  if ((ret=luacv_methodSearch(lua_tostring(L,2),name##_v))!=-1)\
    return name##_v[ret].index(L);\
  lua_pushnil(L);\
  return 1;\
}\
static int name##__newindex(lua_State *L)\
{\
  int ret;\
  if ((ret=luacv_methodSearch(lua_tostring(L,2),name##_v))!=-1)\
    return name##_v[ret].newindex(L);\
  return 0;\
}

#define luacv_alloc cvAlloc
#define luacv_free cvFree

template<typename cvtype>
cvtype* luacv_checkObject(lua_State *L,int i,const char *type_name)
{
  return (cvtype*)((luacv_obj<cvtype>*)luaL_checkudata(L,i,type_name))->data;
}

  template<typename cvtype>
void luacv_pushObject(lua_State *L,cvtype *data,const char *type_name,bool copy=false)
{
  if (!data) lua_pushnil(L);
  else
  {
    luacv_obj<cvtype> *obj=(luacv_obj<cvtype>*)lua_newuserdata(L,sizeof(luacv_obj<cvtype>));
    obj->dealloc_data=true;
    if (copy)
    {
      cvtype *d=(cvtype*)luacv_alloc(sizeof(cvtype));
      *d=*data; 
      obj->data=d;
    }
    else
    {
      obj->data=data;
      obj->ref=luaL_ref(L,LUA_REGISTRYINDEX);
      lua_rawgeti(L,LUA_REGISTRYINDEX,obj->ref);
    }

    luaL_getmetatable(L,type_name);
    lua_setmetatable(L,-2);
  }
}

inline void luacv_pushUserData(lua_State *L,void* data)
{
  if (!data) lua_pushnil(L);
  else
  {
    luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
    obj->data=data;
    obj->ref=luaL_ref(L,LUA_REGISTRYINDEX);
    lua_rawgeti(L,LUA_REGISTRYINDEX,obj->ref);
    obj->dealloc_data=false;
  }
}



#define pushUserData(L,data) luacv_pushUserData(L,data)

inline void luacv_releaseObject(lua_State *L,int i)
{
  luaL_unref(L,LUA_REGISTRYINDEX,((luacv_obj<void>*)lua_touserdata(L,i))->ref);
}
#endif
