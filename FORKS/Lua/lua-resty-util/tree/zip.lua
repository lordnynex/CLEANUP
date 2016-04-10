local ffi = require("ffi")
local setmetatable = setmetatable


local _M = {}

local mt = {
    __index = _M
    }


ffi.cdef[[
        typedef signed char     int8_t;
        typedef unsigned char   uint8_t;
        typedef signed int  int16_t;
        typedef unsigned int    uint16_t;
        typedef signed long int     int32_t; 
        typedef unsigned long int   uint32_t;
        typedef signed long long int    int64_t; 
        typedef unsigned long long int  uint64_t;
        typedef unsigned long long int  uint64_t;

        typedef uint16_t zip_uint16_t;
        typedef uint32_t zip_uint32_t;
        typedef uint64_t zip_uint64_t;

        typedef long time_t;
        typedef unsigned int size_t;

        struct zip_stat {
            zip_uint64_t valid;
            const char *name;    
            zip_uint64_t index;      
            zip_uint64_t size;
            zip_uint64_t comp_size;
            time_t mtime;     
            zip_uint32_t crc;    
            zip_uint16_t comp_method;       
            zip_uint16_t encryption_method;
            zip_uint32_t flags;         
        };

        struct zip;
        struct zip_file;
        struct zip_source;

        struct zip * zip_open(const char *path, int flags, int *);
        const char * zip_strerror(struct zip *archive);
        int zip_error_to_str(char *buf, int len, int ze, int se);
        int zip_get_num_files(struct zip *archive);
        const char * zip_get_name(struct zip *archive, int index, int flags);
        int zip_stat(struct zip *archive, const char *fname, int flags, struct zip_stat *sb);
        int zip_stat_index(struct zip *archive, int index, int flags, struct zip_stat *sb);
        struct zip_file * zip_fopen_index(struct zip *archive, int index, int flags);
        struct zip_file * zip_fopen(struct zip *archive, const char *fname, int flags);
        int zip_fread(struct zip_file *file, void *buf, int nbytes);
        int zip_name_locate(struct zip *archive, const char *fname, int flags);


        typedef struct {
          char *fpos;
          void *base;
          unsigned short handle;
          short flags;
          short unget;
          unsigned long alloc;
          unsigned short buffincrement;
        } FILE;

        FILE *fopen(const char *path, const char *mode);
        size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
        int fclose(FILE *stream);
]]

local zip = ffi.load("libzip")
local orp = ffi.new("int[1]", 0)

function _M:new( filename )
    local cfilename = ffi.new("char["..#filename+1 .."]", filename)
    local zf = zip.zip_open(cfilename, 0, orp)
    local errno = ffi.errno()
    if zf == 0 then
        local buf = ffi.new("char[100]")
        zip.zip_error_to_str(buf, 100, orp[0], errno)
        local errmessage = ffi.string(buf)
        return nil, errmessage
    end
    local num = zip.zip_get_num_files(zf)
    return setmetatable({_zf = zf, _fnum = num }, mt)
end

function _M:listfiles()
    local num = self._fnum
    local ft = {}
    for i=0, num-1 do
        ft[i+1] = ffi.string(zip.zip_get_name(self._zf,i,0))
    end
    return ft
end

function _M:findapkfile()
    local num = self._fnum
    for i=0, num-1 do
        filename = ffi.string(zip.zip_get_name(self._zf,i,0))
        m,err = ngx.re.match(filename, '.*\\.apk')
        if m then
            return m[0]
        end
    end
    return nil
end

function _M:fdecompress (filename, dstdir)
    local index = zip.zip_name_locate(self._zf, filename, 0)
    if index == -1 then
        return nil, "no entry of the name "..filename.." is found in the archive"
    end
    local zs = ffi.new("struct zip_stat[1]")
    local rtn = zip.zip_stat_index(self._zf, index, 0,zs)
    if rtn == -1 then
        return nil, "get zip file info failed"
    end
    local zsobj = zs[0]
    local zf = zip.zip_fopen_index(self._zf,index,0)
    if zf == nil then
        return  nil, "open "..filename.." err"
    end
    local buf = ffi.new("char[4096]")
    local fp = ffi.C.fopen(dstdir.."/"..filename, "w+")
    local sum = 0
    while (sum ~= zsobj.size) do
        len = zip.zip_fread(zf, buf, 4096)
        ffi.C.fwrite(buf, 1, len, fp)
        sum = sum + len
    end
    ffi.C.fclose(fp)
    return 0
    end
return _M