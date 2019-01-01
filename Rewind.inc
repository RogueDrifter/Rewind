// Rewind include by RogueDrifter 2018/12/24 
#include <a_samp>

#if defined r_RewindTime_included_
	#endinput
#endif

#define r_RewindTime_included_
#define PLAYER_SEAT_DRIVER 0
#define PLAYER_SEAT_FRONT_PASSENGER 1
#define MAX_REWIND_TDRAW 12

enum PLAYER_REWIND_DATA
{
	Float:RPosX[4],
	Float:RPosY[4],
	Float:RPosZ[4],

	Float:RewindHP,
	Float:RewindArmour,

	ROldState[4],

	RewindVehicle[4],
	RewindPhases,
	RewindTicks,

	bool:ResyncOn,
	bool:RewindReroll,
	bool:BRewind,
	bool:PRewind,

	Text3D:RewindLabel,

	DotsSync,
	RewindSync,
	HandOfTime
}

enum WORLD_REWIND_DATA
{
	WorldTimeHarmony
}

static 
	Rewind_Player[MAX_PLAYERS][PLAYER_REWIND_DATA],
	Rewind_World[WORLD_REWIND_DATA],
	Text:Rewind_TD[14];

#if defined OnPlayerRewindForPlayer
	forward OnPlayerRewindForPlayer(playerid, forplayerid);
#endif

#if defined OnPlayerStartRewind
	forward OnPlayerStartRewind(playerid);
#endif

#if defined OnPlayerFinishRewind
	forward OnPlayerFinishRewind(playerid);
#endif

#if defined RTC_OnPlayerConnect
	forward RTC_OnPlayerConnect(playerid);
#endif

#if defined FILTERSCRIPT

	#if defined RTC_OnFilterScriptExit
		forward RTC_OnFilterScriptExit();
	#endif

	#if defined RTC_OnFilterScriptInit
		forward RTC_OnFilterScriptInit();
	#endif

#else

	#if defined RTC_OnGameModeExit
		forward RTC_OnGameModeExit();
	#endif

	#if defined RTC_OnGameModeInit
		forward RTC_OnGameModeInit();
	#endif

#endif

#if defined RTC_OnPlayerUpdate
	forward RTC_OnPlayerUpdate(playerid);
#endif

#if defined RTC_OnPlayerDisconnect
	forward RTC_OnPlayerDisconnect(playerid, reason);
#endif

#if defined RTC_OnPlayerStateChange
	forward RTC_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

forward OnPlayerStartRewindEx(playerid);
forward OnPlayerFinishRewindEx(playerid);
forward OnPlayerRewindForPlayerEx(playerid, forplayerid);
forward SyncTimeMachine();
forward HideThisEffect(playerid, EffectType);
forward SetPlayersCamBehind(playerid);
forward RerollTape(playerid);
forward SyncMachinePower(playerid);
forward StartSavingRewindData(playerid);
forward StartSyncingDots(playerid);

