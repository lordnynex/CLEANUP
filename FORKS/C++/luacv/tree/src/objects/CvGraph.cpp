#include "CvGraph.h"

static int CvGraph_tostring(lua_State *L)
{
  CvGraph *seq=checkCvGraph(L,1);
  lua_pushfstring(L,CVGRAPH_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\tfree_elems="CVSETELEM_NAME"\n\tactive_count=%d\n\tedges="CVSET_NAME,seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems,seq->active_count);
  return 1;
}

static int CvGraph_gc(lua_State *L)
{

  luacv_obj<CvGraph>*obj=(luacv_obj<CvGraph>*)luaL_checkudata(L,1,CVGRAPH_NAME);
  if (obj->dealloc_data)
    cvClearGraph(obj->data);
  return 0;
}


static int CvGraph_nedges(lua_State *L)
{
  checkCvGraph(L,1)->edges=checkCvSet(L,3);
  return 0;
}

static int CvGraph_edges(lua_State *L)
{
  pushCvSet(L,checkCvGraph(L,1)->edges);
  return 1;
}
static int CvGraph_nfree_elems(lua_State *L)
{
  checkCvGraph(L,1)->free_elems=checkCvSetElem(L,3);
  return 0;
}

static int CvGraph_free_elems(lua_State *L)
{
  pushCvSetElem(L,checkCvGraph(L,1)->free_elems);
  return 1;
}
static int CvGraph_nactive_count(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".active_count=int";
  checkCvGraph(L,1)->active_count=checkint(L,3);
  return 0;
}

static int CvGraph_active_count(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->active_count);
  return 1;
}


static int CvGraph_nblock_max(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".block_max=userdata";
  checkCvGraph(L,1)->block_max=(schar*)checkldata(L,3);
  return 0;
}

static int CvGraph_block_max(lua_State *L)
{
  pushUserData(L,checkCvGraph(L,1)->block_max);
  return 1;
}

static int CvGraph_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".delta_elems=int";
  checkCvGraph(L,1)->delta_elems=checkint(L,3);
  return 0;
}

static int CvGraph_delta_elems(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->delta_elems);
  return 1;
}

static int CvGraph_nelem_size(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".elem_size=int";
  checkCvGraph(L,1)->elem_size=checkint(L,3);
  return 0;
}

static int CvGraph_elem_size(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->elem_size);
  return 1;
}

static int CvGraph_nflags(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".flags=int";
  checkCvGraph(L,1)->flags=checkint(L,3);
  return 0;
}

static int CvGraph_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->flags);
  return 1;
}

static int CvGraph_nfree_blocks(lua_State *L)
{
  checkCvGraph(L,1)->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvGraph_free_blocks(lua_State *L)
{
  pushCvSeqBlock(L,checkCvGraph(L,1)->free_blocks);
  return 1;
}

static int CvGraph_nfirst(lua_State *L)
{
  checkCvGraph(L,1)->first=checkCvSeqBlock(L,3);
  return 0;
}

static int CvGraph_first(lua_State *L)
{
  pushCvSeqBlock(L,checkCvGraph(L,1)->first);
  return 1;
}

static int CvGraph_nheader_size(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".header_size=int";
  checkCvGraph(L,1)->header_size=checkint(L,3);
  return 0;
}

static int CvGraph_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->header_size);
  return 1;
}

static int CvGraph_nh_prev(lua_State *L)
{
  checkCvGraph(L,1)->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvGraph_h_prev(lua_State *L)
{
  pushCvSeq(L,checkCvGraph(L,1)->h_prev);
  return 1;
}

static int CvGraph_nh_next(lua_State *L)
{
  checkCvGraph(L,1)->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvGraph_h_next(lua_State *L)
{
  pushCvSeq(L,checkCvGraph(L,1)->h_next);
  return 1;
}

static int CvGraph_nptr(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".ptr=userdata";
  checkCvGraph(L,1)->ptr=(schar*)checkldata(L,3);
  return 0;
}

static int CvGraph_ptr(lua_State *L)
{
  pushUserData(L,checkCvGraph(L,1)->ptr);
  return 1;
}


static int CvGraph_nstorage(lua_State *L)
{
  checkCvGraph(L,1)->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvGraph_storage(lua_State *L)
{
  pushCvMemStorage(L,checkCvGraph(L,1)->storage);
  return 1;
}

static int CvGraph_ntotal(lua_State *L)
{
  const char f_msg[]=CVGRAPH_NAME".total=int";
  checkCvGraph(L,1)->total=checkint(L,3);
  return 0;
}

static int CvGraph_total(lua_State *L)
{
  lua_pushnumber(L,checkCvGraph(L,1)->total);
  return 1;
}

static int CvGraph_nv_prev(lua_State *L)
{
  checkCvGraph(L,1)->v_prev=checkCvSeq(L,3);
  return 0;
}

static int CvGraph_v_prev(lua_State *L)
{
  pushCvSeq(L,checkCvGraph(L,1)->v_prev);
  return 1;
}

static int CvGraph_nv_next(lua_State *L)
{
  checkCvGraph(L,1)->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvGraph_v_next(lua_State *L)
{
  pushCvSeq(L,checkCvGraph(L,1)->v_next);
  return 1;
}



static const luacv_method CvGraph_v[]=
{
  {NULL,18,NULL,NULL},
  methodReg(active_count,CvGraph),
  methodReg(block_max,CvGraph),
  methodReg(delta_elems,CvGraph),
  methodReg(edges,CvGraph),
  methodReg(elem_size,CvGraph),
  methodReg(first,CvGraph),
  methodReg(flags,CvGraph),
  methodReg(free_blocks,CvGraph),
  methodReg(free_elems,CvGraph),
  methodReg(header_size,CvGraph),
  methodReg(h_prev,CvGraph),
  methodReg(h_next,CvGraph),
  methodReg(ptr,CvGraph),
  methodReg(storage,CvGraph),
  methodReg(total,CvGraph),
  methodReg(v_prev,CvGraph),
  methodReg(v_next,CvGraph),
};

makeIndexFunctions(CvGraph)
makeObjectCallback(CvGraph);

