#include "CvContourTree.h"

static int CvContourTree_tostring(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  lua_pushfstring(L,CVCONTOURTREE_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\t"CVPOINT_NAME" p1={ [x]=>%d, [y]=>%d }\n\t"CVPOINT_NAME" p2={ [x]=>%d, [y]=>%d }",seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems,seq->p1.x,seq->p1.y,seq->p2.x,seq->p2.y);
  return 1;
}

static int CvContourTree_gc(lua_State *L)
{

  luacv_obj<CvContourTree>*obj=(luacv_obj<CvContourTree>*)luaL_checkudata(L,1,CVCONTOURTREE_NAME);
  if (obj->dealloc_data)
    cvClearSeq((CvSeq*)obj->data);

  return 0;
}
static int CvContourTree_flags(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  lua_pushnumber(L,seq->flags);
  return 1;
}

static int CvContourTree_nflags(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".flags=int";
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->flags=checkint(L,3);
  return 0;
}

static int CvContourTree_header_size(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  lua_pushnumber(L,seq->header_size);
  return 1;
}

static int CvContourTree_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".header_size=int";
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->header_size=checkint(L,3);
  return 0;
}

static int CvContourTree_total(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  lua_pushnumber(L,seq->total);
  return 1;
}

static int CvContourTree_ntotal(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".total=int";
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->total=checkint(L,3);
  return 0;
}

static int CvContourTree_elem_size(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  lua_pushnumber(L,seq->elem_size);
  return 1;
}

static int CvContourTree_nelem_size(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".elem_size=int";
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->elem_size=checkint(L,3);
  return 0;
}

static int CvContourTree_delta_elems(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  lua_pushnumber(L,seq->delta_elems);
  return 1;
}

static int CvContourTree_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".delta_elems=int";
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->delta_elems=checkint(L,3);
  return 0;
}

static int CvContourTree_h_prev(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvSeq(L,seq->h_prev);
  return 1;
}

static int CvContourTree_nh_prev(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".h_prev="CVSEQ_NAME;
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvContourTree_h_next(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvSeq(L,seq->h_next);
  return 1;
}

static int CvContourTree_nh_next(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".h_next="CVSEQ_NAME;
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvContourTree_v_prev(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvSeq(L,seq->v_prev);
  return 1;
}

static int CvContourTree_nv_prev(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".v_prev="CVSEQ_NAME;
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->v_prev=checkCvSeq(L,3);
  return 0;
}
static int CvContourTree_v_next(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvSeq(L,seq->v_next);
  return 1;
}

static int CvContourTree_nv_next(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".v_next="CVSEQ_NAME;
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvContourTree_storage(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvMemStorage(L,seq->storage);
  return 1;
}

static int CvContourTree_nstorage(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".storage="CVMEMSTORAGE_NAME;
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvContourTree_free_blocks(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  pushCvSeqBlock(L,seq->free_blocks);
  return 1;
}

static int CvContourTree_nfree_blocks(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);

  seq->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvContourTree_first(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  pushCvSeqBlock(L,seq->first);
  return 1;
}

static int CvContourTree_nfirst(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  seq->first=checkCvSeqBlock(L,3);
  return 0;
}


static int CvContourTree_block_max(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  pushUserData(L,seq->block_max);
  return 1;
}

static int CvContourTree_nblock_max(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int CvContourTree_ptr(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  pushUserData(L,seq->ptr);
  return 1;
}

static int CvContourTree_nptr(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int CvContourTree_p1(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  pushCvPoint(L,&seq->p1);
  return 1;
}

static int CvContourTree_np1(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  seq->p1=*checkCvPoint(L,3);
  return 0;
}

static int CvContourTree_p2(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  pushCvPoint(L,&seq->p2);
  return 1;
}

static int CvContourTree_np2(lua_State *L)
{
  CvContourTree *seq=checkCvContourTree(L,1);
  seq->p2=*checkCvPoint(L,3);
  return 0;
}



static const luacv_method CvContourTree_v[]=
{
  {NULL,17,NULL,NULL},
  methodReg(block_max,CvContourTree),
  methodReg(delta_elems,CvContourTree),
  methodReg(elem_size,CvContourTree),
  methodReg(first,CvContourTree),
  methodReg(flags,CvContourTree),
  methodReg(free_blocks,CvContourTree),
  methodReg(header_size,CvContourTree),
  methodReg(h_prev,CvContourTree),
  methodReg(h_next,CvContourTree),
  methodReg(ptr,CvContourTree),
  methodReg(p1,CvContourTree),
  methodReg(p2,CvContourTree),
  methodReg(storage,CvContourTree),
  methodReg(total,CvContourTree),
  methodReg(v_prev,CvContourTree),
  methodReg(v_next,CvContourTree),
  };

makeIndexFunctions(CvContourTree)
makeObjectCallback(CvContourTree);

