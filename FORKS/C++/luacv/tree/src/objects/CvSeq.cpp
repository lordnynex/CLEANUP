#include "CvSeq.h"

static int CvSeq_tostring(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushfstring(L,CVSEQ_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME,seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems);
  return 1;
}

static int CvSeq_gc(lua_State *L)
{

  luacv_obj<CvSeq>*obj=(luacv_obj<CvSeq>*)luaL_checkudata(L,1,CVSEQ_NAME);
  if (obj->dealloc_data)
    cvClearSeq(obj->data);

  return 0;
}
static int CvSeq_flags(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushnumber(L,seq->flags);
  return 1;
}

static int CvSeq_nflags(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".flags=int";
  CvSeq *seq=checkCvSeq(L,1);
  seq->flags=checkint(L,3);
  return 0;
}

static int CvSeq_header_size(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushnumber(L,seq->header_size);
  return 1;
}

static int CvSeq_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".header_size=int";
  CvSeq *seq=checkCvSeq(L,1);
  seq->header_size=checkint(L,3);
  return 0;
}

static int CvSeq_total(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushnumber(L,seq->total);
  return 1;
}

static int CvSeq_ntotal(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".total=int";
  CvSeq *seq=checkCvSeq(L,1);
  seq->total=checkint(L,3);
  return 0;
}

static int CvSeq_elem_size(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushnumber(L,seq->elem_size);
  return 1;
}

static int CvSeq_nelem_size(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".elem_size=int";
  CvSeq *seq=checkCvSeq(L,1);
  seq->elem_size=checkint(L,3);
  return 0;
}

static int CvSeq_delta_elems(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  lua_pushnumber(L,seq->delta_elems);
  return 1;
}

static int CvSeq_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME".delta_elems=int";
  CvSeq *seq=checkCvSeq(L,1);
  seq->delta_elems=checkint(L,3);
  return 0;
}

static int CvSeq_h_prev(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeq(L,seq->h_prev);
  return 1;
}

static int CvSeq_nh_prev(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".h_prev="CVSEQ_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvSeq_h_next(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeq(L,seq->h_next);
  return 1;
}

static int CvSeq_nh_next(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".h_next="CVSEQ_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvSeq_v_prev(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeq(L,seq->v_prev);
  return 1;
}

static int CvSeq_nv_prev(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".v_prev="CVSEQ_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->v_prev=checkCvSeq(L,3);
  return 0;
}
static int CvSeq_v_next(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeq(L,seq->v_next);
  return 1;
}

static int CvSeq_nv_next(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".v_next="CVSEQ_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvSeq_storage(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvMemStorage(L,seq->storage);
  return 1;
}

static int CvSeq_nstorage(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".storage="CVMEMSTORAGE_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvSeq_free_blocks(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeqBlock(L,seq->free_blocks);
  return 1;
}

static int CvSeq_nfree_blocks(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".free_blocks="CVSEQBLOCK_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvSeq_first(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushCvSeqBlock(L,seq->first);
  return 1;
}

static int CvSeq_nfirst(lua_State *L)
{
  //const char f_msg[]=CVSEQ_NAME".first="CVSEQBLOCK_NAME;
  CvSeq *seq=checkCvSeq(L,1);
  seq->first=checkCvSeqBlock(L,3);
  return 0;
}


static int CvSeq_block_max(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushUserData(L,seq->block_max);
  return 1;
}

static int CvSeq_nblock_max(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int CvSeq_ptr(lua_State *L)
{
  CvSeq *seq=checkCvSeq(L,1);
  pushUserData(L,seq->ptr);
  return 1;
}

static int CvSeq_nptr(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static const luacv_method CvSeq_v[]=
{
  {NULL,15,NULL,NULL},
  methodReg(block_max,CvSeq),
  methodReg(delta_elems,CvSeq),
  methodReg(elem_size,CvSeq),
  methodReg(first,CvSeq),
  methodReg(flags,CvSeq),
  methodReg(free_blocks,CvSeq),
  methodReg(header_size,CvSeq),
  methodReg(h_prev,CvSeq),
  methodReg(h_next,CvSeq),
  methodReg(ptr,CvSeq),
  methodReg(storage,CvSeq),
  methodReg(total,CvSeq),
  methodReg(v_prev,CvSeq),
  methodReg(v_next,CvSeq),
  };

makeIndexFunctions(CvSeq)
makeObjectCallback(CvSeq);