#if !defined FILTERSCRIPT

	public OnGameModeInit()
	{
		Rewind_World[WorldTimeHarmony] = SetTimer("SyncTimeMachine", 3*60*1000, true);

		Rewind_TD[0] = TextDrawCreate(601.350158, 362.666931, "Bottom");//Bar 1
		TextDrawLetterSize(Rewind_TD[0], 0.505598, 1.614933);
		TextDrawTextSize(Rewind_TD[0], 620.050659, 0.000000);
		TextDrawAlignment(Rewind_TD[0], 1);
		TextDrawColor(Rewind_TD[0], -1768516096);
		TextDrawUseBox(Rewind_TD[0], 1);
		TextDrawBoxColor(Rewind_TD[0], -1140871557);
		TextDrawSetShadow(Rewind_TD[0], 0);
		TextDrawSetOutline(Rewind_TD[0], 0);
		TextDrawBackgroundColor(Rewind_TD[0], 1690867235);
		TextDrawFont(Rewind_TD[0], 3);
		TextDrawSetProportional(Rewind_TD[0], 1);

		Rewind_TD[1] = TextDrawCreate(601.049987, 341.760192, "Middle");//Bar 2
		TextDrawLetterSize(Rewind_TD[1], 0.400000, 1.600000);
		TextDrawTextSize(Rewind_TD[1], 621.000000, 0.000000);
		TextDrawAlignment(Rewind_TD[1], 1);
		TextDrawColor(Rewind_TD[1], -1768516096);
		TextDrawUseBox(Rewind_TD[1], 1);
		TextDrawBoxColor(Rewind_TD[1], -1140871557);
		TextDrawSetShadow(Rewind_TD[1], 0);
		TextDrawSetOutline(Rewind_TD[1], 0);
		TextDrawBackgroundColor(Rewind_TD[1], 1690867235);
		TextDrawFont(Rewind_TD[1], 3);
		TextDrawSetProportional(Rewind_TD[1], 1);

		Rewind_TD[2] = TextDrawCreate(600.950012, 320.458892, "Top");//Bar 3
		TextDrawLetterSize(Rewind_TD[2], 0.400000, 1.600000);
		TextDrawTextSize(Rewind_TD[2], 621.379394, 0.000000);
		TextDrawAlignment(Rewind_TD[2], 1);
		TextDrawColor(Rewind_TD[2], -1768516096);
		TextDrawUseBox(Rewind_TD[2], 1);
		TextDrawBoxColor(Rewind_TD[2], -1140871557);
		TextDrawSetShadow(Rewind_TD[2], 0);
		TextDrawSetOutline(Rewind_TD[2], 0);
		TextDrawBackgroundColor(Rewind_TD[2], 1690867235);
		TextDrawFont(Rewind_TD[2], 3);
		TextDrawSetProportional(Rewind_TD[2], 1);

		Rewind_TD[3] = TextDrawCreate(596.400024, 237.786651, "___-------");//Side1
		TextDrawLetterSize(Rewind_TD[3], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[3], 1);
		TextDrawColor(Rewind_TD[3], 16711935);
		TextDrawSetShadow(Rewind_TD[3], 0);
		TextDrawSetOutline(Rewind_TD[3], 0);
		TextDrawBackgroundColor(Rewind_TD[3], 16711935);
		TextDrawFont(Rewind_TD[3], 1);
		TextDrawSetProportional(Rewind_TD[3], 1);

		Rewind_TD[4] = TextDrawCreate(596.400024, 260.188018, "___-------");//Side2
		TextDrawLetterSize(Rewind_TD[4], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[4], 1);
		TextDrawColor(Rewind_TD[4], 16711935);
		TextDrawSetShadow(Rewind_TD[4], 0);
		TextDrawSetOutline(Rewind_TD[4], 0);
		TextDrawBackgroundColor(Rewind_TD[4], 16711935);
		TextDrawFont(Rewind_TD[4], 1);
		TextDrawSetProportional(Rewind_TD[4], 1);

		Rewind_TD[5] = TextDrawCreate(596.400024, 272.788787, "___-------");//Side3
		TextDrawLetterSize(Rewind_TD[5], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[5], 1);
		TextDrawColor(Rewind_TD[5], 16711935);
		TextDrawSetShadow(Rewind_TD[5], 0);
		TextDrawSetOutline(Rewind_TD[5], 0);
		TextDrawBackgroundColor(Rewind_TD[5], 16711935);
		TextDrawFont(Rewind_TD[5], 1);
		TextDrawSetProportional(Rewind_TD[5], 1);

		Rewind_TD[6] = TextDrawCreate(565.599548, 378.822296, "_____----");//Bottom
		TextDrawLetterSize(Rewind_TD[6], 0.418399, 0.255986);
		TextDrawAlignment(Rewind_TD[6], 1);
		TextDrawColor(Rewind_TD[6], 16711935);
		TextDrawSetShadow(Rewind_TD[6], 0);
		TextDrawSetOutline(Rewind_TD[6], 1);
		TextDrawBackgroundColor(Rewind_TD[6], 16711935);
		TextDrawFont(Rewind_TD[6], 1);
		TextDrawSetProportional(Rewind_TD[6], 1);

		Rewind_TD[7] = TextDrawCreate(622.793579, 273.488830, "___-------");//Side4
		TextDrawLetterSize(Rewind_TD[7], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[7], 1);
		TextDrawColor(Rewind_TD[7], 16711935);
		TextDrawSetShadow(Rewind_TD[7], 0);
		TextDrawSetOutline(Rewind_TD[7], 0);
		TextDrawBackgroundColor(Rewind_TD[7], 16711935);
		TextDrawFont(Rewind_TD[7], 1);
		TextDrawSetProportional(Rewind_TD[7], 1);

		Rewind_TD[8] = TextDrawCreate(622.793579, 253.187591, "___-------");//Side5
		TextDrawLetterSize(Rewind_TD[8], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[8], 1);
		TextDrawColor(Rewind_TD[8], 16711935);
		TextDrawSetShadow(Rewind_TD[8], 0);
		TextDrawSetOutline(Rewind_TD[8], 0);
		TextDrawBackgroundColor(Rewind_TD[8], 16711935);
		TextDrawFont(Rewind_TD[8], 1);
		TextDrawSetProportional(Rewind_TD[8], 1);

		Rewind_TD[9] = TextDrawCreate(622.793579, 237.386627, "___-------");//Side6
		TextDrawLetterSize(Rewind_TD[9], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[9], 1);
		TextDrawColor(Rewind_TD[9], 16711935);
		TextDrawSetShadow(Rewind_TD[9], 0);
		TextDrawSetOutline(Rewind_TD[9], 0);
		TextDrawBackgroundColor(Rewind_TD[9], 16711935);
		TextDrawFont(Rewind_TD[9], 1);
		TextDrawSetProportional(Rewind_TD[9], 1);

		Rewind_TD[10] = TextDrawCreate(564.799560, 315.965087, "_____----");//Top
		TextDrawLetterSize(Rewind_TD[10], 0.418399, 0.255986);
		TextDrawAlignment(Rewind_TD[10], 1);
		TextDrawColor(Rewind_TD[10], 16711935);
		TextDrawSetShadow(Rewind_TD[10], 0);
		TextDrawSetOutline(Rewind_TD[10], 1);
		TextDrawBackgroundColor(Rewind_TD[10], 16711935);
		TextDrawFont(Rewind_TD[10], 1);
		TextDrawSetProportional(Rewind_TD[10], 1);

		Rewind_TD[11] = TextDrawCreate(585.199523, 303.493560, "R_e_w_i_n_d");//Rewind
		TextDrawLetterSize(Rewind_TD[11], 0.245598, 1.331199);
		TextDrawAlignment(Rewind_TD[11], 1);
		TextDrawColor(Rewind_TD[11], -2131976966);
		TextDrawSetShadow(Rewind_TD[11], 0);
		TextDrawSetOutline(Rewind_TD[11], 0);
		TextDrawBackgroundColor(Rewind_TD[11], 255);
		TextDrawFont(Rewind_TD[11], 2);
		TextDrawSetProportional(Rewind_TD[11], 1);

		Rewind_TD[12] = TextDrawCreate(593.199462, 378.906951, "Power");//Power
		TextDrawLetterSize(Rewind_TD[12], 0.261599, 1.383466);
		TextDrawAlignment(Rewind_TD[12], 1);
		TextDrawColor(Rewind_TD[12], -2131976966);
		TextDrawSetShadow(Rewind_TD[12], 0);
		TextDrawSetOutline(Rewind_TD[12], 0);
		TextDrawBackgroundColor(Rewind_TD[12], 255);
		TextDrawFont(Rewind_TD[12], 2);
		TextDrawSetProportional(Rewind_TD[12], 1);

		Rewind_TD[13] = TextDrawCreate(6.000008, 1.839957, "WhiteFlash");//Rewind effect #1
		TextDrawLetterSize(Rewind_TD[13], 0.504799, 48.430923);
		TextDrawTextSize(Rewind_TD[13], 634.000000, 0.000000);
		TextDrawAlignment(Rewind_TD[13], 1);
		TextDrawColor(Rewind_TD[13], 00000000);
		TextDrawUseBox(Rewind_TD[13], 1);
		TextDrawBoxColor(Rewind_TD[13], -70);
		TextDrawSetShadow(Rewind_TD[13], 0);
		TextDrawSetOutline(Rewind_TD[13], 0);
		TextDrawBackgroundColor(Rewind_TD[13], -1);
		TextDrawFont(Rewind_TD[13], 1);
		TextDrawSetProportional(Rewind_TD[13], 1);
		#if defined RTC_OnGameModeInit
			return RTC_OnGameModeInit();
		#else
			return 1;
		#endif
	}

	public OnGameModeExit()
	{
		KillTimer(Rewind_World[WorldTimeHarmony]);

		for(new x; x < MAX_REWIND_TDRAW + 1; x++)
		{
			TextDrawDestroy(Rewind_TD[x]);
		}
		#if defined RTC_OnGameModeExit
			return RTC_OnGameModeExit();
		#else
			return 1;
		#endif
	}
