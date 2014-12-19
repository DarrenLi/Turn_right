myutil = {sdkValues = nil}
myutil.localAndServerTimeDistance = 0
myutil.SecondsOfPerSpiritRecover = 5*60
myutil.SecondsOfPerEnergyRecover = 30*60

myutil.SecondsOfAthleticsCDRecover = 10*60
myutil.SecondsOfAthleticsAwardInerval = 60*60

function myutil:getSecsOfPerSpiritRecover()
	return self.SecondsOfPerSpiritRecover
end

function myutil:getSecsOfPerEnergyRecover()
	return self.SecondsOfPerEnergyRecover
end

function myutil:getSpiritNextRecoverTime(serverNextTime)
	local localToServerTime = os.time() - self.localAndServerTimeDistance
	local timeDistance = localToServerTime - serverNextTime
	local n = math.floor(timeDistance/self.SecondsOfPerSpiritRecover)
	local remainTime = (n+1)*self.SecondsOfPerSpiritRecover - timeDistance

	return remainTime
end

function myutil:getEnergyNextRecoverTime(serverNextTime)
	local localToServerTime = os.time() - self.localAndServerTimeDistance
	local timeDistance = localToServerTime - serverNextTime
	local n = math.floor(timeDistance/self.SecondsOfPerEnergyRecover)
	local remainTime = (n+1)*self.SecondsOfPerEnergyRecover - timeDistance

	return remainTime
end

function myutil:getAthleticsAwardRemainTime()
	local localToServerTime = os.time() - self.localAndServerTimeDistance
	local timeDistance = localToServerTime - ClientRole:getRole():getAthleticsAwardTime()

	local remain = timeDistance%myutil.SecondsOfAthleticsAwardInerval
	if 0 == remain then
		remain = myutil.SecondsOfAthleticsAwardInerval
	end

	return remain

end

function myutil:getFormatTimeFromSecs(timeSecs)
	local hours,remain = math.floor(timeSecs/3600),timeSecs - math.floor(timeSecs/3600)*3600
	if hours < 10 then
		hours = '0'..tostring(hours)
	end

	local minutes,remain = math.floor(remain/60),remain - math.floor(remain/60)*60
	if minutes < 10 then
		minutes = '0'..tostring(minutes)
	end

	if remain < 10 then
		remain = '0'..tostring(remain)
	end

	local timeFormatStr = ""
	if hours == "00" then
		timeFormatStr = tostring(minutes)..":"..tostring(remain)
	else
		timeFormatStr = tostring(hours)..":"..tostring(minutes)..":"..tostring(remain)
	end

	return timeFormatStr
end


function myutil:getFormatTimeFromSecsWithUnit(timeSecs)
	local hours,remain = math.floor(timeSecs/3600),timeSecs - math.floor(timeSecs/3600)*3600
	local minutes,remain = math.floor(remain/60),remain - math.floor(remain/60)*60
	local timeFormatStr = ""
	if timeSecs > 60 then
		if hours == "00" then
			timeFormatStr = tostring(minutes)..localString(minuteStrId)
		else
			timeFormatStr = tostring(hours)..localString(hourStrId)..tostring(minutes)..localString(minuteStrId)
		end
	else
		timeFormatStr = localString(dueStrId)
	end

	return timeFormatStr
end

function myutil:getFormatDayTimeFromSecs(timeSecs,bDisplayMinute)
	local hours,remain = math.floor(timeSecs/3600),timeSecs - math.floor(timeSecs/3600)*3600

	local day = math.floor(hours/24)

	local minutes,remain = math.floor(remain/60),remain - math.floor(remain/60)*60
	if minutes < 10 then
		minutes = '0'..tostring(minutes)
	end

	if remain < 10 then
		remain = '0'..tostring(remain)
	end

	if day > 0 then
		if day > 30 then
			return localString(over30DaysStrId)
		else
			return tostring(day) .. localString(dayStrId)
		end
	elseif hours > 0 then
		if true == bDisplayMinute then
			return myutil:getFormatTimeFromSecsWithUnit(timeSecs)
		else
			return tostring(hours) .. localString(hourStrId)
		end
		
	else
		return localString(dueStrId)
	end
end

function myutil:getFormatDayHoursTimeFromSecs(timeSecs)
	local hours,remain = math.floor(timeSecs/3600),timeSecs - math.floor(timeSecs/3600)*3600

	local day = math.floor(hours/24)

	hours = hours % 24

	return tostring(day) .. localString(dayStrId)..tostring(hours) .. localString(hourStrId)
end

function myutil:getFormatTimeFromSecsWithWord(timeSecs)
	local hours,remain = math.floor(timeSecs/3600),timeSecs - math.floor(timeSecs/3600)*3600
	
	local minutes,remain = math.floor(remain/60),remain - math.floor(remain/60)*60 

	local temp = minutes % 10
	if temp >= 5 then
		minutes = minutes + (10 - temp)
	else
		minutes = minutes - temp
	end  

	if minutes == 60 then hours = hours + 1 end
	if minutes == 0 then minutes = 10 end

	local timeFormatStr = ""
	if hours == 0 then
		timeFormatStr = tostring(minutes)..localString(minuteStrId)
	else
		if minutes == 60 then
			timeFormatStr = tostring(hours)..localString(hourStrId)
		else
			timeFormatStr = tostring(hours)..localString(hourStrId)..tostring(minutes)..localString(minuteStrId)
		end
	end

	return timeFormatStr
