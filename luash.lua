local tempfile = "/tmp/temp.log"
local _M  = { 
        _VERSION = '0.01'
    }

local mt = {
    __index = _M
}


local function execute(cmd)
    local cmd = cmd.." >"..tempfile.." ".."2>&1" 
    local stdout, stderr
    local code = os.execute(cmd)
    local file = io.open(tempfile, "r")
    local output = file:read("*all")
    file:close()
    if code > 0 then
        return code, stdout, output
    end
    return code, output, stderr
end

local function create_dir(dirname)
    local code, out, err = execute("mkdir -p "..dirname)
    if code ~=0 then
        return code, err
    end
    return 0
end
    
local function dirname(filepath)
    local cmd = "dirname "..filepath
    local code, out, err = execute(cmd)
    if code ~= 0 then
        return nil, err
    end
    local dname = string.gsub(out,'\n','')
    return dname
end
     
local function create_dir_if_not_exist(dirname)
    local code, out, err = execute("ls "..dirname)
    if code == 0 then
        return code
    end
    local ccode, cerr = create_dir(dirname)
    if code ~= 0 then
        return ccode, cerr
    end
    return ccode
end

_M["execute"] = execute
_M["create_dir"] = create_dir
_M["dirname"] = dirname
_M["create_dir_if_not_exist"] = create_dir_if_not_exist


return _M
