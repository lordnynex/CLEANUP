#include "CvChainPtReader.h"

static int CvChainPtReader_tostring(lua_State *L)
{
  CvChainPtReader *seq=checkCvChainPtReader(L,1);
  lua_pushfstring(L,CVCHAINPTREADER_NAME" object %p\n\theader_size=%d\n\tseq="CVSEQ_NAME"\n\tblock="CVSEQBLOCK_NAME"\n\tptr=%p\n\tblock_min=%p\n\tblock_max=%p\n\tdelta_index=%d\n\tprev_elem=%p\n\tcode=%d\n\tpoint="CVPOINT_NAME"\n\tdeltas[8][2]=char",seq,seq->header_size,seq->ptr,seq->block_min,seq->block_max,seq->delta_index,seq->prev_elem,seq->code);

  return 1;
}

static int CvChainPtReader_gc(lua_State *L)
{

  luacv_obj<CvChainPtReader>*obj=(luacv_obj<CvChainPtReader>*)luaL_checkudata(L,1,CVCHAINPTREADER_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int CvChainPtReader_nheader_size(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".header_size=int";
  checkCvChainPtReader(L,1)->header_size=checkint(L,3);
return 0;
}

static int CvChainPtReader_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvChainPtReader(L,1)->header_size);
return 1;
}

static int CvChainPtReader_ndelta_index(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".delta_index=int";
  checkCvChainPtReader(L,1)->delta_index=checkint(L,3);
return 0;
}

static int CvChainPtReader_delta_index(lua_State *L)
{
  lua_pushnumber(L,checkCvChainPtReader(L,1)->delta_index);
return 1;
}

static int CvChainPtReader_ncode(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".code=char";
  checkCvChainPtReader(L,1)->code=(char)checkint(L,3);
return 0;
}

static int CvChainPtReader_code(lua_State *L)
{
  lua_pushnumber(L,checkCvChainPtReader(L,1)->code);
return 1;
}
static int CvChainPtReader_nseq(lua_State *L)
{
  checkCvChainPtReader(L,1)->seq=checkCvSeq(L,3);
return 0;
}

static int CvChainPtReader_seq(lua_State *L)
{
  pushCvSeq(L,checkCvChainPtReader(L,1)->seq);
return 1;
}
static int CvChainPtReader_npt(lua_State *L)
{
  checkCvChainPtReader(L,1)->pt=*checkCvPoint(L,3);
return 0;
}

static int CvChainPtReader_pt(lua_State *L)
{
  pushCvPoint(L,&checkCvChainPtReader(L,1)->pt);
return 1;
}
static int CvChainPtReader_nblock(lua_State *L)
{
  checkCvChainPtReader(L,1)->block=checkCvSeqBlock(L,3);
return 0;
}

static int CvChainPtReader_block(lua_State *L)
{
  pushCvSeqBlock(L,checkCvChainPtReader(L,1)->block);
return 1;
}
static int CvChainPtReader_ndeltas(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvChainPtReader_deltas(lua_State *L)
{
  UNIMPL(L);
return 1;
}


static int CvChainPtReader_nptr(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".ptr=userdata";
  checkCvChainPtReader(L,1)->ptr=(schar*)checkldata(L,3);
return 0;
}

static int CvChainPtReader_ptr(lua_State *L)
{
  pushUserData(L,checkCvChainPtReader(L,1)->ptr);
return 1;
}

static int CvChainPtReader_nblock_min(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".block_min=userdata";
  checkCvChainPtReader(L,1)->block_min=(schar*)checkldata(L,3);
return 0;
}

static int CvChainPtReader_block_min(lua_State *L)
{
  pushUserData(L,checkCvChainPtReader(L,1)->block_min);
return 1;
}

static int CvChainPtReader_nblock_max(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".block_max=userdata";
  checkCvChainPtReader(L,1)->block_max=(schar*)checkldata(L,3);
return 0;
}

static int CvChainPtReader_block_max(lua_State *L)
{
  pushUserData(L,checkCvChainPtReader(L,1)->block_max);
return 1;
}

static int CvChainPtReader_nprev_elem(lua_State *L)
{
  const char f_msg[]=CVCHAINPTREADER_NAME".prev_elem=userdata";
  checkCvChainPtReader(L,1)->prev_elem=(schar*)checkldata(L,3);
return 0;
}

static int CvChainPtReader_prev_elem(lua_State *L)
{
  pushUserData(L,checkCvChainPtReader(L,1)->prev_elem);
return 1;
}

static const luacv_method CvChainPtReader_v[]=
{
  {NULL,12,NULL,NULL},
  methodReg(block,CvChainPtReader),
  methodReg(block_min,CvChainPtReader),
  methodReg(block_max,CvChainPtReader),
  methodReg(code,CvChainPtReader),
  methodReg(delta_index,CvChainPtReader),
  methodReg(deltas,CvChainPtReader),
  methodReg(header_size,CvChainPtReader),
  methodReg(ptr,CvChainPtReader),
  methodReg(pt,CvChainPtReader),
  methodReg(prev_elem,CvChainPtReader),
  methodReg(seq,CvChainPtReader),
};

makeIndexFunctions(CvChainPtReader)
makeObjectCallback(CvChainPtReader);


