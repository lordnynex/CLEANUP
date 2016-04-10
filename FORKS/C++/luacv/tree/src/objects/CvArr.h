#ifndef CVARR_NAME
#include "opencv2/core/types_c.h"
#include "luacvaux.h"

#define CVARR_NAME "CvArr"
inline CvArr* checkCvArr(lua_State *L,int i)
{
  if (lua_isuserdata(L,i))
    return (CvArr*)((luacv_obj<CvArr>*)lua_touserdata(L,i))->data;

  if (!lua_isnil(L,i))
    luaL_error(L,"Object isn't usedata nor nil.");

  return NULL;
}

#define pushCvArr(L,data) pushUserData(L,data)
#endif
