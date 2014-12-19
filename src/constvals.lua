require "channelconfigdata"

Currency_type = 
{
    [1] = "CNY", -- RMB
    [2] = "USD", -- US Dollar
    [3] = "THB", -- Thai Baht
    [4] = "EUR", -- Euro
    [4] = "GBP", -- British Pound
    [4] = "INR", -- Indian Rupee
    [4] = "AUD", -- Australian Dollar
    [4] = "CAD", -- Canadian Dollar
    [4] = "AED", -- Emirati Dirham
    [4] = "MYR", -- Malaysian Ringgit
    [4] = "CHF", -- Swiss Franc
}

deviceLanguageMap =
{
	[0] = "Eng",
	[1] = "Chs",
	[2] = "Fren",
	[3] = "Ita",
	[4] = "Ger",
	[5] = "Span",
	[6] = "Russ",
	[7] = "Kore",
	[8] = "Jap",
	[9] = "Hung",
	[10] = "Port",
	[11] = "Arab"
}

guildFightDefenderStatus =
{
	Normal = 0,
	Fighting = 1,
	Occupied = 2,
}

deviceTypeMap =
{
	[0] = "win",
	[1] = "linux",
	[2] = "mac",
	[3] = "android",
	[4] = "iphone",
	[5] = "ipad",
	[6] = "blackberry",
	[7] = "nacl",
	[8] = "ems",
	[9] = "tizen"
}

itemType = 
{
	card                   = 1,
	equipment              = 2,
	material               = 3,
	silver                 = 4,
	GuildContribution      = 6,
	fragment               = 7,
	pskill                 = 8,
	GuildTotalContribution = 9,
	gold                   = 10,
	reputation             = 11,
	honour                 = 12,
	guildexp               = 13,
	friendship             = 14,
	spirit                 = 15,
	energy                 = 16,
	exp                    = 17,
	points                 = 18,
	TotalGold              = 19,
	gift                   = 20,
	RandomAward            = 100
}

itemIcon = 
{
	[itemType.silver]                 = "2dui_zhujiemian_yinbi.png",
	-- [itemType.GuildContribution]      = 6,
	-- [itemType.GuildTotalContribution] = 9,
	[itemType.gold]                   = "2dui_zhujiemian_beili.png",
	-- [itemType.reputation]             = 11,
	[itemType.honour]                 = "2dui_tongyong_rongyu.png",
	-- [itemType.guildexp]               = "2dui_tongyong_cailiao.png",
	[itemType.friendship]             = "2dui_tongyong_youqing.png",
	[itemType.spirit]                 = "2dui_zhujiemian_tili.png",
	[itemType.energy]                 = "2dui_zhujiemian_huoli.png",
	[itemType.exp]                    = "2dui_tongyong_jingyan.png",
	[itemType.points]                 = "2dui_tongyong_jifen.png",
  }

enumUsedCount =
{
	RelationshipSkillCount													= 4,
	PassiveSkillCount														= 3,
	CardAttributeCountAdditional											= 4,
	MainMemberCardCount														= 5,
	AltMemberCardCount														= 3,
	TeamCardCount															= 8,
	CombatCardCount															= 6
}

fontType = 
{
  current = "",
  adventure = "FZDaHei-B02S.TTF",
  normalBtnFont = "ui/dahei.ttf",
}

function startFontAd()
	if getChannelConfigData():getLanguage() == "cht" then 
		fontType.adventure = ""
  		fontType.normalBtnFont = ""
	end
end

local fontSize = 
{
	["chs"] 	= {title = 24, description = 22, },
	["thai"] 	= {title = 24, description = 22, },
	["cht"] 	= {title = 24, description = 22, },
	["en"] 		= {title = 21, description = 17, },
}

CommonFontSize = 
{
	title 			= fontSize[getChannelConfigData():getLanguage()].title,
	description 	= fontSize[getChannelConfigData():getLanguage()].description,
}

ItalicDigitFontColor = 
{
	redColor = ccc3(255,103,91),
	greenColor = ccc3(203,255,91)	
}

NormalFontColor = 
{
	whiteColor = ccc3(255,255,255),
	blackColor = ccc3(0,0,0),
	redColor = ccc3(176,0,0),
	greenColor = ccc3(13,161,27),
	blueColor = ccc3(29,115,197),
	purpleColor = ccc3(120,65,190),
	yellowColor = ccc3(203,148,53),
	normalTextColor = ccc3(43,43,43),
	captainTextColor = ccc3(255,253,101),
    aredColor = ccc3(255, 0, 0)
}

QualityFontColor = 
{
  [1] = NormalFontColor.whiteColor,
  [2] = NormalFontColor.whiteColor,
  [3] = NormalFontColor.greenColor,
  [4] = NormalFontColor.blueColor,
  [5] = NormalFontColor.purpleColor,
  [6] = NormalFontColor.yellowColor
}

QualityFontColorARGB = 
{
  [1] = "FFFFFFFF",
  [2] = "FFFFFFFF",
  [3] = "FF0DA11B",
  [4] = "FF1D73C5",
  [5] = "FF7841BE",
  [6] = "FFCB9435"
}