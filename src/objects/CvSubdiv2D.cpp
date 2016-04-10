#include "CvSubdiv2D.h"

static int CvSubdiv2D_tostring(lua_State *L)
{
  CvSubdiv2D *seq=checkCvSubdiv2D(L,1);
  lua_pushfstring(L,CVSUBDIV2D_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\tfree_elems="CVSETELEM_NAME"\n\tactive_count=%d\n\tedges="CVSET_NAME"\n\tquad_edges=%d\n\tis_geometry_valid=%d\n\trecent_edge=userdata\n\ttopleft="CVPOINT2D32F_NAME"\n\tbottomright="CVPOINT2D32F_NAME,seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems,seq->active_count,seq->quad_edges,seq->is_geometry_valid);
  return 1;
}

static int CvSubdiv2D_gc(lua_State *L)
{

  luacv_obj<CvSubdiv2D>*obj=(luacv_obj<CvSubdiv2D>*)luaL_checkudata(L,1,CVSUBDIV2D_NAME);
  if (obj->dealloc_data)
    cvClearGraph((CvGraph*)obj->data);
  return 0;
}


static int CvSubdiv2D_nedges(lua_State *L)
{
  checkCvSubdiv2D(L,1)->edges=(CvSet*)checkCvSeq(L,3);
  return 0;
}

static int CvSubdiv2D_edges(lua_State *L)
{
  pushCvSeq(L,(CvSeq*)checkCvSubdiv2D(L,1)->edges);
  return 1;
}
static int CvSubdiv2D_nfree_elems(lua_State *L)
{
  checkCvSubdiv2D(L,1)->free_elems=checkCvSetElem(L,3);
  return 0;
}

static int CvSubdiv2D_free_elems(lua_State *L)
{
  pushCvSetElem(L,checkCvSubdiv2D(L,1)->free_elems);
  return 1;
}
static int CvSubdiv2D_nactive_count(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".active_count=int";
  checkCvSubdiv2D(L,1)->active_count=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_active_count(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->active_count);
  return 1;
}


static int CvSubdiv2D_nblock_max(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".block_max=userdata";
  checkCvSubdiv2D(L,1)->block_max=(schar*)checkldata(L,3);
  return 0;
}

static int CvSubdiv2D_block_max(lua_State *L)
{
  pushUserData(L,(void*)checkCvSubdiv2D(L,1)->block_max);
  return 1;
}

static int CvSubdiv2D_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".delta_elems=int";
  checkCvSubdiv2D(L,1)->delta_elems=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_delta_elems(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->delta_elems);
  return 1;
}

static int CvSubdiv2D_nelem_size(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".elem_size=int";
  checkCvSubdiv2D(L,1)->elem_size=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_elem_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->elem_size);
  return 1;
}

static int CvSubdiv2D_nflags(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".flags=int";
  checkCvSubdiv2D(L,1)->flags=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->flags);
  return 1;
}

static int CvSubdiv2D_nfree_blocks(lua_State *L)
{
  checkCvSubdiv2D(L,1)->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvSubdiv2D_free_blocks(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSubdiv2D(L,1)->free_blocks);
  return 1;
}

static int CvSubdiv2D_nfirst(lua_State *L)
{
  checkCvSubdiv2D(L,1)->first=checkCvSeqBlock(L,3);
  return 0;
}

static int CvSubdiv2D_first(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSubdiv2D(L,1)->first);
  return 1;
}

static int CvSubdiv2D_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".header_size=int";
  checkCvSubdiv2D(L,1)->header_size=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->header_size);
  return 1;
}