#else
	public OnFilterScriptInit()
	{
		Rewind_World[WorldTimeHarmony] = SetTimer("SyncTimeMachine", 3*60*1000, true);

		Rewind_TD[0] = TextDrawCreate(601.350158, 362.666931, "Bottom");//Bar 1
		TextDrawLetterSize(Rewind_TD[0], 0.505598, 1.614933);
		TextDrawTextSize(Rewind_TD[0], 620.050659, 0.000000);
		TextDrawAlignment(Rewind_TD[0], 1);
		TextDrawColor(Rewind_TD[0], -1768516096);
		TextDrawUseBox(Rewind_TD[0], 1);
		TextDrawBoxColor(Rewind_TD[0], -1140871557);
		TextDrawSetShadow(Rewind_TD[0], 0);
		TextDrawSetOutline(Rewind_TD[0], 0);
		TextDrawBackgroundColor(Rewind_TD[0], 1690867235);
		TextDrawFont(Rewind_TD[0], 3);
		TextDrawSetProportional(Rewind_TD[0], 1);

		Rewind_TD[1] = TextDrawCreate(601.049987, 341.760192, "Middle");//Bar 2
		TextDrawLetterSize(Rewind_TD[1], 0.400000, 1.600000);
		TextDrawTextSize(Rewind_TD[1], 621.000000, 0.000000);
		TextDrawAlignment(Rewind_TD[1], 1);
		TextDrawColor(Rewind_TD[1], -1768516096);
		TextDrawUseBox(Rewind_TD[1], 1);
		TextDrawBoxColor(Rewind_TD[1], -1140871557);
		TextDrawSetShadow(Rewind_TD[1], 0);
		TextDrawSetOutline(Rewind_TD[1], 0);
		TextDrawBackgroundColor(Rewind_TD[1], 1690867235);
		TextDrawFont(Rewind_TD[1], 3);
		TextDrawSetProportional(Rewind_TD[1], 1);

		Rewind_TD[2] = TextDrawCreate(600.950012, 320.458892, "Top");//Bar 3
		TextDrawLetterSize(Rewind_TD[2], 0.400000, 1.600000);
		TextDrawTextSize(Rewind_TD[2], 621.379394, 0.000000);
		TextDrawAlignment(Rewind_TD[2], 1);
		TextDrawColor(Rewind_TD[2], -1768516096);
		TextDrawUseBox(Rewind_TD[2], 1);
		TextDrawBoxColor(Rewind_TD[2], -1140871557);
		TextDrawSetShadow(Rewind_TD[2], 0);
		TextDrawSetOutline(Rewind_TD[2], 0);
		TextDrawBackgroundColor(Rewind_TD[2], 1690867235);
		TextDrawFont(Rewind_TD[2], 3);
		TextDrawSetProportional(Rewind_TD[2], 1);

		Rewind_TD[3] = TextDrawCreate(596.400024, 237.786651, "___-------");//Side1
		TextDrawLetterSize(Rewind_TD[3], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[3], 1);
		TextDrawColor(Rewind_TD[3], 16711935);
		TextDrawSetShadow(Rewind_TD[3], 0);
		TextDrawSetOutline(Rewind_TD[3], 0);
		TextDrawBackgroundColor(Rewind_TD[3], 16711935);
		TextDrawFont(Rewind_TD[3], 1);
		TextDrawSetProportional(Rewind_TD[3], 1);

		Rewind_TD[4] = TextDrawCreate(596.400024, 260.188018, "___-------");//Side2
		TextDrawLetterSize(Rewind_TD[4], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[4], 1);
		TextDrawColor(Rewind_TD[4], 16711935);
		TextDrawSetShadow(Rewind_TD[4], 0);
		TextDrawSetOutline(Rewind_TD[4], 0);
		TextDrawBackgroundColor(Rewind_TD[4], 16711935);
		TextDrawFont(Rewind_TD[4], 1);
		TextDrawSetProportional(Rewind_TD[4], 1);

		Rewind_TD[5] = TextDrawCreate(596.400024, 272.788787, "___-------");//Side3
		TextDrawLetterSize(Rewind_TD[5], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[5], 1);
		TextDrawColor(Rewind_TD[5], 16711935);
		TextDrawSetShadow(Rewind_TD[5], 0);
		TextDrawSetOutline(Rewind_TD[5], 0);
		TextDrawBackgroundColor(Rewind_TD[5], 16711935);
		TextDrawFont(Rewind_TD[5], 1);
		TextDrawSetProportional(Rewind_TD[5], 1);

		Rewind_TD[6] = TextDrawCreate(565.599548, 378.822296, "_____----");//Bottom
		TextDrawLetterSize(Rewind_TD[6], 0.418399, 0.255986);
		TextDrawAlignment(Rewind_TD[6], 1);
		TextDrawColor(Rewind_TD[6], 16711935);
		TextDrawSetShadow(Rewind_TD[6], 0);
		TextDrawSetOutline(Rewind_TD[6], 1);
		TextDrawBackgroundColor(Rewind_TD[6], 16711935);
		TextDrawFont(Rewind_TD[6], 1);
		TextDrawSetProportional(Rewind_TD[6], 1);

		Rewind_TD[7] = TextDrawCreate(622.793579, 273.488830, "___-------");//Side4
		TextDrawLetterSize(Rewind_TD[7], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[7], 1);
		TextDrawColor(Rewind_TD[7], 16711935);
		TextDrawSetShadow(Rewind_TD[7], 0);
		TextDrawSetOutline(Rewind_TD[7], 0);
		TextDrawBackgroundColor(Rewind_TD[7], 16711935);
		TextDrawFont(Rewind_TD[7], 1);
		TextDrawSetProportional(Rewind_TD[7], 1);

		Rewind_TD[8] = TextDrawCreate(622.793579, 253.187591, "___-------");//Side5
		TextDrawLetterSize(Rewind_TD[8], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[8], 1);
		TextDrawColor(Rewind_TD[8], 16711935);
		TextDrawSetShadow(Rewind_TD[8], 0);
		TextDrawSetOutline(Rewind_TD[8], 0);
		TextDrawBackgroundColor(Rewind_TD[8], 16711935);
		TextDrawFont(Rewind_TD[8], 1);
		TextDrawSetProportional(Rewind_TD[8], 1);

		Rewind_TD[9] = TextDrawCreate(622.793579, 237.386627, "___-------");//Side6
		TextDrawLetterSize(Rewind_TD[9], 0.012799, 16.055454);
		TextDrawAlignment(Rewind_TD[9], 1);
		TextDrawColor(Rewind_TD[9], 16711935);
		TextDrawSetShadow(Rewind_TD[9], 0);
		TextDrawSetOutline(Rewind_TD[9], 0);
		TextDrawBackgroundColor(Rewind_TD[9], 16711935);
		TextDrawFont(Rewind_TD[9], 1);
		TextDrawSetProportional(Rewind_TD[9], 1);

		Rewind_TD[10] = TextDrawCreate(564.799560, 315.965087, "_____----");//Top
		TextDrawLetterSize(Rewind_TD[10], 0.418399, 0.255986);
		TextDrawAlignment(Rewind_TD[10], 1);
		TextDrawColor(Rewind_TD[10], 16711935);
		TextDrawSetShadow(Rewind_TD[10], 0);
		TextDrawSetOutline(Rewind_TD[10], 1);
		TextDrawBackgroundColor(Rewind_TD[10], 16711935);
		TextDrawFont(Rewind_TD[10], 1);
		TextDrawSetProportional(Rewind_TD[10], 1);

		Rewind_TD[11] = TextDrawCreate(585.199523, 303.493560, "R_e_w_i_n_d");//Rewind
		TextDrawLetterSize(Rewind_TD[11], 0.245598, 1.331199);
		TextDrawAlignment(Rewind_TD[11], 1);
		TextDrawColor(Rewind_TD[11], -2131976966);
		TextDrawSetShadow(Rewind_TD[11], 0);
		TextDrawSetOutline(Rewind_TD[11], 0);
		TextDrawBackgroundColor(Rewind_TD[11], 255);
		TextDrawFont(Rewind_TD[11], 2);
		TextDrawSetProportional(Rewind_TD[11], 1);

		Rewind_TD[12] = TextDrawCreate(593.199462, 378.906951, "Power");//Power
		TextDrawLetterSize(Rewind_TD[12], 0.261599, 1.383466);
		TextDrawAlignment(Rewind_TD[12], 1);
		TextDrawColor(Rewind_TD[12], -2131976966);
		TextDrawSetShadow(Rewind_TD[12], 0);
		TextDrawSetOutline(Rewind_TD[12], 0);
		TextDrawBackgroundColor(Rewind_TD[12], 255);
		TextDrawFont(Rewind_TD[12], 2);
		TextDrawSetProportional(Rewind_TD[12], 1);

		Rewind_TD[13] = TextDrawCreate(6.000008, 1.839957, "WhiteFlash");//Rewind effect #1
		TextDrawLetterSize(Rewind_TD[13], 0.504799, 48.430923);
		TextDrawTextSize(Rewind_TD[13], 634.000000, 0.000000);
		TextDrawAlignment(Rewind_TD[13], 1);
		TextDrawColor(Rewind_TD[13], 00000000);
		TextDrawUseBox(Rewind_TD[13], 1);
		TextDrawBoxColor(Rewind_TD[13], -70);
		TextDrawSetShadow(Rewind_TD[13], 0);
		TextDrawSetOutline(Rewind_TD[13], 0);
		TextDrawBackgroundColor(Rewind_TD[13], -1);
		TextDrawFont(Rewind_TD[13], 1);
		TextDrawSetProportional(Rewind_TD[13], 1);
		#if defined RTC_OnFilterScriptInit
			return RTC_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}

	public OnFilterScriptExit()
	{
		KillTimer(Rewind_World[WorldTimeHarmony]);

		for(new x; x < MAX_REWIND_TDRAW + 1; x++)
		{
			TextDrawDestroy(Rewind_TD[x]);
		}
		#if defined RTC_OnFilterScriptExit
			return RTC_OnFilterScriptExit();
		#else
			return 1;
		#endif
	}

