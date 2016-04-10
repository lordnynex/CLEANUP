#include "CvSURFPoint.h"

static int CvSURFPoint_tostring(lua_State *L)
{
  CvSURFPoint *point=checkCvSURFPoint(L,1);
  lua_pushfstring(L,CVSURFPOINT_NAME" object %p\n\t"CVPOINT2D32F_NAME" pt={ [x]=>%f, [y]=>%f }\n\tint laplacian=%d\n\tint size=%d\n\tnum dir=%f\n\tnum hessian=%f",point,point->pt.x,point->pt.y,point->laplacian,point->size,point->dir,point->hessian);
  return 1;
}

static int CvSURFPoint_gc(lua_State *L)
{
  luacv_obj<CvSURFPoint>*obj=(luacv_obj<CvSURFPoint>*)luaL_checkudata(L,1,CVSURFPOINT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSURFPoint_ndir(lua_State *L)
{
  const char f_msg[]=CVSURFPOINT_NAME".dir=num";
  checkCvSURFPoint(L,1)->dir=(float)checknumber(L,3);
return 0;
}

static int CvSURFPoint_dir(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFPoint(L,1)->dir);
return 1;
}

static int CvSURFPoint_nhessian(lua_State *L)
{
  const char f_msg[]=CVSURFPOINT_NAME".hessian=num";
  checkCvSURFPoint(L,1)->hessian=(float)checknumber(L,3);
return 0;
}

static int CvSURFPoint_hessian(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFPoint(L,1)->hessian);
return 1;
}


static int CvSURFPoint_npt(lua_State *L)
{
  checkCvSURFPoint(L,1)->pt=*checkCvPoint2D32f(L,3);
return 0;
}

static int CvSURFPoint_pt(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCvSURFPoint(L,1)->pt);
return 1;
}

static int CvSURFPoint_nlaplacian(lua_State *L)
{
  const char f_msg[]=CVSURFPOINT_NAME".laplacian=int";
  checkCvSURFPoint(L,1)->laplacian=checkint(L,3);
return 0;
}

static int CvSURFPoint_laplacian(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFPoint(L,1)->laplacian);
return 1;
}

static int CvSURFPoint_nsize(lua_State *L)
{
  const char f_msg[]=CVSURFPOINT_NAME".size=int";
  checkCvSURFPoint(L,1)->size=checkint(L,3);
return 0;
}

static int CvSURFPoint_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSURFPoint(L,1)->size);
return 1;
}

static const luacv_method CvSURFPoint_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(dir,CvSURFPoint),
  methodReg(hessian,CvSURFPoint),
  methodReg(laplacian,CvSURFPoint),
  methodReg(pt,CvSURFPoint),
  methodReg(size,CvSURFPoint),
};

makeIndexFunctions(CvSURFPoint)
makeObjectCallback(CvSURFPoint);

