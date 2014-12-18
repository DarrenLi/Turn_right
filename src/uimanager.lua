local _allTests = {
    { id = 1, isBack = true,  name = "startscene"     , create_func= function() return  require("startscene").create() end },
    { id = 2, isBack = true,  name = "gamescene"      , create_func= function() return  require("GameScene").create() end },
  }

local TESTS_COUNT = table.getn(_allTests)
uimanager = {
    mIdx = 1,
    mArray = {}
}
-- create scene
function uimanager:CreateScene(nIdx,backBtn)
    if nIdx < 1 or nIdx > TESTS_COUNT then return nil end
    cc.Director:getInstance():purgeCachedData()
    local scene = _allTests[nIdx].create_func()
    table.insert(self.mArray,1,_allTests[nIdx])
    self.mIdx = nIdx
    if self:IsBack() then
        if backBtn == nil then backBtn = self:CreateBackBtn() end
        scene:addChild(backBtn)
    end
    return scene
end

function uimanager:BackScene()
    table.remove(self.mArray,1)
    if self.mArray[1].isBack then
        local scene = self.mArray[1].create_func()
        if self:IsBack() then
            scene:addChild(self:CreateBackBtn())
        end
        if scene then
            cc.Director:getInstance():replaceScene(scene)
        end
    else
        self:BackScene()
    end
end

function uimanager:IsBack()
    local back = false
	if table.getn(self.mArray) > 1 then
	   back = true
	end
	return back
end

function uimanager:CreateBackBtn()
    cc.SpriteFrameCache:getInstance():addSpriteFrames("ui/2dui_dlcj.plist")
    local button = ccui.Button:create()
    button:setTouchEnabled(true)
    button:loadTextures("2dui_denglu_anniunormal.png", "2dui_denglu_anniupushed.png", "",1)
    button:setPosition(cc.p(100, 50))
    local function onBackScene(sender, eventType)
        if eventType == ccui.TouchEventType.ended then
            uimanager:BackScene()
        end
    end
    button:addTouchEventListener(onBackScene)
    return button
end