#endif

public OnPlayerConnect(playerid)
{
	Rewind_Player[playerid][HandOfTime] = SetTimerEx("StartSavingRewindData", 4000, 1, "i", playerid);
	Rewind_Player[playerid][DotsSync] = SetTimerEx("StartSyncingDots", 1000, 1, "i", playerid);

	Rewind_Player[playerid][RewindLabel] = 
		Create3DTextLabel("[][][][][][][][][][]" , 0xFFFFFFFF, 0.0, 0.0, 0.0, 25.0, 0, 1);
	Attach3DTextLabelToPlayer(Rewind_Player[playerid][RewindLabel], playerid, 0.0, 0.0, 0.3);

	#if defined RTC_OnPlayerConnect
		return RTC_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerUpdate(playerid)
{
	Rewind_Player[playerid][RewindTicks] = GetTickCount();
	#if defined RTC_OnPlayerUpdate
		return RTC_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
public OnPlayerDisconnect(playerid, reason)
{
	ResetTimeMachine(playerid);

	KillTimer(Rewind_Player[playerid][HandOfTime]);
	Delete3DTextLabel(Rewind_Player[playerid][RewindLabel]);
	Rewind_Player[playerid][RewindLabel] = Text3D:INVALID_3DTEXT_ID;
	
	#if defined RTC_OnPlayerDisconnect
		return RTC_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(!Rewind_Player[playerid][PRewind] && !Rewind_Player[playerid][BRewind])
	{
		Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] = oldstate;
	}

	#if defined RTC_OnPlayerStateChange
		return RTC_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}

static stock RewindPlayerForPlayer(playerid, forplayerid)
{
	if(Rewind_Player[playerid][ResyncOn]) HideTimeMachineSynced(playerid);
	PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
	RerollTape(playerid);
	return OnPlayerRewindForPlayerEx(playerid, forplayerid);
}

static stock RewindWorld(playerid)
{
	#if !defined foreach 
		for(new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if(!IsPlayerConnected(i) || i == playerid || Rewind_Player[i][PRewind] || Rewind_Player[i][BRewind]) continue;
			if(Rewind_Player[i][RewindPhases] == 0) continue;
			if(IsPlayerStreamedIn(i, playerid))
			{
				Rewind_Player[i][BRewind] = true;
				TogglePlayerControllable(i, 0);
				RewindPlayerForPlayer(i, playerid);
			}
		}
	#else
		foreach(new i: Player)
		{
			if(!IsPlayerConnected(i) || i == playerid || Rewind_Player[i][PRewind] || Rewind_Player[i][BRewind]) continue;
			if(Rewind_Player[i][RewindPhases] == 0) continue;
			if(IsPlayerStreamedIn(i, playerid))
			{
				Rewind_Player[i][BRewind] = true;
				TogglePlayerControllable(i, 0);
				RewindPlayerForPlayer(i, playerid);
			}
		}
	#endif
	return 1;
}

static stock ShowPlayerRewindScene(playerid)
{
	for(new x = 3; x < MAX_REWIND_TDRAW + 1; x++)
	{
		TextDrawShowForPlayer(playerid, Rewind_TD[x]);
	}

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 1:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
		}
		
		case 2:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
		}

		case 3:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
		}
	}
	return 1;
}