static int CvSubdiv2D_nh_prev(lua_State *L)
{
  checkCvSubdiv2D(L,1)->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvSubdiv2D_h_prev(lua_State *L)
{
  pushCvSeq(L,checkCvSubdiv2D(L,1)->h_prev);
  return 1;
}

static int CvSubdiv2D_nh_next(lua_State *L)
{
  checkCvSubdiv2D(L,1)->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvSubdiv2D_h_next(lua_State *L)
{
  pushCvSeq(L,checkCvSubdiv2D(L,1)->h_next);
  return 1;
}

static int CvSubdiv2D_nptr(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".ptr=userdata";
  checkCvSubdiv2D(L,1)->ptr=(schar*)checkldata(L,3);
  return 0;
}

static int CvSubdiv2D_ptr(lua_State *L)
{
  pushUserData(L,checkCvSubdiv2D(L,1)->ptr);
  return 1;
}


static int CvSubdiv2D_nstorage(lua_State *L)
{
  checkCvSubdiv2D(L,1)->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvSubdiv2D_storage(lua_State *L)
{
  pushCvMemStorage(L,checkCvSubdiv2D(L,1)->storage);
  return 1;
}

static int CvSubdiv2D_ntotal(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".total=int";
  checkCvSubdiv2D(L,1)->total=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_total(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->total);
  return 1;
}

static int CvSubdiv2D_nv_prev(lua_State *L)
{
  checkCvSubdiv2D(L,1)->v_prev=checkCvSeq(L,3);
  return 0;
}

static int CvSubdiv2D_v_prev(lua_State *L)
{
  pushCvSeq(L,checkCvSubdiv2D(L,1)->v_prev);
  return 1;
}

static int CvSubdiv2D_nv_next(lua_State *L)
{
  checkCvSubdiv2D(L,1)->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvSubdiv2D_v_next(lua_State *L)
{
  pushCvSeq(L,checkCvSubdiv2D(L,1)->v_next);
  return 1;
}

static int CvSubdiv2D_nquad_edges(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".quad_edges=int";
  checkCvSubdiv2D(L,1)->quad_edges=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_quad_edges(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->quad_edges);
  return 1;
}

static int CvSubdiv2D_nis_geometry_valid(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".is_geometry_valid=int";
  checkCvSubdiv2D(L,1)->is_geometry_valid=checkint(L,3);
  return 0;
}

static int CvSubdiv2D_is_geometry_valid(lua_State *L)
{
  lua_pushnumber(L,checkCvSubdiv2D(L,1)->is_geometry_valid);
  return 1;
}


static int CvSubdiv2D_ntopleft(lua_State *L)
{
  checkCvSubdiv2D(L,1)->topleft=*checkCvPoint2D32f(L,3);
  return 0;
}

static int CvSubdiv2D_topleft(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCvSubdiv2D(L,1)->topleft);
  return 1;
}

static int CvSubdiv2D_nbottomright(lua_State *L)
{
  checkCvSubdiv2D(L,1)->bottomright=*checkCvPoint2D32f(L,3);
  return 0;
}

static int CvSubdiv2D_bottomright(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCvSubdiv2D(L,1)->bottomright);
  return 1;
}

static int CvSubdiv2D_nrecent_edge(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME".recent_edge=userdata";
  checkCvSubdiv2D(L,1)->recent_edge=(size_t)checkldata(L,3);
  return 0;
}

static int CvSubdiv2D_recent_edge(lua_State *L)
{
  pushUserData(L,(void*)checkCvSubdiv2D(L,1)->recent_edge);
  return 1;
}

static const luacv_method CvSubdiv2D_v[]=
{
  {NULL,23,NULL,NULL},
  methodReg(active_count,CvSubdiv2D),
  methodReg(block_max,CvSubdiv2D),
  methodReg(bottomright,CvSubdiv2D),
  methodReg(delta_elems,CvSubdiv2D),
  methodReg(edges,CvSubdiv2D),
  methodReg(elem_size,CvSubdiv2D),
  methodReg(first,CvSubdiv2D),
  methodReg(flags,CvSubdiv2D),
  methodReg(free_blocks,CvSubdiv2D),
  methodReg(free_elems,CvSubdiv2D),
  methodReg(header_size,CvSubdiv2D),
  methodReg(h_prev,CvSubdiv2D),
  methodReg(h_next,CvSubdiv2D),
  methodReg(is_geometry_valid,CvSubdiv2D),
  methodReg(ptr,CvSubdiv2D),
  methodReg(quad_edges,CvSubdiv2D),
  methodReg(storage,CvSubdiv2D),
  methodReg(recent_edge,CvSubdiv2D),
  methodReg(total,CvSubdiv2D),
  methodReg(topleft,CvSubdiv2D),
  methodReg(v_prev,CvSubdiv2D),
  methodReg(v_next,CvSubdiv2D)
};

makeIndexFunctions(CvSubdiv2D)
makeObjectCallback(CvSubdiv2D);

