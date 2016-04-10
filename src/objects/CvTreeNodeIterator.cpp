#include "CvTreeNodeIterator.h"

static int CvTreeNodeIterator_tostring(lua_State *L)
{
  CvTreeNodeIterator *iter=checkCvTreeNodeIterator(L,1);
  lua_pushfstring(L,CVTREENODEITERATOR_NAME" object %p\n\tnode=%p\n\tlevel=%d\n\tmax_level=%d",iter,iter->node,iter->level,iter->max_level);

  return 1;
}
static int CvTreeNodeIterator_gc(lua_State *L)
{
  luacv_obj<CvTreeNodeIterator>*obj=(luacv_obj<CvTreeNodeIterator>*)luaL_checkudata(L,1,CVTREENODEITERATOR_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvTreeNodeIterator_node(lua_State *L)
{
  pushUserData(L,(void*)checkCvTreeNodeIterator(L,1)->node);
  return 1;
}

static int CvTreeNodeIterator_level(lua_State *L)
{
  lua_pushnumber(L,checkCvTreeNodeIterator(L,1)->level);
  return 1;
}

static int CvTreeNodeIterator_max_level(lua_State *L)
{
  lua_pushnumber(L,checkCvTreeNodeIterator(L,1)->max_level);
  return 1;
 }

static int CvTreeNodeIterator_nlevel(lua_State *L)
{
  const char f_msg[]=CVTREENODEITERATOR_NAME".level=int";
  checkCvTreeNodeIterator(L,1)->level=checkint(L,3);
  return 0;
}

static int CvTreeNodeIterator_nmax_level(lua_State *L)
{
  const char f_msg[]=CVTREENODEITERATOR_NAME".max_level=int";
  checkCvTreeNodeIterator(L,1)->max_level=checkint(L,3);
  return 0;
}

static int CvTreeNodeIterator_nnode(lua_State *L)
{
  const char f_msg[]=CVTREENODEITERATOR_NAME".node=userdata";
  checkCvTreeNodeIterator(L,1)->node=checkldata(L,3);
  return 0;   
}

static const luacv_method CvTreeNodeIterator_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(level,CvTreeNodeIterator),
  methodReg(node,CvTreeNodeIterator),
  methodReg(max_level,CvTreeNodeIterator)
};

makeIndexFunctions(CvTreeNodeIterator)
makeObjectCallback(CvTreeNodeIterator);