stock bool:IsPlayerRewinding(playerid) 
	return Rewind_Player[playerid][PRewind];

stock bool:IsPlayerInvolvedInRewind(playerid) 
	return Rewind_Player[playerid][BRewind];
	
stock StartRewinding(playerid)
{
	if(Rewind_Player[playerid][PRewind] || Rewind_Player[playerid][BRewind]) return 0;
	if(Rewind_Player[playerid][RewindPhases] < 1) return 0;
	Rewind_Player[playerid][PRewind] = true;
	if(Rewind_Player[playerid][ResyncOn]) HideTimeMachineSynced(playerid);

	ShowPlayerRewindScene(playerid);
	PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
	TogglePlayerControllable(playerid, 0);
	RewindWorld(playerid);
	RerollTape(playerid);
	return OnPlayerStartRewindEx(playerid);	
}

stock ShowTimeMachineSynced(playerid)
{
	if(Rewind_Player[playerid][ResyncOn]) return 0;

	if(!Rewind_Player[playerid][ResyncOn]) Rewind_Player[playerid][ResyncOn] = true;
	for(new x = 3; x < MAX_REWIND_TDRAW + 1; x++)
	{
		TextDrawShowForPlayer(playerid, Rewind_TD[x]);
	}

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 1:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
		}
		
		case 2:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
		}

		case 3:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
		}
	}
	return Rewind_Player[playerid][RewindSync] = SetTimerEx("SyncMachinePower", 1000, true, "i", playerid);
}

