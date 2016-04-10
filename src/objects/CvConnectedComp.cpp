#include "CvConnectedComp.h"

static int CvConnectedComp_tostring(lua_State *L)
{
  CvConnectedComp *c=checkCvConnectedComp(L,1);
  lua_pushfstring(L,CVCONNECTEDCOMP_NAME" object: %p\n\tarea=%d\n\tvalue="CVSCALAR_NAME"\n\trect="CVRECT_NAME"\n\tcontour="CVSEQ_NAME,c,c->area);
  return 1;
}

static int CvConnectedComp_gc(lua_State *L)
{

  luacv_obj<CvConnectedComp>*obj=(luacv_obj<CvConnectedComp>*)luaL_checkudata(L,1,CVCONNECTEDCOMP_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int CvConnectedComp_narea(lua_State *L)
{
  const char f_msg[]=CVCONNECTEDCOMP_NAME".area=num";
  checkCvConnectedComp(L,1)->area=checknumber(L,3);
return 0;
}

static int CvConnectedComp_area(lua_State *L)
{
  lua_pushnumber(L,checkCvConnectedComp(L,1)->area);
return 1;
}

static int CvConnectedComp_nvalue(lua_State *L)
{
  checkCvConnectedComp(L,1)->value=*checkCvScalar(L,3);
return 0;
}

static int CvConnectedComp_value(lua_State *L)
{
  pushCvScalar(L,&checkCvConnectedComp(L,1)->value);
return 1;
}

static int CvConnectedComp_nrect(lua_State *L)
{
  checkCvConnectedComp(L,1)->rect=*checkCvRect(L,3);
return 0;
}

static int CvConnectedComp_rect(lua_State *L)
{
  pushCvRect(L,&checkCvConnectedComp(L,1)->rect);
return 1;
}

static int CvConnectedComp_ncontour(lua_State *L)
{
  checkCvConnectedComp(L,1)->contour=checkCvSeq(L,3);
return 0;
}

static int CvConnectedComp_contour(lua_State *L)
{
  pushCvSeq(L,checkCvConnectedComp(L,1)->contour);
return 1;
}

static const luacv_method CvConnectedComp_v[]=
{
  {NULL,5,NULL,NULL},
  methodReg(area,CvConnectedComp),
  methodReg(contour,CvConnectedComp),
  methodReg(rect,CvConnectedComp),
  methodReg(value,CvConnectedComp),

};

makeIndexFunctions(CvConnectedComp)
makeObjectCallback(CvConnectedComp);
