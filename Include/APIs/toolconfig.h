
/*

注意：次库的接口不适合高频繁调用，如果需要，需保存到缓存后使用。

*/



#ifndef __TOOL_API_H__
#define __TOOL_API_H__


#ifdef __cplusplus
extern "C" {
#endif

/*
[Config]
Language=
Default Language=
Audio Input Number=
Alarm Input Number=
Alarm Output Number=
Playback Limit Number=
Max Encode=
Display Type=
Video Type=
VgaResolution=
VgaHue=
VgaBrightness=
VgaContrast=
VgaSaturation=
TvTop=
TvBottom=
TvLeft=
TvRight=
TvHue=
TvBrightness=
TvContrast=
TvSaturation=
HDMI=
ModeConfig=
ProtocolConfig=
VvnatServerAddr=
VvnatServerPort=
Mode%d_ability=
Mode%d_netchannel=
Mode%d_localchannel=
FROND_TYPE=
DHProtocol=
DDNS=
Mode%d_MainStreamSplite=
ADHue=
ADBrightness=
ADContrast=
ADSaturation=
Eth%dHostIp=
Eth%dNetMark=
Eth%dGateWay=
PrimaryDns=
SecondaryDns=
Wlen0HostIp= 					
Wlen0NetMark=				
Wlen0GateWay=				
*/
#define TOOL_KEY_LANGUAGE					"Language"
#define TOOL_KEY_DEFAULT_LANGUAGE			"Default Language"
#define TOOL_KEY_AUDIO_INPUT_NUMBER			"Audio Input Number"
#define TOOL_KEY_ALARM_INPUT_NUMBER			"Alarm Input Number"
#define TOOL_KEY_ALARM_OUTPUT_NUMBER		"Alarm Output Number"
#define TOOL_KEY_PLAYBACK_LIMIT_NUMBER		"Playback Limit Number"
#define TOOL_KEY_MAX_ENCODE					"Max Encode"
#define TOOL_KEY_DISPLAY_TYPE				"Display Type"
#define TOOL_KEY_VIDEO_TYPE					"Video Type"
#define TOOL_KEY_VGA_RESOLUTION				"VgaResolution"
#define TOOL_KEY_VGA_HUE					"VgaHue"
#define TOOL_KEY_VGA_BRIGHTNESS				"VgaBrightness"
#define TOOL_KEY_VGA_CONTRAST				"VgaContrast"
#define TOOL_KEY_VGA_SATURATION				"VgaSaturation"
#define TOOL_KEY_TV_TOP						"TvTop"
#define TOOL_KEY_TV_BOTTOM					"TvBottom"
#define TOOL_KEY_TV_LEFT					"TvLeft"
#define TOOL_KEY_TV_RIGHT					"TvRight"
#define TOOL_KEY_TV_HUE						"TvHue"
#define TOOL_KEY_TV_BRIGHTNESS				"TvBrightness"
#define TOOL_KEY_TV_CONTRAST				"TvContrast"
#define TOOL_KEY_TV_SATURATION				"TvSaturation"
#define TOOL_KEY_HDMI						"HDMI"
#define TOOL_KEY_MODE_CONFIG				"ModeConfig"
#define TOOL_KEY_PROTOCOL_CONFIG			"ProtocolConfig"
#define TOOL_KEY_VVNAT_SERVER_ADDR			"VvnatServerAddr"
#define TOOL_KEY_VVNET_SERVER_PORT			"VvnatServerPort"
#define TOOL_KEY_MODE_COUNT					"ModeCount"
#define TOOL_KEY_MODE_ABILITY				"Mode%d_ability"
#define TOOL_KEY_MODE_NET_CHANNEL			"Mode%d_netchannel"
#define TOOL_KEY_MODE_LOCAL_CHANNEL			"Mode%d_localchannel"
#define TOOL_KEY_FRONDTYPE					"FROND_TYPE"
#define TOOL_KEY_DHPROTOCOL					"DHProtocol"
#define TOOL_KEY_DDNS_CONFIG				"DDNS"
#define TOOL_KEY_MAINSTREAM_SPLITE			"Mode%d_MainStreamSplite"
#define TOOL_KEY_AD_HUE						"ADHue"
#define TOOL_KEY_AD_BRIGHTNESS				"ADBrightness"
#define TOOL_KEY_AD_CONTRAST				"ADContrast"
#define TOOL_KEY_AD_SATURATION				"ADSaturation"
#define TOOL_KEY_AD_SHARPNESS				"ADSharpness"
#define TOOL_KEY_AD_COLORMODE				"ADColorMode"

#define TOOL_KEY_ETH_HOST_IP				"Eth%dHostIp"
#define TOOL_KEY_ETH_NET_MARK				"Eth%dNetMark"
#define TOOL_KEY_ETH_GATE_WAY				"Eth%dGateWay"
#define	TOOL_KEY_PRIMARY_DNS				"PrimaryDns"
#define TOOL_KEY_SECONDARY_DNS				"SecondaryDns"
#define TOOL_KEY_WLEN0_HOST_IP				"Wlen0HostIp"
#define TOOL_KEY_WLEN0_NET_MARK				"Wlen0NetMark"
#define TOOL_KEY_WLEN0_GATE_WAY				"Wlen0GateWay"
#define TOOL_KEY_UPNP_OPEN					"UpnpOpen"
#ifdef GOOK_LIN
#define TOOL_KEY_GOOLINK_UUID               "UUID"        //add cwl 12-17
#endif

//使用某种协议登入前端设备的默认用户名和密码
#define TOOL_KEY_PROTOCOL_USENAME			"Protocol%dUserName"
#define TOOL_KEY_PROTOCOL_PASSWORD			"Protocol%dPassWord"

////////////////////////////////////////////////////////////////////////////////////////////////////////
//通用接口

/*
返回值：文件不存在返回-1，存在返回0
*/
int ToolConfigReserved();

/*
Ex接口给TOOL_KEY_MODE_ABILITY有%d的KEY使用的
返回值：错误返回""
*/
const char* ToolGetStringValue(const char* key);
const char* ToolGetStringValueEx(const char* key, int index);

/*
返回值：错误返回-1
*/
int ToolGetIntValue(const char* key);
int ToolGetIntValueEx(const char* key, int index);



/////////////////////////////////////////////////////////////////////////////////////////////////////////
//特定接口

/*
将以逗号分隔的数值字符串，转换成int数组。两个分隔符间不是数值的话，会被遗弃。
result：推荐空间为64
返回值：返回数组数量
*/
int ToolStringArrToIntArr(const char* arr_value, int* result);



#ifdef __cplusplus
}
#endif

#endif //__TOOL_API_H__