stock HideTimeMachineSynced(playerid)
{
	Rewind_Player[playerid][ResyncOn] = false;
	KillTimer(Rewind_Player[playerid][RewindSync]);
	return HideForPlayerRewindScene(playerid);
}

static HideForPlayerRewindScene(playerid)
{
	for(new x; x < MAX_REWIND_TDRAW + 1; x++)
	{
		TextDrawHideForPlayer(playerid, Rewind_TD[x]);
	}
	return 1;
}

static GetBarDots(playerid)
{
    new Rewind_Dots[64];

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 3:
		{
			Rewind_Dots = "{000066}[][][][][][][][][][]";
		}
		case 2:
		{
			Rewind_Dots = "[][][][]{000066}[][][][][][]";
		}
		case 1:
		{
			Rewind_Dots = "[][][][][][][]{000066}[][][]";
		}
		case 0:
		{
			Rewind_Dots = "[][][][][][][][][][]";
		}
	}
 
    return Rewind_Dots;
}

static SetPlayerPosByPhase(playerid)
{
	new PlayerTempState = GetPlayerState(playerid);
	if(Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] == -1) Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] = PlayerTempState;

	if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) )
	{
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , PLAYER_SEAT_DRIVER);
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
	}
	else if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) ||
	(PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) )
	{
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , PLAYER_SEAT_FRONT_PASSENGER);
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
	}


	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) )
	{
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
	}
	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) || 
	(PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) )
	{
		SetPlayerPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
	}
	return 1;
}

static FinishRewind(playerid)
{
	Rewind_Player[playerid][RewindReroll] = false;
	Rewind_Player[playerid][BRewind] = false;
	Rewind_Player[playerid][PRewind] = false;
	Rewind_Player[playerid][ResyncOn] = false;
	KillTimer(Rewind_Player[playerid][RewindSync]);
	
	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);
	StopAudioStreamForPlayer(playerid);
	HideForPlayerRewindScene(playerid);
	return OnPlayerFinishRewindEx(playerid);
}

