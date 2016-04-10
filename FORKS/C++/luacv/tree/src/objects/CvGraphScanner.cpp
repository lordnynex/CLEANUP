#include "CvGraphScanner.h"

static int CvGraphScanner_tostring(lua_State *L)
{
  CvGraphScanner *scan=checkCvGraphScanner(L,1);
  lua_pushfstring(L,CVGRAPHSCANNER_NAME" object %p\n\tvtx="CVGRAPHVTX_NAME"\n\tdst="CVGRAPHVTX_NAME"\n\tedge="CVGRAPHEDGE_NAME"\n\tgraph="CVGRAPH_NAME"\n\tstack="CVSEQ_NAME"\n\tindex=%d\n\tmask=%d",scan,scan->index,scan->mask);
  return 1;
}

static int CvGraphScanner_gc(lua_State *L)
{
  luacv_obj<CvGraphScanner>*obj=(luacv_obj<CvGraphScanner>*)luaL_checkudata(L,1,CVGRAPHSCANNER_NAME);
  if (obj->dealloc_data)
    cvReleaseGraphScanner(&obj->data);
  return 0;
}


static int CvGraphScanner_nvtx(lua_State *L)
{
  checkCvGraphScanner(L,1)->vtx=checkCvGraphVtx(L,3);
  return 0;
}

static int CvGraphScanner_vtx(lua_State *L)
{
  pushCvGraphVtx(L,checkCvGraphScanner(L,1)->vtx);
  return 1;
}


static int CvGraphScanner_ndst(lua_State *L)
{
  checkCvGraphScanner(L,1)->dst=checkCvGraphVtx(L,3);
  return 0;
}

static int CvGraphScanner_dst(lua_State *L)
{
  pushCvGraphVtx(L,checkCvGraphScanner(L,1)->dst);
  return 1;
}

static int CvGraphScanner_nedge(lua_State *L)
{
  checkCvGraphScanner(L,1)->edge=checkCvGraphEdge(L,3);
  return 0;
}

static int CvGraphScanner_edge(lua_State *L)
{
  pushCvGraphEdge(L,checkCvGraphScanner(L,1)->edge);
  return 1;
}

static int CvGraphScanner_nindex(lua_State *L)
{
  const char f_msg[]=CVGRAPHSCANNER_NAME".index=int";
  checkCvGraphScanner(L,1)->index=checkint(L,3);
  return 0;
}

static int CvGraphScanner_index(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphScanner(L,1)->index);
  return 1;
}
static int CvGraphScanner_nmask(lua_State *L)
{
  const char f_msg[]=CVGRAPHSCANNER_NAME".mask=int";
  checkCvGraphScanner(L,1)->mask=checkint(L,3);
  return 0;
}

static int CvGraphScanner_mask(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphScanner(L,1)->mask);
  return 1;
}


static int CvGraphScanner_nstack(lua_State *L)
{
  checkCvGraphScanner(L,1)->stack=checkCvSeq(L,3);
  return 0;
}

static int CvGraphScanner_stack(lua_State *L)
{
  pushCvSeq(L,checkCvGraphScanner(L,1)->stack);
  return 1;
}

static int CvGraphScanner_ngraph(lua_State *L)
{
  checkCvGraphScanner(L,1)->graph=checkCvGraph(L,3);
  return 0;
}

static int CvGraphScanner_graph(lua_State *L)
{
  pushCvGraph(L,checkCvGraphScanner(L,1)->graph);
  return 1;
}

static const luacv_method CvGraphScanner_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(edge,CvGraphScanner),
  methodReg(dst,CvGraphScanner),
  methodReg(graph,CvGraphScanner),
  methodReg(index,CvGraphScanner),
  methodReg(mask,CvGraphScanner),
  methodReg(stack,CvGraphScanner),
  methodReg(vtx,CvGraphScanner),
};

makeIndexFunctions(CvGraphScanner)
makeObjectCallback(CvGraphScanner);

