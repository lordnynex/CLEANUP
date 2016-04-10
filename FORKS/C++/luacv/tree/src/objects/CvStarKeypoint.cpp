#include "CvStarKeypoint.h"

static int CvStarKeypoint_tostring(lua_State *L)
{
  CvStarKeypoint *p=checkCvStarKeypoint(L,1);
  lua_pushfstring(L,CVSTARKEYPOINT_NAME" object: %p\n\t"CVPOINT_NAME" pt{ x=>%d, y=%d }\n\tint size=%d\n\tnum response=%f",p,p->pt.x,p->pt.y,p->size,p->response);
  return 1;
}

static int CvStarKeypoint_gc(lua_State *L)
{
  luacv_obj<CvStarKeypoint>*obj=(luacv_obj<CvStarKeypoint>*)luaL_checkudata(L,1,CVSTARKEYPOINT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvStarKeypoint_nsize(lua_State *L)
{
  const char f_msg[]=CVSTARKEYPOINT_NAME".size=int";
  checkCvStarKeypoint(L,1)->size=checkint(L,3);
return 0;
}

static int CvStarKeypoint_size(lua_State *L)
{
  lua_pushnumber(L,checkCvStarKeypoint(L,1)->size);
return 1;
}


static int CvStarKeypoint_nresponse(lua_State *L)
{
  const char f_msg[]=CVSTARKEYPOINT_NAME".response=num";
  checkCvStarKeypoint(L,1)->response=(float)checknumber(L,3);
return 0;
}

static int CvStarKeypoint_response(lua_State *L)
{
  lua_pushnumber(L,checkCvStarKeypoint(L,1)->response);
return 1;
}


static int CvStarKeypoint_npt(lua_State *L)
{
  checkCvStarKeypoint(L,1)->pt=*checkCvPoint(L,3);
return 0;
}

static int CvStarKeypoint_pt(lua_State *L)
{
  pushCvPoint(L,&checkCvStarKeypoint(L,1)->pt);
return 1;
}



static const luacv_method CvStarKeypoint_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(pt,CvStarKeypoint),
  methodReg(response,CvStarKeypoint),
  methodReg(size,CvStarKeypoint),
};

makeIndexFunctions(CvStarKeypoint)
makeObjectCallback(CvStarKeypoint);
