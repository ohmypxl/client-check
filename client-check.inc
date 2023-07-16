/*
 * Client Check.inc
 * Version: 2.0
 * Author: pxl
 *
 * About:
 * This file based on Android Check.inc that got renamed into Client Check.inc
 * Just in case if someone manages to play samp on a fucking calculator or smth. 
 *
 * Note:
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Credits:
 * Y-Less - For the rconfix/OnClientCheckResponse code
 * ff-agus44 - For the fork fork
 * Fairuz - For the opcode/initial script
*/

#if defined _INC_y_android
	#error "You're already using y_android bro!"
#endif

#if defined _INC_client_check_defined
	#endinput
#endif

#define _INC_client_check_defined

#if !defined CLIENT_CHECK_TIME
	#define CLIENT_CHECK_TIME 5_000
#endif

enum ClientStatus {
	USING_CLIENT_UNKNOWN = -1,
	USING_CLIENT_ANDROID = 0,
	USING_CLIENT_PC,
}

static
	Client_g_sPlayerCheckTime[MAX_PLAYERS] = {0, ...},
	ClientStatus:Client_g_sPlayerClientStatus[MAX_PLAYERS] = {USING_CLIENT_UNKNOWN, ...};

stock CheckPlayerClient(playerid)
{
	// Reset each check to avoid false positive
	Client_g_sPlayerClientStatus[playerid] = USING_CLIENT_UNKNOWN;

	SendClientCheck(playerid, 0x48, 0, 0, 2);
	Client_g_sPlayerCheckTime[playerid] = SetTimerEx(#Client_InternalTimer, CLIENT_CHECK_TIME, true, "i", playerid);
	return 1;
}

stock bool:IsPlayerUsingAndroid(playerid)
{
	return (IsPlayerConnected(playerid) && Client_g_sPlayerClientStatus[playerid] == USING_CLIENT_ANDROID);
}

stock bool:IsPlayerUsingPC(playerid)
{
	return (IsPlayerConnected(playerid) && Client_g_sPlayerClientStatus[playerid] == USING_CLIENT_PC);
}

stock ClientStatus:GetPlayerClientStatus(playerid)
{
	if (IsPlayerConnected(playerid))
	{
		return USING_CLIENT_UNKNOWN;
	}

	return (Client_g_sPlayerClientStatus[playerid]);
}

/*
	Internal Function/Callback
*/

#if !defined CLIENT_NO_AUTO_CHECK
public OnPlayerConnect(playerid)
{
	CheckPlayerClient(playerid);

    #if defined Pxl_OnPlayerConnect
        return Pxl_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

public OnPlayerDisconnect(playerid, reason)
{
	if (Client_g_sPlayerCheckTime[playerid])
	{
		KillTimer(Client_g_sPlayerCheckTime[playerid]);
		Client_g_sPlayerCheckTime[playerid] = 0;
	}

	Client_g_sPlayerClientStatus[playerid] = USING_CLIENT_UNKNOWN;

	#if defined Pxl_OnPlayerDisconnect
		return Pxl_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

// Implement simple IsPlayerPaused check if not available
// For fixing the false positive thingy.
#if !defined IsPlayerPaused
static
	Client_g_sPlayerLastTick[MAX_PLAYERS];

public OnPlayerUpdate(playerid) {
    Client_g_sPlayerLastTick[playerid] = GetTickCount();
	#if defined Pxl_OnPlayerUpdate
		return Pxl_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}

stock bool:Pxl_IsPlrPaused(playerid) {
    return (GetTickCount() - Client_g_sPlayerLastTick[playerid] > 1500);
}

forward Client_InternalTimer(playerid);
public Client_InternalTimer(playerid)
{
	if (IsPlayerPaused(playerid))
	{
		// Keep refiring if AFK!
		CheckPlayerClient(playerid);
		return;
	}

    if (Client_g_sPlayerClientStatus[playerid] != USING_CLIENT_ANDROID)
    {
        Client_g_sPlayerClientStatus[playerid] = USING_CLIENT_ANDROID;
        CallLocalFunction(#OnClientChecked, "ii", playerid, 0);
    }
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect( Pxl_OnPlayerConnect(
#if defined Pxl_OnPlayerConnect
    forward Pxl_OnPlayerConnect(playerid);
#endif
#endif

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate Pxl_OnPlayerUpdate
#if defined Pxl_OnPlayerUpdate
	forward Pxl_OnPlayerUpdate(playerid);
#endif

#define IsPlayerPaused Pxl_IsPlrPaused
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect( Pxl_OnPlayerDisconnect(
#if defined Pxl_OnPlayerDisconnect
	forward Pxl_OnPlayerDisconnect(playerid, reason);
#endif

/*
	Internal Standalone Include
*/

#if !defined _INC_y_rconfix && !defined _INC_SAMP_Community_fixes || !defined _inc_fixes
static LoadClientCheckFix()
{
	// Test if the fix has already been loaded.
	setproperty(27, #FIXES_OnClientCheckResponse, 0);
	SendRconCommand(#FIXES_OnClientCheckResponse);

	if (getproperty(27, #FIXES_OnClientCheckResponse) == 27)
	{
		return;
	}
	
	// If it is not exists, create the file first.
	if (!fexist("callbackfix.amx"))
	{
		static
			lRCFixCode[] = {
				0x00000120, 0x0808F1E0, 0x00080014, 0x000000AC, 0x00000190, 0x000001C0, 0x000041C0, 0xFFFFFFFF,
				0x00000038, 0x00000048, 0x00000060, 0x00000060, 0x00000060, 0x00000060, 0x00000094, 0x00000062,
				0x00000008, 0x00000078, 0x00000000, 0x00000086, 0x00000000, 0x0000008D, 0x00000000, 0x00000099,
				0x6E4F001F, 0x65696C43, 0x6843746E, 0x526B6365, 0x6F707365, 0x0065736E, 0x63526E4F, 0x6F436E6F,
				0x6E616D6D, 0x74730064, 0x706D6372, 0x74657300, 0x706F7270, 0x79747265, 0x6C614300, 0x6D65526C,
				0x4665746F, 0x74636E75, 0x006E6F69, 0x2E007880, 0xFFFF8727, 0x01277FFF, 0x0C290027, 0x7B801027,
				0x36142C00, 0x1C270C81, 0x00271B27, 0x10271B27, 0x2C017B80, 0x30548014, 0x2E305980, 0x81180581,
				0x05811405, 0x0C058110, 0x00272827, 0x7B801827, 0x301C2C02, 0xB0A5B284, 0xFD9A8545, 0x9B846E9E,
				0x8665D2B1, 0x6886D1F3, 0xD68DAB86, 0xCDAB8652, 0xF3866FE0, 0x0000CACD, 0xCB860000, 0x0069D2A5
			};
		new
			File:fs = fopen("callbackfix.amx", io_write);

		if (fs)
		{
			fblockwrite(fs, lRCFixCode);
			fclose(fs);
		}
	}

	/* 
		after that we check if the caller is actually exist, then send the rcon command
		the reason why the original code using this instead of else is because when the file is not exists
		using else here will not be called.
	*/
	if (fexist("callbackfix.amx"))
	{
		SendRconCommand("loadfs ../scriptfiles/callbackfix");
		state OnClientCheckResponse : OnClientCheckResponse_GM;
	}
	else
	{
		printf("[Android Check]: Cannot load callbackfix.amx");
		printf("NOTE: Make sure you have the \"scriptfiles\" folder");
		SendRconCommand("exit");
	}
}

public OnGameModeInit()
{
	LoadClientCheckFix();

	#if defined Pxl_OnGameModeInit
		return Pxl_OnGameModeInit();
	#else
		return 1;
	#endif
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit( Pxl_OnGameModeInit(
#if defined Pxl_OnGameModeInit
	forward Pxl_OnGameModeInit();
#endif

// Maybe if all of the three include isn't available this might do! YES!
// Because open.mp fixes this behavior so i don't think we ever need the state tho
#if !defined _INC_open_mp
forward OnClientCheckResponse(playerid, actionid, memaddr, retndata);
public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	// Invoke the global callback (assuming we are in an FS).
	CallRemoteFunction(#FIXES_OnClientCheckResponse, "iiii", playerid, actionid, memaddr, retndata);
	#if defined Pxl_OnClientCheckResponse
		return Pxl_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
	#else
		return 1;
	#endif
}

forward FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata);

public FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata) <OnClientCheckResponse : OnClientCheckResponse_FS>
{
	return 0;
}

public FIXES_OnClientCheckResponse(playerid, actionid, memaddr, retndata) <OnClientCheckResponse : OnClientCheckResponse_GM>
{
	#if defined Pxl_OnClientCheckResponse
		return Pxl_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
	#else
		return 0;
	#endif
}

#if defined _ALS_OnClientCheckResponse
	#error _ALS_OnClientCheckResponse defined
#endif
#define _ALS_OnClientCheckResponse

#define OnClientCheckResponse( Pxl_OnClientCheckResponse(

#if defined Pxl_OnClientCheckResponse
	forward Pxl_OnClientCheckResponse(playerid, actionid, memaddr, retndata);
#endif
#endif
#endif

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	if (actionid == 0x48) 
	{
		if (Client_g_sPlayerCheckTime[playerid])
		{
			KillTimer(Client_g_sPlayerCheckTime[playerid]);
			Client_g_sPlayerCheckTime[playerid] = 0;

			if (Client_g_sPlayerClientStatus[playerid] != USING_CLIENT_PC)
			{
				Client_g_sPlayerClientStatus[playerid] = USING_CLIENT_PC;
				CallLocalFunction(#OnPlayerClientChecked, "ii", playerid, 1);
				return 1;
			}
		}
	}
	return 0;
}

// Complex Algorithm Thingy that enables IsUsing... to be used outside of the uhhh
// Basically it will do pointless job tbh
#define IsUsingAndroid(%0) (_:PXLOVRLD1:PXLOVRLD2:PXLOVRLD3:CheckDevice__(ANDROID,%0))
#define IsUsingPC(%0) (_:PXLOVRLD1:PXLOVRLD2:PXLOVRLD3:CheckDevice__(PC,%0))
#define PXLOVRLD1:PXLOVRLD2:PXLOVRLD3:CheckDevice__(%1,status) (status == USING_CLIENT_%1)
#define PXLOVRLD2:PXLOVRLD3:CheckDevice__(ANDROID,%2) IsPlayerUsingAndroid(%2)
#define PXLOVRLD3:CheckDevice__(PC,%1) IsPlayerUsingPC(%1)

// Define forward if defined
#if defined OnPlayerClientChecked
	forward OnPlayerClientChecked(playerid, ClientStatus:status);
#endif

// Undef Internal Function
#if defined Pxl_IsPlrPaused
	#undef IsPlayerPaused
#endif