static ResetTimeMachine(playerid)
{
	Rewind_Player[playerid][RewindHP] = 0.0;
	Rewind_Player[playerid][RewindArmour] = 0.0;
	Rewind_Player[playerid][RewindPhases] = 0;
	Rewind_Player[playerid][RewindTicks] = 0;
	Rewind_Player[playerid][RewindReroll] = false;
	Rewind_Player[playerid][BRewind] = false;
	Rewind_Player[playerid][PRewind] = false;
	Rewind_Player[playerid][ResyncOn] = false;
	KillTimer(Rewind_Player[playerid][RewindSync]);
	
	for(new x; x < 4 + 1; x++)
	{
		Rewind_Player[playerid][RPosX][x] = 0.0;
		Rewind_Player[playerid][RPosY][x] = 0.0;
		Rewind_Player[playerid][RPosZ][x] = 0.0;

		Rewind_Player[playerid][RewindVehicle][x] = 0;
		Rewind_Player[playerid][ROldState][x] = 0;
	}
	
	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);
	StopAudioStreamForPlayer(playerid);
	HideForPlayerRewindScene(playerid);
	return 1;
}

public OnPlayerRewindForPlayerEx(playerid, forplayerid)
{
	#if defined OnPlayerRewindForPlayer
		return OnPlayerRewindForPlayer(playerid, forplayerid);
	#else
		return 1;
	#endif
}

public OnPlayerStartRewindEx(playerid)
{
	#if defined OnPlayerStartRewind
		return OnPlayerStartRewind(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerFinishRewindEx(playerid)
{
	#if defined OnPlayerFinishRewind
		return OnPlayerFinishRewind(playerid);
	#else
		return 1;
	#endif
}

public StartSyncingDots(playerid)
{
	Update3DTextLabelText(Rewind_Player[playerid][RewindLabel], GetPlayerColor(playerid) >>> 8, GetBarDots(playerid));
	return 1;
}

public SyncTimeMachine()
{
	#if !defined foreach 
		for(new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			KillTimer(Rewind_Player[i][HandOfTime]);
			Rewind_Player[i][HandOfTime] = SetTimerEx("StartSavingRewindData", 4000, 1, "i", i);
		}
	#else
		foreach(new i: Player)
		{
			KillTimer(Rewind_Player[i][HandOfTime]);
			Rewind_Player[i][HandOfTime] = SetTimerEx("StartSavingRewindData", 4000, 1, "i", i);
		}
	#endif
	return 1;
}

public HideThisEffect(playerid, EffectType)
{
	switch(EffectType)
	{
		case 0:
		{
			TextDrawHideForPlayer(playerid, Rewind_TD[13]);
		}
	}
	return 1;
}

public SetPlayersCamBehind(playerid)
{
	return SetCameraBehindPlayer(playerid);
}

public RerollTape(playerid)
{
	TextDrawShowForPlayer(playerid, Rewind_TD[13]);
	SetTimerEx("HideThisEffect", 100, false, "ii", playerid, 0);

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 0:
		{
			SetPlayerPosByPhase(playerid);
			FinishRewind(playerid);
			Rewind_Player[playerid][RewindPhases] = 3;
			Rewind_Player[playerid][RewindReroll] = true;
			SetPlayerHealth(playerid, Rewind_Player[playerid][RewindHP]);
			SetPlayerArmour(playerid, Rewind_Player[playerid][RewindArmour]);
			return 1;
		}

		case 1, 2, 3:
		{
			switch(Rewind_Player[playerid][RewindPhases])
			{
				case 1:
				{
					TextDrawHideForPlayer(playerid, Rewind_TD[0]);
				}
				case 2:
				{
					TextDrawHideForPlayer(playerid, Rewind_TD[1]);
				}
				case 3:
				{
					TextDrawHideForPlayer(playerid, Rewind_TD[2]);
				}
			}

			if(Rewind_Player[playerid][RewindPhases] == 3)
			{
				InterpolateCameraPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]+10, 450);
				InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] + 1, Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] - 1, Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] + 1, 450);
				SetTimerEx("SetPlayersCamBehind", 700, false, "i", playerid);
			}
			else
			{
				InterpolateCameraPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] +1 ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]+10, 450);
				InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] +1 ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] + 1, Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] - 1, Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] + 1, 450);
				SetTimerEx("SetPlayersCamBehind", 700, false, "i", playerid);
			}
			SetPlayerPosByPhase(playerid);
			Rewind_Player[playerid][RewindPhases]--;
			SetTimerEx("RerollTape", 2000, false, "i", playerid);
			return 1;
		}
	}
	return 1;
}

public SyncMachinePower(playerid)
{
	for(new x; x < 3; x++)
	{
		TextDrawHideForPlayer(playerid, Rewind_TD[x]);
	}

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 1:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
		}
		
		case 2:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
		}

		case 3:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
		}
	}
	return 1;
}

