#include "CvSubdiv2DPoint.h"

static int CvSubdiv2DPoint_tostring(lua_State *L)
{
  CvSubdiv2DPoint *p=checkCvSubdiv2DPoint(L,1);
  lua_pushfstring(L,CVSUBDIV2DPOINT_NAME" object: %p\n\tflags=%d\n\tfirst=userdata\n\tpt="CVPOINT2D32F_NAME"\n\tid=%d",p,p->flags,p->id);
  return 1;
}

static int CvSubdiv2DPoint_gc(lua_State *L)
{
  luacv_obj<CvSubdiv2DPoint>*obj=(luacv_obj<CvSubdiv2DPoint>*)luaL_checkudata(L,1,CVSUBDIV2DPOINT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvSubdiv2DPoint_nflags(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME".flags=int";
  checkCvSubdiv2DPoint(L,1)->flags=checkint(L,3);
return 0;
}

static int CvSubdiv2DPoint_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2DPoint(L,1)->flags);
return 1;
}

static int CvSubdiv2DPoint_nid(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME".id=int";
  checkCvSubdiv2DPoint(L,1)->id=checkint(L,3);
return 0;
}

static int CvSubdiv2DPoint_id(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2DPoint(L,1)->id);
return 1;
}

static int CvSubdiv2DPoint_npt(lua_State *L)
{
  //const char f_msg[]=CVSUBDIV2DPOINT_NAME".pt="CVPOINT2D32F_NAME;
  checkCvSubdiv2DPoint(L,1)->pt=*checkCvPoint2D32f(L,3);
return 0;
}

static int CvSubdiv2DPoint_pt(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCvSubdiv2DPoint(L,1)->pt);
return 1;
}


static int CvSubdiv2DPoint_nfirst(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME".first=userdata";
  checkCvSubdiv2DPoint(L,1)->first=(size_t)checkldata(L,3);
return 0;
}

static int CvSubdiv2DPoint_first(lua_State *L)
{
  pushUserData(L,(void*)checkCvSubdiv2DPoint(L,1)->first);
return 1;
}



static const luacv_method CvSubdiv2DPoint_v[]=
{
  {NULL,5,NULL,NULL},
  methodReg(flags,CvSubdiv2DPoint),
  methodReg(first,CvSubdiv2DPoint),
  methodReg(id,CvSubdiv2DPoint),
  methodReg(pt,CvSubdiv2DPoint),
};

makeIndexFunctions(CvSubdiv2DPoint)
makeObjectCallback(CvSubdiv2DPoint);
