#include "CvBGCodeBookElem.h"
static int CvBGCodeBookElem_tostring(lua_State *L)
{
  CvBGCodeBookElem *e=checkCvBGCodeBookElem(L,1);
  lua_pushfstring(L,CVBGCODEBOOKELEM_NAME" object %p\n\t"CVBGCODEBOOKELEM_NAME" next=%p\n\tint tLastUpdate=%d\n\tint stale=%d\n\tboxMin={ [1]=>%d, [2]=>%d, [3]=>%d }\n\tboxMax={ [1]=>%d, [2]=>%d, [3]=>%d }\n\tlearnMin={ [1]=>%d, [2]=>%d, [3]=>%d }\n\tlearnMax={ [1]=>%d, [2]=>%d, [3]=>%d }",e,e->next,e->tLastUpdate,e->stale,e->boxMin[0],e->boxMin[1],e->boxMin[2],e->boxMax[0],e->boxMax[1],e->boxMax[2],e->learnMin[0],e->learnMin[1],e->learnMin[2],e->learnMax[0],e->learnMax[1],e->learnMax[2]);
  return 1;
}

static int CvBGCodeBookElem_gc(lua_State *L)
{
  luacv_obj<CvBGCodeBookElem>*obj=(luacv_obj<CvBGCodeBookElem>*)luaL_checkudata(L,1,CVBGCODEBOOKELEM_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvBGCodeBookElem_boxMin(lua_State *L)
{
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->boxMin[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookElem_nboxMin(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".boxMin={int, int, int}";
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->boxMin[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvBGCodeBookElem_boxMax(lua_State *L)
{
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->boxMax[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookElem_nboxMax(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".boxMax={int, int, int}";
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->boxMax[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}
static int CvBGCodeBookElem_learnMin(lua_State *L)
{
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->learnMin[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookElem_nlearnMin(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".learnMin={int, int, int}";
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->learnMin[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}
static int CvBGCodeBookElem_learnMax(lua_State *L)
{
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->learnMax[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookElem_nlearnMax(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".learnMax={int, int, int}";
    CvBGCodeBookElem *s=checkCvBGCodeBookElem(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->learnMax[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvBGCodeBookElem_tLastUpdate(lua_State *L)
{
    lua_pushnumber(L,checkCvBGCodeBookElem(L,1)->tLastUpdate);
    return 1;
}

static int CvBGCodeBookElem_ntLastUpdate(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".tLastUpdate=int";
    checkCvBGCodeBookElem(L,1)->tLastUpdate=checkint(L,3);
    return 0;
}
static int CvBGCodeBookElem_stale(lua_State *L)
{
    lua_pushnumber(L,checkCvBGCodeBookElem(L,1)->stale);
    return 1;
}

static int CvBGCodeBookElem_nstale(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKELEM_NAME".stale=int";
    checkCvBGCodeBookElem(L,1)->stale=checkint(L,3);
    return 0;
}
static int CvBGCodeBookElem_next(lua_State *L)
{
    pushUserData(L,checkCvBGCodeBookElem(L,1)->next);
    luaL_getmetatable(L,CVBGCODEBOOKELEM_NAME);
    lua_setmetatable(L,-2);
    return 1;
}

static int CvBGCodeBookElem_nnext(lua_State *L)
{
    checkCvBGCodeBookElem(L,1)->next=checkCvBGCodeBookElem(L,3);
    return 0;
}
static const luacv_method CvBGCodeBookElem_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(boxMin,CvBGCodeBookElem),
  methodReg(boxMax,CvBGCodeBookElem),
  methodReg(learnMin,CvBGCodeBookElem),
  methodReg(learnMax,CvBGCodeBookElem),
  methodReg(next,CvBGCodeBookElem),
  methodReg(stale,CvBGCodeBookElem),
  methodReg(tLastUpdate,CvBGCodeBookElem),
};

makeIndexFunctions(CvBGCodeBookElem)
makeObjectCallback(CvBGCodeBookElem);
