local lpeg = require "lpeg"
local C, Cs, Ct, P, S = lpeg.C, lpeg.Cs, lpeg.Ct, lpeg.P, lpeg.S

local eol = P'\r\n' + P'\n'
local quoted_field = '"'*Cs(((P(1)-'"')+P'""'/'"')^0)*'"'
local unquoted_field = C((1-S',\r\n"')^0)
local field = quoted_field + unquoted_field
local record = Ct(field*(','*field)^0)
local nonemptyrecord = #P(1-eol)*record
local records = Ct((record*eol)^0*nonemptyrecord^-1)*-1


function fromCSV(path, b)
  
  --return service.fromCSV(path)
  
    local fileUtils = cc.FileUtils:getInstance()
    local name = fileUtils:fullPathForFilename(path)
    local buffer = fileUtils:getStringFromFile(name)
	local ret = lpeg.match(records, buffer)

	for _,v in ipairs(ret) do
    local vT = v
    for j, k in ipairs(vT) do
			vT[j] = tonumber(k)
		end
	end
  
  if b then
    return ret
  else
    return ret
  end
end

function fromCSV_zero(path, b)
  
  --return service.fromCSV(path)
  
    local fileUtils = cc.FileUtils:getInstance()
    local name = fileUtils:fullPathForFilename(path)
    local buffer = fileUtils:getStringFromFile(name)
	local ret = lpeg.match(records, buffer)

	for _,v in ipairs(ret) do
    local vT = v
    for j, k in ipairs(vT) do
			vT[j] = tonumber(k)
      if vT[j] == nil then
        vT[j] = 0
      end
		end
	end
  
  if b then
    return ret
  else
    return ret
  end
end


function fromCSV_withStr(path, b)
  
  --return service.fromCSV(path)
  
    local fileUtils = cc.FileUtils:getInstance()
    local name = fileUtils:fullPathForFilename(path)
    local buffer = fileUtils:getStringFromFile(name)
	local ret = lpeg.match(records, buffer)

	for _,v in ipairs(ret) do
    local vT = v
    for j, k in ipairs(vT) do
      if j == 34 or tonumber(k) == nil then
        vT[j] = k
      else
        vT[j] = tonumber(k)
      end
		end
	end
  
  if b then
    return ret
  else
    return ret
  end
end

function fromCSV_Test(name)
  local f = assert(io.open(name, "r"))
	local t = f:read("*a")
	f:close()
	local ret = lpeg.match(records, t)

	for _,v in ipairs(ret) do
    local vT = v
		for j, k in ipairs(vT) do
			v[j] = tonumber(k)
		end
	end
	return ret
end

function fromCSV_Str(path, strIdx)
    local fileUtils = cc.FileUtils:getInstance()
    local name = fileUtils:fullPathForFilename(path)
    local buffer = fileUtils:getStringFromFile(name)
	local ret = lpeg.match(records, buffer)

	for _,v in ipairs(ret) do
		for j, k in ipairs(v) do
          if j < strIdx then
            v[j] = tonumber(k)
          end
		end
	end
	return ret
end

csvData = {}
function csvData:startData()
    csvData.mStr = require("stringtabel").create()
end