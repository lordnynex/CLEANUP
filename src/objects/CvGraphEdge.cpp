#include "CvGraphEdge.h"
#include "CvGraphVtx.h"

static int CvGraphEdge_tostring(lua_State *L)
{
  CvGraphEdge *edge=checkCvGraphEdge(L,1);
  lua_pushfstring(L,CVGRAPHEDGE_NAME" object %p\n\tflags=%d\n\tweight=%d\n\tnext="CVGRAPHEDGE_NAME"\n\tvtx="CVGRAPHVTX_NAME,edge,edge->flags,edge->weight);

  return 1;
}
static int CvGraphEdge_gc(lua_State *L)
{

  luacv_obj<CvGraphEdge>*obj=(luacv_obj<CvGraphEdge>*)luaL_checkudata(L,1,CVGRAPHEDGE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvGraphEdge_nflags(lua_State *L)
{
  const char f_msg[]=CVGRAPHEDGE_NAME".flags=int";
  checkCvGraphEdge(L,1)->flags=checkint(L,3);
return 0;
}

static int CvGraphEdge_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphEdge(L,1)->flags);
return 1;
}

static int CvGraphEdge_nweight(lua_State *L)
{
  const char f_msg[]=CVGRAPHEDGE_NAME".weight=num";
  checkCvGraphEdge(L,1)->weight=(float)checknumber(L,3);
return 0;
}

static int CvGraphEdge_weight(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphEdge(L,1)->weight);
return 1;
}

static int CvGraphEdge_nnext(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)!=2))luaL_error(L,CVGRAPHEDGE_NAME"->next must be table of 2 elements indexed from 0.");
  CvGraphEdge *edge=checkCvGraphEdge(L,1);
  for(int i=0;i<2;i++)
  {
    lua_rawgeti(L,3,i);
    edge->next[i]=checkCvGraphEdge(L,4);
    lua_pop(L,1);
  }
return 0;
}

static int CvGraphEdge_next(lua_State *L)
{
  CvGraphEdge *edge=checkCvGraphEdge(L,1);
  lua_newtable(L);
  for(int i=0;i<2;i++)
  {
    pushCvGraphEdge(L,edge->next[i]);
    lua_rawseti(L,-2,i);
  }

  return 1;
}
static int CvGraphEdge_nvtx(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)!=2))luaL_error(L,CVGRAPHEDGE_NAME"->vtx must be table of 2 elements indexed from 0.");
  CvGraphEdge *edge=checkCvGraphEdge(L,1);
  for(int i=0;i<2;i++)
  {
    lua_rawgeti(L,3,i);
    edge->vtx[i]=checkCvGraphVtx(L,4);
    lua_pop(L,1);
  }
return 0;
}

static int CvGraphEdge_vtx(lua_State *L)
{
  CvGraphEdge *edge=checkCvGraphEdge(L,1);
  lua_newtable(L);
  for(int i=0;i<2;i++)
  {
    pushCvGraphVtx(L,edge->vtx[i]);
    lua_rawseti(L,-2,i);
  }

  return 1;
}


static const luacv_method CvGraphEdge_v[]=
{
  {NULL,5,NULL,NULL},
  methodReg(flags,CvGraphEdge),
  methodReg(next,CvGraphEdge),
  methodReg(vtx,CvGraphEdge),
  methodReg(weight,CvGraphEdge),
};

makeIndexFunctions(CvGraphEdge)
makeObjectCallback(CvGraphEdge);
