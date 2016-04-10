local runtimeModule = require('abtesting.adapter.runtime')
local policyModule  = require('abtesting.adapter.policy')
local redisModule   = require('abtesting.utils.redis')
local systemConf    = require('abtesting.utils.init')
local handler       = require('abtesting.error.handler').handler
local utils         = require('abtesting.utils.utils')
local ERRORINFO     = require('abtesting.error.errcode').info

local cjson         = require('cjson.safe')
local doresp        = utils.doresp
local dolog         = utils.dolog

local redisConf     = systemConf.redisConf
local divtypes      = systemConf.divtypes
local prefixConf    = systemConf.prefixConf
local policyLib     = prefixConf.policyLibPrefix
local runtimeLib    = prefixConf.runtimeInfoPrefix
local domain_name   = prefixConf.domainname

local policyID      = ngx.var.arg_policyid

if policyID then
    policyID = tonumber(ngx.var.arg_policyid)
    if not policyID or policyID < 0 then
        local info = ERRORINFO.PARAMETER_TYPE_ERROR 
        local desc = "policyID should be a positive Integer"
        local response = doresp(info, desc)
        dolog(info, desc)
        ngx.say(response)
        return
    end
end

if not policyID then
    local request_body  = ngx.var.request_body
    local postData      = cjson.decode(request_body)
    
    if not request_body then
        -- ERRORCODE.PARAMETER_NONE
        local info = ERRORINFO.PARAMETER_NONE 
        local desc = 'request_body or post data to get policyID'
        local response = doresp(info, desc)
        dolog(info, desc)
        ngx.say(response)
        return
    end
    
    if not postData then
        -- ERRORCODE.PARAMETER_ERROR
        local info = ERRORINFO.PARAMETER_ERROR 
        local desc = 'postData is not a json string'
        local response = doresp(info, desc)
        dolog(info, desc)
        ngx.say(response)
        return
    end
    
    policyID = postData.policyid
    
    if not policyID then
        local info = ERRORINFO.PARAMETER_ERROR 
        local desc = "policyID is needed"
        local response = doresp(info, desc)
        dolog(info, desc)
        ngx.say(response)
        return
    end
    
    policyID = tonumber(postData.policyid)
    
    if not policyID or policyID < 0 then
        local info = ERRORINFO.PARAMETER_TYPE_ERROR 
        local desc = "policyID should be a positive Integer"
        local response = doresp(info, desc)
        dolog(info, desc)
        ngx.say(response)
        return
    end
end

local red = redisModule:new(redisConf)
local ok, err = red:connectdb()
if not ok then
    local info = ERRORINFO.REDIS_CONNECT_ERROR
    local response = doresp(info, err)
    dolog(info, desc)
    ngx.say(response)
    return
end


local pfunc = function()
    local runtimeMod = runtimeModule:new(red.redis, runtimeLib) 
    return runtimeMod:get(domain_name)
end

local status, info = xpcall(pfunc, handler)
if not status then
    local errinfo   = info[1]
    local errstack  = info[2] 
    local err, desc = errinfo[1], errinfo[2]
    local response  = doresp(err, desc)
    dolog(err, desc, nil, errstack)
    ngx.say(response)
    return
end

local divModname        = info[1]
local divPolicy         = info[2]
local userInfoModname   = info[3]

if divPolicy and divPolicy ~= ngx.null then
    local s, e = string.find(divPolicy, policyLib)
    if s and e then
        local sub = string.sub(divPolicy, e+1)
        local _, _, num = string.find(sub, "(%d+)")
        num = tonumber(num)
        if num and num == policyID then
            local response = doresp(ERRORINFO.POLICY_BUSY_ERROR, policyID)
            ngx.say(response)
            return
        end
    end
end

local pfunc = function()
    local policyMod = policyModule:new(red.redis, policyLib) 
    return policyMod:del(policyID)
end

local status, info = xpcall(pfunc, handler)
if not status then
    local errinfo   = info[1]
    local errstack  = info[2] 
    local err, desc = errinfo[1], errinfo[2]
    local response  = doresp(err, desc)
    dolog(err, desc, nil, errstack)
    ngx.say(response)
    return
end

local response = doresp(ERRORINFO.SUCCESS)
ngx.say(response)
