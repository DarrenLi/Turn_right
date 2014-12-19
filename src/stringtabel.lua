
local stringtabel = class("stringtabel",function()
    return {}
end)

function stringtabel.create()
    local strT = stringtabel.new()
    strT.strk =
    {
      stringId = 2,
      chs = 3,
      thai = 4,
    }
    strT:createData()
    strT:loadFromCSV()
    return strT
end

function stringtabel:ctor()
end

function stringtabel:playBgMusic()
end

function stringtabel:createData()
    self.strTable =
    {
      [10011] = "你妹好了没呀？",
    }
end

function stringtabel:getLangaugeIndex()
    if myutil:getSdkValues() ~= nil and myutil:getSdkValues().getLanguage ~= nil then
        return self.strk[util:getSdkValues():getLanguage()]
    end
    return self.strk.chs
end

function stringtabel:loadFromCSV()
  local tCsv = assert(fromCSV_Str("data/strings.csv", self:getLangaugeIndex()))

  for i = 1, math.huge do
      local t = tCsv[i]
      if t == nil then break end
      if t[self.strk.stringId] == nil then break end
    
      local function matchStr(str)
        return '\n'
      end
        self.strTable[t[self.strk.stringId]] = string.gsub(t[self:getLangaugeIndex()], "%\\(%w)", matchStr)
      end
end

function stringtabel:localString(id)
    if self.strTable[id] == nil then return "" end
    return self.strTable[id]
end

return stringtabel