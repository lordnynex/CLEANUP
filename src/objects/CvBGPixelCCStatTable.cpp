#include "CvBGPixelCCStatTable.h"
static int CvBGPixelCCStatTable_tostring(lua_State *L)
{
  CvBGPixelCCStatTable *s=checkCvBGPixelCCStatTable(L,1);
  lua_pushfstring(L,CVBGPIXELCCSTATTABLE_NAME" object %p\n\tnum Pv=%f\n\tnum Pvb=%f\n\tv={ [1]=>%d, [2]=>%d, [3]=>%d, [4]=>%d, [5]=>%d, [6]=>%d}",s,s->Pv,s->Pvb,s->v[0],s->v[1],s->v[2],s->v[3],s->v[4],s->v[5]);
  return 1;
}

static int CvBGPixelCCStatTable_gc(lua_State *L)
{
  luacv_obj<CvBGPixelCCStatTable>*obj=(luacv_obj<CvBGPixelCCStatTable>*)luaL_checkudata(L,1,CVBGPIXELCCSTATTABLE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvBGPixelCCStatTable_nPv(lua_State *L)
{
  const char f_msg[]=CVBGPIXELCCSTATTABLE_NAME".Pv=num";
  checkCvBGPixelCCStatTable(L,1)->Pv=checkint(L,3);
return 0;
}

static int CvBGPixelCCStatTable_Pv(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelCCStatTable(L,1)->Pv);
return 1;
}

static int CvBGPixelCCStatTable_nPvb(lua_State *L)
{
  const char f_msg[]=CVBGPIXELCCSTATTABLE_NAME".Pvb=num";
  checkCvBGPixelCCStatTable(L,1)->Pvb=checkint(L,3);
return 0;
}

static int CvBGPixelCCStatTable_Pvb(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelCCStatTable(L,1)->Pvb);
return 1;
}

static int CvBGPixelCCStatTable_va(lua_State *L)
{
    CvBGPixelCCStatTable *s=checkCvBGPixelCCStatTable(L,1);
    lua_newtable(L);
    for(int i=0;i<6;i++)
    {
      lua_pushnumber(L,s->v[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGPixelCCStatTable_nva(lua_State *L)
{
    const char f_msg[]=CVBGPIXELCCSTATTABLE_NAME".v={int, int, int}";
    CvBGPixelCCStatTable *s=checkCvBGPixelCCStatTable(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=6)) luaL_error(L,f_msg);
    for(int i=0;i<6;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->v[i]=(char)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static const luacv_method CvBGPixelCCStatTable_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(Pv,CvBGPixelCCStatTable),
  methodReg(Pvb,CvBGPixelCCStatTable),
  methodReg(va,CvBGPixelCCStatTable),

};

makeIndexFunctions(CvBGPixelCCStatTable)
makeObjectCallback(CvBGPixelCCStatTable);
