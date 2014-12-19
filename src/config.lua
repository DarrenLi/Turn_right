local cdr = {
    cardupgradeexprate = 1,
    mQTEPrefectScore = 2,
    mQTEPrefectRate = 3,
    mQTEGreatScore = 4,
    mQTEGreatRate = 5,
    mQTEGoodScore = 6,
    mQTEGoodRate = 7,
    mQTECoolScore = 8,
    mQTECoolRate = 9,
    mQTEBadScore = 10,
    mQTEBadRate = 11,
    mFriendAssistCD = 12,
    mFriendClearCD = 13,
    mFriendMinAssist = 14,
    mFriendMaxAssist = 15,
    mPvPCoolDown = 16,
    mGoldCostForClearPvPCoolDown = 17,
    mNoQTERate = 18,
    mTreasureCount=19,
    mMatrixLotteryTime=20,
    mMaterixLotteryUse=21,
}

configDataTable = {}

local function loadFromCSV()
    local tCsv = assert(fromCSV("data/config.csv"))

    for i = 1, math.huge do
        local t = tCsv[i]
        if t == nil then 
            break 
        end
        if t[1] == nil then
            break
        end
        function t:cardupgradeexprate()
            return self[cdr.cardupgradeexprate]
        end

        function t:QTEPrefectScore()
            return self[cdr.mQTEPrefectScore]
        end

        function t:QTEPrefectRate()
            return self[cdr.mQTEPrefectRate]
        end

        function t:QTEGreatScore()
            return self[cdr.mQTEGreatScore]
        end

        function t:QTEGreatRate()
            return self[cdr.mQTEGreatRate]
        end

        function t:QTEGoodScore()
            return self[cdr.mQTEGoodScore]
        end

        function t:QTEGoodRate()
            return self[cdr.mQTEGoodRate]
        end

        function t:QTECoolScore()
            return self[cdr.mQTECoolScore]
        end

        function t:QTECoolRate()
            return self[cdr.mQTECoolRate]
        end

        function t:QTEBadScore()
            return self[cdr.mQTEBadScore]
        end

        function t:QTEBadRate()
            return self[cdr.mQTEBadRate]
        end

        function t:friendAssistCD()
            return self[cdr.mFriendAssistCD]
        end

        function t:friendClearCD()
            return self[cdr.mFriendClearCD] 
        end

        function t:friendMinAssist()
            return self[cdr.mFriendMinAssist]
        end

        function t:friendMaxAssist()
            return self[cdr.mFriendMaxAssist]
        end

        function t:pvpCoolDown()
            return self[cdr.mPvPCoolDown]
        end

        function t:goldCostForClearPvPCoolDown()
            return self[cdr.mGoldCostForClearPvPCoolDown]
        end

        function t:NoQTERate()
            return self[cdr.mNoQTERate]
        end

        function t:TreasureCount()
            return self[cdr.mTreasureCount]
        end

        function t:MaterixLotteryUse()
            return self[cdr.mMaterixLotteryUse]
        end

        configDataTable = t
    end
end

loadFromCSV()

function getQTEScore(scoreType)
    return configDataTable[scoreType*2]
end

function getQTERate(QTEscore)
    for i=cdr.mQTEPrefectScore,cdr.mQTEBadScore do
        if QTEscore >= configDataTable[i] then
            return configDataTable[i+1],i
        end
        i = i+1
    end
end

function getQTEType(QTERate)
    for i=cdr.mQTEPrefectRate, cdr.mFriendAssistCD do
        if (QTERate == configDataTable[i]) then
            return configDataTable[i-1],(i-1)
        end
    end
end