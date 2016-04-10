#include "CvBGPixelCStatTable.h"
static int CvBGPixelCStatTable_tostring(lua_State *L)
{
  CvBGPixelCStatTable *s=checkCvBGPixelCStatTable(L,1);
  lua_pushfstring(L,CVBGPIXELCSTATTABLE_NAME" object %p\n\tnum Pv=%f\n\tnum Pvb=%f\n\tv={ [1]=>%d, [2]=>%d, [3]=>%d}",s,s->Pv,s->Pvb,s->v[0],s->v[1],s->v[2]);
  return 1;
}

static int CvBGPixelCStatTable_gc(lua_State *L)
{
  luacv_obj<CvBGPixelCStatTable>*obj=(luacv_obj<CvBGPixelCStatTable>*)luaL_checkudata(L,1,CVBGPIXELCSTATTABLE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvBGPixelCStatTable_nPv(lua_State *L)
{
  const char f_msg[]=CVBGPIXELCSTATTABLE_NAME".Pv=num";
  checkCvBGPixelCStatTable(L,1)->Pv=checkint(L,3);
return 0;
}

static int CvBGPixelCStatTable_Pv(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelCStatTable(L,1)->Pv);
return 1;
}

static int CvBGPixelCStatTable_nPvb(lua_State *L)
{
  const char f_msg[]=CVBGPIXELCSTATTABLE_NAME".Pvb=num";
  checkCvBGPixelCStatTable(L,1)->Pvb=checkint(L,3);
return 0;
}

static int CvBGPixelCStatTable_Pvb(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelCStatTable(L,1)->Pvb);
return 1;
}

static int CvBGPixelCStatTable_va(lua_State *L)
{
    CvBGPixelCStatTable *s=checkCvBGPixelCStatTable(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->v[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGPixelCStatTable_nva(lua_State *L)
{
    const char f_msg[]=CVBGPIXELCSTATTABLE_NAME".v={int, int, int}";
    CvBGPixelCStatTable *s=checkCvBGPixelCStatTable(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->v[i]=(char)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static const luacv_method CvBGPixelCStatTable_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(Pv,CvBGPixelCStatTable),
  methodReg(Pvb,CvBGPixelCStatTable),
  methodReg(va,CvBGPixelCStatTable),

};

makeIndexFunctions(CvBGPixelCStatTable)
makeObjectCallback(CvBGPixelCStatTable);