end

function myutil:getSdkValues()
  if self.sdkValues == nil and sdkvalue ~= nil then
    self.sdkValues = sdkvalue()
  end
  return self.sdkValues
end

function myutil:getConfigType()
	if self:getSdkValues().getConfigType ~= nil then
		return self:getSdkValues():getConfigType()
	else
		return string.format("%s.%s.dis",self:getSdkValues():getPlatformName(),self:getSdkValues():getDeviceType())
	end
end

function myutil:openURL(str)
	if self:getSdkValues().openURL ~= nil and str ~= "" then
		self:getSdkValues():openURL(str)
	end
end

function myutil:OpenEvaluationURL()
	if getChannelConfigData():getEvaluationURL() ~= "" then
		self:openURL(getChannelConfigData():getEvaluationURL())
	end
end

function myutil:getEvaluationFaceURL()
	if self:getSdkValues().openURL ~= nil and getChannelConfigData():FaceBookEvIP() ~= "" then
		return getChannelConfigData():FaceBookEvIP()
	end
	return nil
end

function myutil:OpenEvaluationFaceURL()
	if self:getEvaluationFaceURL() ~= nil then
		self:openURL(self:getEvaluationFaceURL())
	end
end

function myutil:getShareFacebookURL()
	if self:getSdkValues().shareFacebook ~= nil and getChannelConfigData():getFacebookUrl() ~= "" then
		return getChannelConfigData():getFacebookUrl()
	end
	return nil
end

function myutil:ShareFacebookURL()
	if self:getShareFacebookURL() ~= nil then
		self:getSdkValues():shareFacebook(
				localString(7007001),
				string.format(localString(7007101),ClientRole:getRole():getLevel()),
				getChannelConfigData():getFacebookUrl(),
				getChannelConfigData():getSFIconURL()
				)
	end
end

function myutil:flyerTrackEvent()
	if self:getSdkValues().flyerTrackEvent ~= nil then
  		local level = ClientRole:getRole():getLevel()
  		if level == 3 or level == 10 or level == 20 then
			self:getSdkValues():flyerTrackEvent(string.format("Level%dFinish",level),"")
		end
	end
end

function myutil:getSdkCurrencytype()
    if self.getSdkValues().getCurrencytype ~= nil then
        return Currency_type[self:getSdkValues():getCurrencytype()]
	end
	return Currency_type[1]
end

function myutil:dateToTimeSecs(iYear,iMonth,iDay,iHour,iMin,iSecs)
	local time = os.time{ year = iYear, month = iMonth, day = iDay, hour = iHour, min = iMin, sec= iSecs} 
	return time
end

function  myutil:convert2SecsFromYMDHMS(dateStr)
	local nowTime  = myutil:getServerTime()
	local startYear =  tonumber(string.sub(dateStr,1,4))
	local startMonth = tonumber(string.sub(dateStr,5,6))
	local startDay = tonumber(string.sub(dateStr,7,8))
	local startHour = tonumber(string.sub(dateStr,9,10))
	local startMin = tonumber(string.sub(dateStr,11,12))
	local startSec = tonumber(string.sub(dateStr,13,14))
	return myutil:dateToTimeSecs(startYear,startMonth,startDay,startHour,startMin,startSec)
end

function myutil:convert2SecsFromYMD(dateStr)
      local tm = {}
      tm.year = tonumber(string.sub(dateStr,1,4))
      tm.month = tonumber(string.sub(dateStr,5,6))
      tm.day = tonumber(string.sub(dateStr,7,8))
      tm.hour = 0
      return os.time(tm)
    end

function  myutil:nowIsInRange(startTimeStr,endTimeStr)
	local nowTime  = myutil:getServerTime()
	local startYear =  tonumber(string.sub(startTimeStr,1,4))
	local startMonth = tonumber(string.sub(startTimeStr,5,6))
	local startDay = tonumber(string.sub(startTimeStr,7,8))
	local startHour = tonumber(string.sub(startTimeStr,9,10))
	local startMin = tonumber(string.sub(startTimeStr,11,12))
	local startSec = tonumber(string.sub(startTimeStr,13,14))

	local endYear = tonumber(string.sub(endTimeStr,1,4))
	local endMonth = tonumber(string.sub(endTimeStr,5,6))
	local endDay = tonumber(string.sub(endTimeStr,7,8))
	local endHour = tonumber(string.sub(endTimeStr,9,10))
	local endMin = tonumber(string.sub(endTimeStr,11,12))
	local endSec = tonumber(string.sub(endTimeStr,13,14))

	local startTime = myutil:dateToTimeSecs(startYear,startMonth,startDay,startHour,startMin,startSec)
	local endTime = myutil:dateToTimeSecs(endYear,endMonth,endDay,endHour,endMin,endSec)
	if nowTime >= startTime and nowTime < endTime then
		return true
	else
		return false
	end
end

function myutil:deepcopy(t)
	if type(t) ~= 'table' then return t end
	local mt = getmetatable(t)
	local res = {}
	for k,v in pairs(t) do
		if type(v) == 'table' then
		v = myutil:deepcopy(v)
	end
	res[k] = v
	end
	setmetatable(res,mt)
	return res
end