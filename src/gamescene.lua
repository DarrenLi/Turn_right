
local GameScene = class("GameScene",function()
    return cc.Scene:create()
end)

function GameScene.create()
    local scene = GameScene.new()
    scene:addChild(scene:createLayer())
    return scene
end


function GameScene:ctor()
    self.visibleSize = cc.Director:getInstance():getVisibleSize()
    self.origin = cc.Director:getInstance():getVisibleOrigin()
    self.schedulerID = nil
end

function GameScene:playBgMusic()
end

function GameScene:createLayer()
    local rootNode = cc.CSLoader:createNode("gamescene.csb")
    
    local mapNode = rootNode:getChildByName("Map_1")
    
    return rootNode
end


return GameScene
