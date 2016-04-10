#include "CvQuadEdge2D.h"

static int CvQuadEdge2D_tostring(lua_State *L)
{
  CvQuadEdge2D *edge=checkCvQuadEdge2D(L,1);
  lua_pushfstring(L,CVQUADEDGE2D_NAME" object: %p\n\tflags=%d\n\tpt[4]="CVSUBDIV2DPOINT_NAME"\n\tnext[4]=userdata",edge,edge->flags);
  return 1;
}

static int CvQuadEdge2D_gc(lua_State *L)
{
  luacv_obj<CvQuadEdge2D>*obj=(luacv_obj<CvQuadEdge2D>*)luaL_checkudata(L,1,CVQUADEDGE2D_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvQuadEdge2D_nflags(lua_State *L)
{
  const char f_msg[]=CVQUADEDGE2D_NAME".flags=int";
  checkCvQuadEdge2D(L,1)->flags=checkint(L,3);
return 0;
}

static int CvQuadEdge2D_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvQuadEdge2D(L,1)->flags);
return 1;
}

static int CvQuadEdge2D_npt(lua_State *L)
{
    const char f_msg[]=CVQUADEDGE2D_NAME".pt={"CVSUBDIV2DPOINT_NAME", "CVSUBDIV2DPOINT_NAME", "CVSUBDIV2DPOINT_NAME", "CVSUBDIV2DPOINT_NAME"}";
    CvQuadEdge2D *edge=checkCvQuadEdge2D(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=4)) luaL_error(L,f_msg);
    for(int i=0;i<4;i++) 
    {
      lua_rawgeti(L,3,i+1);
      edge->pt[i]=checkCvSubdiv2DPoint(L,4);
      lua_pop(L,1);
    }

return 0;
}

static int CvQuadEdge2D_pt(lua_State *L)
{
  CvQuadEdge2D *edge=checkCvQuadEdge2D(L,1);
  lua_newtable(L);
  for(int i=0;i<4;i++)
  {
    pushUserData(L,edge->pt[i]);
    luaL_getmetatable(L,CVSUBDIV2DPOINT_NAME);
    lua_setmetatable(L,-2);
    lua_rawseti(L,3,i+1);
  }
  return 1;
}


static int CvQuadEdge2D_nnext(lua_State *L)
{
    const char f_msg[]=CVQUADEDGE2D_NAME".next={ userdata, userdata, userdata, userdata}";
    CvQuadEdge2D *edge=checkCvQuadEdge2D(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=4)) luaL_error(L,f_msg);
    for(int i=0;i<4;i++) 
    {
      lua_rawgeti(L,3,i+1);
      edge->next[i]=(size_t)checkldata(L,4);
      lua_pop(L,1);
    }

return 0;
}

static int CvQuadEdge2D_next(lua_State *L)
{
  CvQuadEdge2D *edge=checkCvQuadEdge2D(L,1);
  lua_newtable(L);
  for(int i=0;i<4;i++)
  {
    pushUserData(L,(void*)edge->next[i]);
    lua_rawseti(L,3,i+1);
  }
  return 1;
}


static const luacv_method CvQuadEdge2D_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(flags,CvQuadEdge2D),
  methodReg(next,CvQuadEdge2D),
  methodReg(pt,CvQuadEdge2D)
};

makeIndexFunctions(CvQuadEdge2D)
makeObjectCallback(CvQuadEdge2D);