public StartSavingRewindData(playerid)
{
	new PlayerVehicleID = GetPlayerVehicleID(playerid),
	 	PlayerCurrentState = GetPlayerState(playerid);

	if(Rewind_Player[playerid][PRewind] || Rewind_Player[playerid][BRewind]) return 0;
	if(GetTickCount() > (Rewind_Player[playerid][RewindTicks]+6000)) return 0;
	if(PlayerCurrentState == PLAYER_STATE_NONE || PlayerCurrentState == PLAYER_STATE_WASTED 
	|| PlayerCurrentState == PLAYER_STATE_SPECTATING || PlayerCurrentState == PLAYER_STATE_EXIT_VEHICLE
	|| PlayerCurrentState == PLAYER_STATE_ENTER_VEHICLE_DRIVER 
	|| PlayerCurrentState == PLAYER_STATE_ENTER_VEHICLE_PASSENGER) return 0;

	if(Rewind_Player[playerid][RewindPhases] == 3)
	{
		if(Rewind_Player[playerid][RewindReroll])
		{
			Rewind_Player[playerid][RewindReroll] = false;
			Rewind_Player[playerid][ROldState][0] = 
			Rewind_Player[playerid][ROldState][1] = 
			Rewind_Player[playerid][ROldState][2] = 
			Rewind_Player[playerid][ROldState][3] = -1;
			Rewind_Player[playerid][RewindPhases] = 0;
		}
		else Rewind_Player[playerid][RewindReroll] = true;
	}
	
	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 0:
		{
			GetPlayerHealth(playerid, Rewind_Player[playerid][RewindHP]);
			GetPlayerArmour(playerid, Rewind_Player[playerid][RewindArmour]);
			Rewind_Player[playerid][RewindPhases]++;
			switch(PlayerCurrentState)
			{
				case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
				{
					GetVehiclePos(PlayerVehicleID, Rewind_Player[playerid][RPosX][0], Rewind_Player[playerid][RPosY][0], Rewind_Player[playerid][RPosZ][0]);
					Rewind_Player[playerid][RewindVehicle][0] = PlayerVehicleID;
				}
				case PLAYER_STATE_ONFOOT:
				{
					GetPlayerPos(playerid, Rewind_Player[playerid][RPosX][0], Rewind_Player[playerid][RPosY][0], Rewind_Player[playerid][RPosZ][0]);
				}
			}
			return 1;
		}

		case 1:
		{
			Rewind_Player[playerid][RewindPhases]++;
			switch(PlayerCurrentState)
			{
				case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
				{
					GetVehiclePos(PlayerVehicleID, Rewind_Player[playerid][RPosX][1], Rewind_Player[playerid][RPosY][1], Rewind_Player[playerid][RPosZ][1]);
					Rewind_Player[playerid][RewindVehicle][1] = PlayerVehicleID;
				}
				case PLAYER_STATE_ONFOOT:
				{
					GetPlayerPos(playerid, Rewind_Player[playerid][RPosX][1], Rewind_Player[playerid][RPosY][1], Rewind_Player[playerid][RPosZ][1]);
				}
			}
			return 1;
		}

		case 2:
		{
			Rewind_Player[playerid][RewindPhases]++;
			switch(PlayerCurrentState)
			{
				case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
				{
					GetVehiclePos(PlayerVehicleID, Rewind_Player[playerid][RPosX][2], Rewind_Player[playerid][RPosY][2], Rewind_Player[playerid][RPosZ][2]);
					Rewind_Player[playerid][RewindVehicle][2] = PlayerVehicleID;
				}
				case PLAYER_STATE_ONFOOT:
				{
					GetPlayerPos(playerid, Rewind_Player[playerid][RPosX][2], Rewind_Player[playerid][RPosY][2], Rewind_Player[playerid][RPosZ][2]);
				}
			}
			return 1;
		}

		case 3:
		{
			switch(PlayerCurrentState)
			{
				case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
				{
					GetVehiclePos(PlayerVehicleID, Rewind_Player[playerid][RPosX][3], Rewind_Player[playerid][RPosY][3], Rewind_Player[playerid][RPosZ][3]);
					Rewind_Player[playerid][RewindVehicle][3] = PlayerVehicleID;
				}
				case PLAYER_STATE_ONFOOT:
				{
					GetPlayerPos(playerid, Rewind_Player[playerid][RPosX][3], Rewind_Player[playerid][RPosY][3], Rewind_Player[playerid][RPosZ][3]);
				}
			}
		}
	}
	return 1;
}

//Hookings:
#if defined _ALS_OnPlayerDisconnect
  #undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect RTC_OnPlayerDisconnect

#if defined _ALS_OnPlayerConnect
  #undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect RTC_OnPlayerConnect

#if defined _ALS_OnPlayerStateChange
  #undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange RTC_OnPlayerStateChange

#if defined _ALS_OnPlayerUpdate
  #undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate RTC_OnPlayerUpdate

#if defined FILTERSCRIPT

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit RTC_OnFilterScriptInit

	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif

	#define OnFilterScriptExit RTC_OnFilterScriptExit

#else

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif

	#define OnGameModeInit RTC_OnGameModeInit

	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif

	#define OnGameModeExit RTC_OnGameModeExit

#endif
