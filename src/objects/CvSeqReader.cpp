#include "CvSeqReader.h"

static int CvSeqReader_tostring(lua_State *L)
{
  CvSeqReader *seq=checkCvSeqReader(L,1);
  lua_pushfstring(L,CVSEQREADER_NAME" object %p\n\theader_size=%d\n\tseq="CVSEQ_NAME"\n\tblock="CVSEQBLOCK_NAME"\n\tptr=%p\n\tblock_min=%p\n\tblock_max=%p\n\tdelta_index=%d\n\tprev_elem=%p",seq,seq->header_size,seq->ptr,seq->block_min,seq->block_max,seq->delta_index,seq->prev_elem);

  return 1;
}

static int CvSeqReader_gc(lua_State *L)
{

  luacv_obj<CvSeqReader>*obj=(luacv_obj<CvSeqReader>*)luaL_checkudata(L,1,CVSEQREADER_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int CvSeqReader_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".header_size=int";
  checkCvSeqReader(L,1)->header_size=checkint(L,3);
return 0;
}

static int CvSeqReader_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSeqReader(L,1)->header_size);
return 1;
}

static int CvSeqReader_ndelta_index(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".delta_index=int";
  checkCvSeqReader(L,1)->delta_index=checkint(L,3);
return 0;
}

static int CvSeqReader_delta_index(lua_State *L)
{
  lua_pushnumber(L,checkCvSeqReader(L,1)->delta_index);
return 1;
}

static int CvSeqReader_nseq(lua_State *L)
{
  checkCvSeqReader(L,1)->seq=checkCvSeq(L,3);
return 0;
}

static int CvSeqReader_seq(lua_State *L)
{
  pushCvSeq(L,checkCvSeqReader(L,1)->seq);
return 1;
}

static int CvSeqReader_nblock(lua_State *L)
{
  checkCvSeqReader(L,1)->block=checkCvSeqBlock(L,3);
return 0;
}

static int CvSeqReader_block(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSeqReader(L,1)->block);
return 1;
}

static int CvSeqReader_nptr(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".ptr=userdata";
  checkCvSeqReader(L,1)->ptr=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqReader_ptr(lua_State *L)
{
  pushUserData(L,checkCvSeqReader(L,1)->ptr);
return 1;
}

static int CvSeqReader_nblock_min(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".block_min=userdata";
  checkCvSeqReader(L,1)->block_min=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqReader_block_min(lua_State *L)
{
  pushUserData(L,checkCvSeqReader(L,1)->block_min);
return 1;
}

static int CvSeqReader_nblock_max(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".block_max=userdata";
  checkCvSeqReader(L,1)->block_max=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqReader_block_max(lua_State *L)
{
  pushUserData(L,checkCvSeqReader(L,1)->block_max);
return 1;
}

static int CvSeqReader_nprev_elem(lua_State *L)
{
  const char f_msg[]=CVSEQREADER_NAME".prev_elem=userdata";
  checkCvSeqReader(L,1)->prev_elem=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqReader_prev_elem(lua_State *L)
{
  pushUserData(L,checkCvSeqReader(L,1)->prev_elem);
return 1;
}

static const luacv_method CvSeqReader_v[]=
{
  {NULL,9,NULL,NULL},
  methodReg(block,CvSeqReader),
  methodReg(block_min,CvSeqReader),
  methodReg(block_max,CvSeqReader),
  methodReg(delta_index,CvSeqReader),
  methodReg(header_size,CvSeqReader),
  methodReg(ptr,CvSeqReader),
  methodReg(prev_elem,CvSeqReader),
  methodReg(seq,CvSeqReader),
};

makeIndexFunctions(CvSeqReader)
makeObjectCallback(CvSeqReader);


