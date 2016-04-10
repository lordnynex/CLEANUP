#include "CvScalar.h"
static int CvScalar_tostring(lua_State *L)
{
  CvScalar *s=checkCvScalar(L,1);
  lua_pushfstring(L,CVSCALAR_NAME" object %p\n\tval={ [1]=>%f, [2]=>%f, [3]=>%f, [4]=>%f }",s,s->val[0],s->val[1],s->val[2],s->val[3]);
  return 1;
}

static int CvScalar_gc(lua_State *L)
{
  luacv_obj<CvScalar>*obj=(luacv_obj<CvScalar>*)luaL_checkudata(L,1,CVSCALAR_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvScalar_val(lua_State *L)
{
    CvScalar *s=checkCvScalar(L,1);
    lua_newtable(L);
    for(int i=0;i<4;i++)
    {
      lua_pushnumber(L,s->val[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvScalar_nval(lua_State *L)
{
    const char f_msg[]=CVSCALAR_NAME".val={int, int, int, int}";
    CvScalar *s=checkCvScalar(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=4)) luaL_error(L,f_msg);
    for(int i=0;i<4;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->val[i]=checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static const luacv_method CvScalar_v[]=
{
  {NULL,2,NULL,NULL},
  methodReg(val,CvScalar),
};

makeIndexFunctions(CvScalar)
makeObjectCallback(CvScalar);
