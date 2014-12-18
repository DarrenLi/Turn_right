
local StartScene = class("StartScene",function()
    return cc.Scene:create()
end)

function StartScene.create()
    local scene = StartScene.new()
    scene:addChild(scene:createLayer())
    return scene
end


function StartScene:ctor()
    self.visibleSize = cc.Director:getInstance():getVisibleSize()
    self.origin = cc.Director:getInstance():getVisibleOrigin()
    self.schedulerID = nil
end

function StartScene:playBgMusic()
end

function StartScene:createLayer()
    local rootNode = cc.CSLoader:createNode("startscene.csb")

    local function onStartClick(sender, eventType)
        if eventType == ccui.TouchEventType.ended then
            local scene = uimanager:CreateScene(2)
            cc.Director:getInstance():replaceScene(scene)
        end
    end
    local startBtn = rootNode:getChildByName("Button_1")
    startBtn:addTouchEventListener(onStartClick)
    return rootNode
end


return StartScene
