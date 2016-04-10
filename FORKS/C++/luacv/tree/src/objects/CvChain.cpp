#include "CvChain.h"
static int CvChain_tostring(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushfstring(L,CVCHAIN_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\torigin="CVPOINT_NAME,seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems);
  return 1;
}

static int CvChain_gc(lua_State *L)
{
  CvChain *chain=checkCvChain(L,1);
  if (chain)
  {
    cvClearSeq((CvSeq*)chain);
    luacv_free(&chain);
  }
  return 0;
}

static int CvChain_flags(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushnumber(L,seq->flags);
  return 1;
}

static int CvChain_nflags(lua_State *L)
{
  const char f_msg[]=CVCHAIN_NAME".flags=int";
  CvChain *seq=checkCvChain(L,1);
  seq->flags=checkint(L,3);
  return 0;
}

static int CvChain_header_size(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushnumber(L,seq->header_size);
  return 1;
}

static int CvChain_nheader_size(lua_State *L)
{
  const char f_msg[]=CVCHAIN_NAME".header_size=int";
  CvChain *seq=checkCvChain(L,1);
  seq->header_size=checkint(L,3);
  return 0;
}

static int CvChain_total(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushnumber(L,seq->total);
  return 1;
}

static int CvChain_ntotal(lua_State *L)
{
  const char f_msg[]=CVCHAIN_NAME".total=int";
  CvChain *seq=checkCvChain(L,1);
  seq->total=checkint(L,3);
  return 0;
}

static int CvChain_elem_size(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushnumber(L,seq->elem_size);
  return 1;
}

static int CvChain_nelem_size(lua_State *L)
{
  const char f_msg[]=CVCHAIN_NAME".elem_size=int";
  CvChain *seq=checkCvChain(L,1);
  seq->elem_size=checkint(L,3);
  return 0;
}

static int CvChain_delta_elems(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  lua_pushnumber(L,seq->delta_elems);
  return 1;
}

static int CvChain_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVCHAIN_NAME".delta_elems=int";
  CvChain *seq=checkCvChain(L,1);
  seq->delta_elems=checkint(L,3);
  return 0;
}

static int CvChain_h_prev(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeq(L,seq->h_prev);
  return 1;
}

static int CvChain_nh_prev(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".h_prev="CVCHAIN_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvChain_h_next(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeq(L,seq->h_next);
  return 1;
}

static int CvChain_nh_next(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".h_next="CVCHAIN_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvChain_v_prev(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeq(L,seq->v_prev);
  return 1;
}

static int CvChain_nv_prev(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".v_prev="CVCHAIN_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->v_prev=checkCvSeq(L,3);
  return 0;
}
static int CvChain_v_next(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeq(L,seq->v_next);
  return 1;
}

static int CvChain_nv_next(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".v_next="CVCHAIN_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvChain_storage(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvMemStorage(L,seq->storage);
  return 1;
}

static int CvChain_nstorage(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".storage="CVMEMSTORAGE_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvChain_origin(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvPoint(L,&seq->origin);
  return 1;
}

static int CvChain_norigin(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".storage="CVMEMSTORAGE_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->origin=*checkCvPoint(L,3);
  return 0;
}

static int CvChain_free_blocks(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeqBlock(L,seq->free_blocks);
  return 1;
}

static int CvChain_nfree_blocks(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".free_blocks="CVCHAINBLOCK_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvChain_first(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushCvSeqBlock(L,seq->first);
  return 1;
}

static int CvChain_nfirst(lua_State *L)
{
  //const char f_msg[]=CVCHAIN_NAME".first="CVCHAINBLOCK_NAME;
  CvChain *seq=checkCvChain(L,1);
  seq->first=checkCvSeqBlock(L,3);
  return 0;
}


static int CvChain_block_max(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushUserData(L,seq->block_max);
  return 1;
}

static int CvChain_nblock_max(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int CvChain_ptr(lua_State *L)
{
  CvChain *seq=checkCvChain(L,1);
  pushUserData(L,seq->ptr);
  return 1;
}

static int CvChain_nptr(lua_State *L)
{
  UNIMPL(L);
  return 0;
}


static const luacv_method CvChain_v[]=
{
  {NULL,16,NULL,NULL},
  methodReg(block_max,CvChain),
  methodReg(delta_elems,CvChain),
  methodReg(elem_size,CvChain),
  methodReg(first,CvChain),
  methodReg(flags,CvChain),
  methodReg(free_blocks,CvChain),
  methodReg(header_size,CvChain),
  methodReg(h_prev,CvChain),
  methodReg(h_next,CvChain),
  methodReg(origin,CvChain),
  methodReg(ptr,CvChain),
  methodReg(storage,CvChain),
  methodReg(total,CvChain),
  methodReg(v_prev,CvChain),
  methodReg(v_next,CvChain),
  };

makeIndexFunctions(CvChain)
makeObjectCallback(CvChain);
