#include "CvGaussBGPoint.h"
static int CvGaussBGPoint_tostring(lua_State *L)
{
  CvGaussBGPoint *p=checkCvGaussBGPoint(L,1);
  lua_pushfstring(L,CVGAUSSBGPOINT_NAME" object %p\n\t"CVGAUSSBGVALUES_NAME" g_values=%p",p,p->g_values);
  return 1;
}

static int CvGaussBGPoint_gc(lua_State *L)
{
  luacv_obj<CvGaussBGPoint>*obj=(luacv_obj<CvGaussBGPoint>*)luaL_checkudata(L,1,CVGAUSSBGPOINT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvGaussBGPoint_g_values(lua_State *L)
{
    pushCvGaussBGValues(L,checkCvGaussBGPoint(L,1)->g_values);
    return 1;
}

static int CvGaussBGPoint_ng_values(lua_State *L)
{
    checkCvGaussBGPoint(L,1)->g_values=checkCvGaussBGValues(L,3);
    return 0;
}
static const luacv_method CvGaussBGPoint_v[]=
{
  {NULL,2,NULL,NULL},
  methodReg(g_values,CvGaussBGPoint),
};

makeIndexFunctions(CvGaussBGPoint)
makeObjectCallback(CvGaussBGPoint);
