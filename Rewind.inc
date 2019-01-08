// Rewind include by RogueDrifter 2018/12/24 -767758715

#if defined REWIND_MAXIMUM_PHASES
	#if REWIND_MAXIMUM_PHASES > 29 || REWIND_MAXIMUM_PHASES < 5
		#error Make sure REWIND_MAXIMUM_PHASES value is between 5 and 29 (or equal to either)
	#endif
#endif

#include <a_samp>

#if defined r_RewindTime_included_
	#endinput
#endif

#define r_RewindTime_included_
#define PLAYER_SEAT_DRIVER 0
#define PLAYER_SEAT_FRONT_PASSENGER 1
#define MAX_REWIND_TDRAW 12
#define REWIND_PLAYERS_HARMONY 2
#define SYNCEED_REWIND_BUFFER 85

#if !defined REWIND_CHOOSING_DIALOG
	#define REWIND_CHOOSING_DIALOG 32766
#endif

#if !defined REWIND_MAXIMUM_PHASES
	#define REWIND_MAXIMUM_PHASES 5
#endif

enum PLAYER_REWIND_DATA
{
	Float:RSPosX[SYNCEED_REWIND_BUFFER + 1],
	Float:RSPosY[SYNCEED_REWIND_BUFFER + 1],
	Float:RSPosZ[SYNCEED_REWIND_BUFFER + 1],
	Float:RPosX[REWIND_MAXIMUM_PHASES + 1],
	Float:RPosY[REWIND_MAXIMUM_PHASES + 1],
	Float:RPosZ[REWIND_MAXIMUM_PHASES + 1],
	Float:RPosAngle[REWIND_MAXIMUM_PHASES + 1],
	Float:RSPosAngle[SYNCEED_REWIND_BUFFER + 1],

	Float:RewindHP,
	Float:RewindArmour,

	RSOldState[SYNCEED_REWIND_BUFFER + 1],
	ROldState[REWIND_MAXIMUM_PHASES + 1],
	RPInterior[REWIND_MAXIMUM_PHASES + 1],
	RPVWorld[REWIND_MAXIMUM_PHASES + 1],
	RSPInterior[SYNCEED_REWIND_BUFFER + 1],
	RSPVWorld[SYNCEED_REWIND_BUFFER + 1],

	RewindSVehicle[SYNCEED_REWIND_BUFFER + 1],
	RewindVehicle[REWIND_MAXIMUM_PHASES + 1],
	RewindPhases,
	SyncedRewindFlags,

	bool:ResyncOn,
	bool:BRewind,
	bool:PRewind,
	bool:SyncedRewindOn,
	bool:SyncedRewindDouble,
	bool:PrewindWorld,

	bool:PRewindFlash,
	bool:PRewindCamera,
	bool:PRewindAuto,

	Text3D:RewindLabel,

	SyncedRerollTimer,
	SyncedRewindTimer,
	DotsSync,
	RewindSync,
	HandOfTime
}

enum WORLD_REWIND_DATA
{
	WorldTimeHarmony,

	bool:RewindCamera,
	bool:RewindFlash,
	bool:RewindAuto,
	bool:WorldRewind
}

static 
	Rewind_Player[MAX_PLAYERS][PLAYER_REWIND_DATA],
	Rewind_World[WORLD_REWIND_DATA],
	PlayerText:Rewind_PTD[MAX_PLAYERS][3],
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

#if defined OnPlayerFastRewind
	forward OnPlayerFastRewind(playerid);
#endif

#if defined OnPlayerFinishFastRewind
	forward OnPlayerFinishFastRewind(playerid);
#endif

#if defined RTC_OnPlayerConnect
	forward RTC_OnPlayerConnect(playerid);
#endif

#if defined RTC_OnDialogResponse
	forward RTC_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
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

#if defined RTC_OnPlayerDisconnect
	forward RTC_OnPlayerDisconnect(playerid, reason);
#endif

forward OnPlayerStartRewindEx(playerid);
forward OnPlayerFinishRewindEx(playerid);
forward OnPlayerRewindForPlayerEx(playerid, forplayerid);
forward SyncTimeMachine();
forward HideThisEffect(playerid);
forward SetPlayersCamBehind(playerid);
forward RerollTape(playerid);
forward SyncMachinePower(playerid);
forward StartSavingRewindData(playerid);
forward StartSyncingDots(playerid);
forward SyncedRewindProcess(playerid);
forward StartRerollingTapeSynced(playerid, RewindFlag);

#if !defined FILTERSCRIPT
	public OnGameModeInit()
#else
	public OnFilterScriptInit()
#endif
	{
		Rewind_World[WorldTimeHarmony] = SetTimer("SyncTimeMachine", REWIND_PLAYERS_HARMONY*60*1000, true);

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

		#if !defined FILTERSCRIPT
			#if defined RTC_OnGameModeInit
				return RTC_OnGameModeInit();
			#else
				return 1;
			#endif
		#else
			#if defined RTC_OnFilterScriptInit
				return RTC_OnFilterScriptInit();
			#else
				return 1;
			#endif
		#endif
	}

#if !defined FILTERSCRIPT 
	public OnGameModeExit()
#else
	public OnFilterScriptExit()
#endif
	{
		KillTimer(Rewind_World[WorldTimeHarmony]);

		for(new x; x < MAX_REWIND_TDRAW + 2; x++)
		{
			TextDrawDestroy(Rewind_TD[x]);
		}

		#if !defined FILTERSCRIPT
			#if defined RTC_OnGameModeExit
				return RTC_OnGameModeExit();
			#else
				return 1;
			#endif
		#else
			#if defined OnFilterScriptExit
				return OnFilterScriptExit();
			#else
				return 1;
			#endif
		#endif
	}


public OnPlayerConnect(playerid)
{
	Rewind_Player[playerid][HandOfTime] = SetTimerEx("StartSavingRewindData", 4000, 1, "i", playerid);
	Rewind_Player[playerid][DotsSync] = SetTimerEx("StartSyncingDots", 1000, 1, "i", playerid);

	Rewind_Player[playerid][RewindLabel] = 
		Create3DTextLabel("[][][][][][][][][][]" , 0xFFFFFFFF, 0.0, 0.0, 0.0, 25.0, 0, 1);
	Attach3DTextLabelToPlayer(Rewind_Player[playerid][RewindLabel], playerid, 0.0, 0.0, 0.3);

	Rewind_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 600.399719, 319.920074, "x1");
	PlayerTextDrawLetterSize(playerid, Rewind_PTD[playerid][0], 0.378399, 1.428267);
	PlayerTextDrawAlignment(playerid, Rewind_PTD[playerid][0], 1);
	PlayerTextDrawColor(playerid, Rewind_PTD[playerid][0], 255);
	PlayerTextDrawSetShadow(playerid, Rewind_PTD[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, Rewind_PTD[playerid][0], 1);
	PlayerTextDrawBackgroundColor(playerid, Rewind_PTD[playerid][0], -1067088598);
	PlayerTextDrawFont(playerid, Rewind_PTD[playerid][0], 2);
	PlayerTextDrawSetProportional(playerid, Rewind_PTD[playerid][0], 1);

	Rewind_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 600.399658, 341.573547, "x1");
	PlayerTextDrawLetterSize(playerid, Rewind_PTD[playerid][1], 0.378399, 1.428267);
	PlayerTextDrawAlignment(playerid, Rewind_PTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, Rewind_PTD[playerid][1], 255);
	PlayerTextDrawSetShadow(playerid, Rewind_PTD[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, Rewind_PTD[playerid][1], 1);
	PlayerTextDrawBackgroundColor(playerid, Rewind_PTD[playerid][1], -1067088598);
	PlayerTextDrawFont(playerid, Rewind_PTD[playerid][1], 2);
	PlayerTextDrawSetProportional(playerid, Rewind_PTD[playerid][1], 1);

	Rewind_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 600.600769, 361.881530, "x1");
	PlayerTextDrawLetterSize(playerid, Rewind_PTD[playerid][2], 0.378399, 1.428267);
	PlayerTextDrawAlignment(playerid, Rewind_PTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, Rewind_PTD[playerid][2], 255);
	PlayerTextDrawSetShadow(playerid, Rewind_PTD[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, Rewind_PTD[playerid][2], 1);
	PlayerTextDrawBackgroundColor(playerid, Rewind_PTD[playerid][2], -1067088598);
	PlayerTextDrawFont(playerid, Rewind_PTD[playerid][2], 2);
	PlayerTextDrawSetProportional(playerid, Rewind_PTD[playerid][2], 1);

	#if defined RTC_OnPlayerConnect
		return RTC_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerDisconnect(playerid, reason)
{
	ResetTimeMachine(playerid);
	SyncedRewindResetTools(playerid);

	TextDrawHideForPlayer(playerid, Rewind_TD[13]);
	KillTimer(Rewind_Player[playerid][HandOfTime]);
	Delete3DTextLabel(Rewind_Player[playerid][RewindLabel]);
	Rewind_Player[playerid][RewindLabel] = Text3D:INVALID_3DTEXT_ID;
	
	#if defined RTC_OnPlayerDisconnect
		return RTC_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == REWIND_CHOOSING_DIALOG)
	{
		switch(response)
		{
			case true:
			{
				if(Rewind_Player[playerid][SyncedRewindFlags] == SYNCEED_REWIND_BUFFER) Rewind_Player[playerid][SyncedRewindFlags] --;
				StopAudioStreamForPlayer(playerid);
				PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
				RewindPlayerFullySynced(playerid);
				Rewind_Player[playerid][SyncedRewindOn] = true;

				#if defined OnPlayerFastRewind
					OnPlayerFastRewind(playerid);
				#endif
			}

			case false:
			{
				TogglePlayerControllable(playerid, 1);
				SyncedRewindResetTools(playerid);
			}
		}
	}

	#if defined RTC_OnDialogResponse
		return RTC_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
	#else
		return 0;
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
			if(!IsPlayerConnected(i) || i == playerid || Rewind_Player[i][PRewind] || Rewind_Player[i][BRewind] || Rewind_Player[i][SyncedRewindOn]) continue;
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
			if(!IsPlayerConnected(i) || i == playerid || Rewind_Player[i][PRewind] || Rewind_Player[i][BRewind] || Rewind_Player[i][SyncedRewindOn]) continue;
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

static stock RewindMakePlayerChoose(playerid)
{
	TogglePlayerControllable(playerid, 0);
	PlayAudioStreamForPlayer(playerid, "https://vocaroo.com/media_command.php?media=s0iFx9S5qoFf&command=download_mp3");
	ShowPlayerDialog(playerid, REWIND_CHOOSING_DIALOG, DIALOG_STYLE_MSGBOX, "{FFFFFF}Time machine", "{FFFFFF}Play this rewind?", "{00FF00}Yes", "{FF0000}No");
	return 1;
}

static stock SyncedRewindResetTools(playerid)
{
	StopAudioStreamForPlayer(playerid);
	TogglePlayerControllable(playerid, 1);
	Rewind_Player[playerid][SyncedRewindOn] = false;
	Rewind_Player[playerid][SyncedRewindDouble] = false;
	Rewind_Player[playerid][SyncedRewindFlags] = 0;
	KillTimer(Rewind_Player[playerid][SyncedRewindTimer]);
	KillTimer(Rewind_Player[playerid][SyncedRerollTimer]);
	return 1;
}

static stock RewindPlayerFullySynced(playerid)
{
	return Rewind_Player[playerid][SyncedRerollTimer] = SetTimerEx("StartRerollingTapeSynced", 100, true, "i", playerid, Rewind_Player[playerid][SyncedRewindFlags]); 
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
		case 4..REWIND_MAXIMUM_PHASES:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
			new Rewind_TD_String[10];
			if(Rewind_Player[playerid][RewindPhases] >= 4 && Rewind_Player[playerid][RewindPhases] <= 3+9) 
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][2], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			}
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+1 && Rewind_Player[playerid][RewindPhases] <= 3+9+9)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][1], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			} 
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+9+1 && Rewind_Player[playerid][RewindPhases] <= REWIND_MAXIMUM_PHASES)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][0], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][0]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
			}
		}
	}

	return 1;
}

stock bool:IsPlayerRewinding(playerid) 
	return Rewind_Player[playerid][PRewind];

stock bool:IsPlayerInvolvedInRewind(playerid) 
	return Rewind_Player[playerid][BRewind];
	
stock bool:IsPlayerFastRewinding(playerid)
	return Rewind_Player[playerid][SyncedRewindOn];

stock ToggleRewindFlash(bool:toggle)
{
	return Rewind_World[RewindFlash] = toggle;
}

stock ToggleRewindFlashForPlayer(playerid, bool:toggle)
{
	return Rewind_Player[playerid][PRewindFlash] = toggle;
}

stock ToggleCameraMovements(bool:toggle)
{
	return Rewind_World[RewindCamera] = toggle;
}

stock ToggleCameraMovementsForPlayer(playerid, bool:toggle)
{
	return Rewind_Player[playerid][PRewindCamera] = toggle;
}

stock ToggleRewindAuto(bool:toggle)
{
	return Rewind_World[RewindAuto] = toggle;
}

stock ToggleRewindAutoForPlayer(playerid, bool:toggle)
{
	return Rewind_Player[playerid][PRewindAuto] = toggle;
}

stock ToggleWorldRewind(bool:toggle)
{
	return Rewind_World[WorldRewind] = toggle;
}

stock ToggleWorldRewindForPlayer(playerid, bool:toggle)
{
	return Rewind_Player[playerid][PrewindWorld] = toggle;
}

stock StartRecordingRewindSynced(playerid)
{
	if(Rewind_Player[playerid][PRewind] || Rewind_Player[playerid][BRewind] || Rewind_Player[playerid][SyncedRewindDouble]) return 0;
	Rewind_Player[playerid][SyncedRewindDouble] = true;

	SaveRewindFlag(playerid);
	Rewind_Player[playerid][SyncedRewindTimer] = SetTimerEx("SyncedRewindProcess", 100, true, "i", playerid);
	return 1;
}

stock StartRewinding(playerid)
{
	if(Rewind_Player[playerid][PRewind] || Rewind_Player[playerid][BRewind] || Rewind_Player[playerid][SyncedRewindOn] || Rewind_Player[playerid][SyncedRewindDouble]) return 0;
	if(Rewind_Player[playerid][RewindPhases] < 1) return 0;
	Rewind_Player[playerid][PRewind] = true;
	if(Rewind_Player[playerid][ResyncOn]) HideTimeMachineSynced(playerid);

	if(Rewind_Player[playerid][RewindPhases] == REWIND_MAXIMUM_PHASES) Rewind_Player[playerid][RewindPhases] --;

	ShowPlayerRewindScene(playerid);
	PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
	TogglePlayerControllable(playerid, 0);
	if(!Rewind_World[WorldRewind] && !Rewind_Player[playerid][PrewindWorld]) RewindWorld(playerid);
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
		case 4..REWIND_MAXIMUM_PHASES:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
			new Rewind_TD_String[10];
			if(Rewind_Player[playerid][RewindPhases] >= 4 && Rewind_Player[playerid][RewindPhases] <= 3+9) 
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][2], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			}
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+1 && Rewind_Player[playerid][RewindPhases] <= 3+9+9)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][1], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			} 
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+9+1 && Rewind_Player[playerid][RewindPhases] <= REWIND_MAXIMUM_PHASES)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][0], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][0]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			}
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

static RewindOldEqualsNew(playerid, CurrentFlagCount)
{
	if(CurrentFlagCount == -1) return 0;

	new Float:Rewind_X, Float:Rewind_Y, Float:Rewind_Z;
	GetPlayerPos(playerid, Rewind_X, Rewind_Y, Rewind_Z);

	if(Rewind_Player[playerid][RSPosX][CurrentFlagCount] == Rewind_X && Rewind_Player[playerid][RSPosY][CurrentFlagCount] == Rewind_Y &&
	Rewind_Player[playerid][RSPosZ][CurrentFlagCount] == Rewind_Z)
	{
		return 1;
	}

	return 0;
}

static SaveRewindFlag(playerid)
{
	new PlayerRewindVeh = GetPlayerVehicleID(playerid);
	new PlayerRewindState = GetPlayerState(playerid);
	Rewind_Player[playerid][RSPInterior][ Rewind_Player[playerid][SyncedRewindFlags] ] = GetPlayerInterior(playerid);
	Rewind_Player[playerid][RSPVWorld][ Rewind_Player[playerid][SyncedRewindFlags] ]   = GetPlayerVirtualWorld(playerid);
	switch(PlayerRewindState)
	{
		case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
		{
			GetVehiclePos(PlayerRewindVeh, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ]
				, Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ]), Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] = PlayerRewindVeh;
			GetVehicleZAngle(PlayerRewindVeh, Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		}

		case PLAYER_STATE_ONFOOT:
		{
			GetPlayerPos(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ]
		, 		Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ]);
			GetPlayerFacingAngle(playerid, Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		}
	}
	return Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ] = PlayerRewindState;
}

static HideForPlayerRewindScene(playerid)
{
	for(new x; x < MAX_REWIND_TDRAW + 1; x++)
	{
		TextDrawHideForPlayer(playerid, Rewind_TD[x]);
	}
	RewindResetPlayerTD(playerid);
	return 1;	
}

static RewindResetPlayerTD(playerid)
{
	PlayerTextDrawHide(playerid, Rewind_PTD[playerid][0]);
	PlayerTextDrawHide(playerid, Rewind_PTD[playerid][1]);
	PlayerTextDrawHide(playerid, Rewind_PTD[playerid][2]);
	return 1;
}

static GetBarDots(playerid)
{
	new Rewind_Dots[64];

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 4..REWIND_MAXIMUM_PHASES:
		{
			Rewind_Dots = "{006600}[][][][][][][][][][]";
		}
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

static SetPlayeRSPosByPhaseSynced(playerid)
{
	new PlayerTempState = GetPlayerState(playerid);
	if(Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ] == -1 || Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ] == 0) Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ] = PlayerTempState;

	if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_DRIVER) )
	{
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] , PLAYER_SEAT_DRIVER);
		SetVehiclePos(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPVWorld][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPInterior][ Rewind_Player[playerid][SyncedRewindFlags] ]);
	}
	else if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_PASSENGER) ||
	(PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_PASSENGER) )
	{	
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] , PLAYER_SEAT_FRONT_PASSENGER);
		SetVehiclePos(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPVWorld][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPInterior][ Rewind_Player[playerid][SyncedRewindFlags] ]);
	}


	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_PASSENGER) )
	{
		SetVehiclePos(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPVWorld][ Rewind_Player[playerid][SyncedRewindFlags] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindSVehicle][ Rewind_Player[playerid][SyncedRewindFlags] ], Rewind_Player[playerid][RSPInterior][ Rewind_Player[playerid][SyncedRewindFlags] ]);
	}
	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_ONFOOT) || 
	(PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_ONFOOT) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][RSOldState][ Rewind_Player[playerid][SyncedRewindFlags] ]  == PLAYER_STATE_ONFOOT) )
	{
		SetPlayerPos(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] );
		SetPlayerFacingAngle(playerid, Rewind_Player[playerid][RSPosAngle][ Rewind_Player[playerid][SyncedRewindFlags] ]); 
	}

	SetPlayerVirtualWorld(playerid, Rewind_Player[playerid][RSPVWorld][ Rewind_Player[playerid][SyncedRewindFlags] ]);
	SetPlayerInterior(playerid, Rewind_Player[playerid][RSPInterior][ Rewind_Player[playerid][SyncedRewindFlags] ]);
	return 1;
}

static SetPlayerPosByPhase(playerid)
{

	new PlayerTempState = GetPlayerState(playerid);
	if(Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] == -1 || Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] == 0) Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] = PlayerTempState;

	if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) )
	{
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , PLAYER_SEAT_DRIVER);
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPVWorld][ Rewind_Player[playerid][RewindPhases] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPInterior][ Rewind_Player[playerid][RewindPhases] ]);
	}
	else if( (PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) ||
	(PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) )
	{	
		PutPlayerInVehicle(playerid, Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , PLAYER_SEAT_FRONT_PASSENGER);
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPVWorld][ Rewind_Player[playerid][RewindPhases] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPInterior][ Rewind_Player[playerid][RewindPhases] ]);
	}


	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_DRIVER) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_PASSENGER) )
	{
		SetVehiclePos(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
		SetVehicleZAngle(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ]);
		SetVehicleVirtualWorld(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPVWorld][ Rewind_Player[playerid][RewindPhases] ]);
		LinkVehicleToInterior(Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPInterior][ Rewind_Player[playerid][RewindPhases] ]);
	}
	else if( (PlayerTempState == PLAYER_STATE_DRIVER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) || 
	(PlayerTempState == PLAYER_STATE_ONFOOT && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) ||
	(PlayerTempState == PLAYER_STATE_PASSENGER && Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ]  == PLAYER_STATE_ONFOOT) )
	{
		SetPlayerPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] );
		SetPlayerFacingAngle(playerid, Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ]); 
	}

	SetPlayerVirtualWorld(playerid, Rewind_Player[playerid][RPVWorld][ Rewind_Player[playerid][RewindPhases] ]);
	SetPlayerInterior(playerid, Rewind_Player[playerid][RPInterior][ Rewind_Player[playerid][RewindPhases] ]);
	return 1;
}

static FinishRewind(playerid)
{
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
	Rewind_Player[playerid][PRewindFlash] = false;
	Rewind_Player[playerid][PRewindAuto] = false;
	Rewind_Player[playerid][PRewindCamera] = false;
	Rewind_Player[playerid][PrewindWorld] = false;
	Rewind_Player[playerid][RewindHP] = 0.0;
	Rewind_Player[playerid][RewindArmour] = 0.0;
	Rewind_Player[playerid][RewindPhases] = 0;
	Rewind_Player[playerid][BRewind] = false;
	Rewind_Player[playerid][PRewind] = false;
	Rewind_Player[playerid][ResyncOn] = false;
	KillTimer(Rewind_Player[playerid][RewindSync]);
	
	for(new x; x < REWIND_MAXIMUM_PHASES + 1; x++)
	{
		Rewind_Player[playerid][RPosX][x] = 0.0;
		Rewind_Player[playerid][RPosY][x] = 0.0;
		Rewind_Player[playerid][RPosZ][x] = 0.0;
		Rewind_Player[playerid][RPosAngle][x] = 0.0;

		Rewind_Player[playerid][RewindVehicle][x] = 0;
		Rewind_Player[playerid][ROldState][x] = 0;

		Rewind_Player[playerid][RPInterior][x] = 0;
		Rewind_Player[playerid][RPVWorld][x] = 0;
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
	return Update3DTextLabelText(Rewind_Player[playerid][RewindLabel], GetPlayerColor(playerid) >>> 8, GetBarDots(playerid));
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

public HideThisEffect(playerid)
{
	return TextDrawHideForPlayer(playerid, Rewind_TD[13]);
}

public SetPlayersCamBehind(playerid)
{
	return SetCameraBehindPlayer(playerid);
}

public RerollTape(playerid)
{
	if(!Rewind_Player[playerid][PRewindFlash] && !Rewind_World[RewindFlash])
	{
		TextDrawShowForPlayer(playerid, Rewind_TD[13]);
		SetTimerEx("HideThisEffect", 100, false, "i", playerid);
	}

	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 23:
		{
			StopAudioStreamForPlayer(playerid);
			PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
		}

		case 16:
		{
			StopAudioStreamForPlayer(playerid);
			PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
		}

		case 9:
		{
			StopAudioStreamForPlayer(playerid);
			PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");
		}

		case 3:
		{
			StopAudioStreamForPlayer(playerid);
			PlayAudioStreamForPlayer(playerid, "https://s0.vocaroo.com/media/download_temp/Vocaroo_s02Q30FyGOWp.mp3");				
		}
	}
	
	switch(Rewind_Player[playerid][RewindPhases])
	{
		case 0:
		{
			SetPlayerPosByPhase(playerid);
			FinishRewind(playerid);
			SetPlayerHealth(playerid, Rewind_Player[playerid][RewindHP]);
			SetPlayerArmour(playerid, Rewind_Player[playerid][RewindArmour]);
			return 1;
		}

		case 1..REWIND_MAXIMUM_PHASES:
		{
			switch(Rewind_Player[playerid][RewindPhases])
			{
				case 1..3:
				{
					PlayerTextDrawHide(playerid, Rewind_PTD[playerid][0]);
					PlayerTextDrawHide(playerid, Rewind_PTD[playerid][1]);
					PlayerTextDrawHide(playerid, Rewind_PTD[playerid][2]);
					TextDrawHideForPlayer(playerid, Rewind_TD[ Rewind_Player[playerid][RewindPhases] - 1]);
				}
				case 4..REWIND_MAXIMUM_PHASES:
				{
					new Rewind_TD_String[10];
					if(Rewind_Player[playerid][RewindPhases] >= 4 && Rewind_Player[playerid][RewindPhases] <= 3+9) 
					{
						format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3);
						PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][2], Rewind_TD_String);
						PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
					}
					else if(Rewind_Player[playerid][RewindPhases] >= 3+9+1 && Rewind_Player[playerid][RewindPhases] <= 3+9+9)  
					{
						format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9);
						PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][1], Rewind_TD_String);
						PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
					} 
					else if(Rewind_Player[playerid][RewindPhases] >= 3+9+9+1 && Rewind_Player[playerid][RewindPhases] <= REWIND_MAXIMUM_PHASES)  
					{
						format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9-9);
						PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][0], Rewind_TD_String);
						PlayerTextDrawShow(playerid, Rewind_PTD[playerid][0]); 
					}
				}
			}

			if(!Rewind_Player[playerid][PRewindCamera] && !Rewind_World[RewindCamera])
			{
				if(Rewind_Player[playerid][RewindPhases] == REWIND_MAXIMUM_PHASES)
				{
					InterpolateCameraPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]+10, 450);
					InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] + 1, Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] - 1, Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] + 1, 450);
				}
				else
				{
					InterpolateCameraPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] +1 ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]+10, 450);
					InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] +1 ] , Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] +1 ], Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ] + 1, Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ] - 1, Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ] + 1, 450);
				}
			}

			if(!Rewind_Player[playerid][PRewindCamera] && !Rewind_World[RewindCamera]) SetTimerEx("SetPlayersCamBehind", 700, false, "i", playerid);
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
	RewindResetPlayerTD(playerid);
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
		case 4..REWIND_MAXIMUM_PHASES:
		{
			TextDrawShowForPlayer(playerid, Rewind_TD[0]);
			TextDrawShowForPlayer(playerid, Rewind_TD[1]);
			TextDrawShowForPlayer(playerid, Rewind_TD[2]);
			new Rewind_TD_String[10];
			if(Rewind_Player[playerid][RewindPhases] >= 4 && Rewind_Player[playerid][RewindPhases] <= 3+9) 
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][2], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			}
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+1 && Rewind_Player[playerid][RewindPhases] <= 3+9+9)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][1], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 

			} 
			else if(Rewind_Player[playerid][RewindPhases] >= 3+9+9+1 && Rewind_Player[playerid][RewindPhases] <= REWIND_MAXIMUM_PHASES)  
			{
				format(Rewind_TD_String,sizeof(Rewind_TD_String), "X%d", Rewind_Player[playerid][RewindPhases]-3-9-9);
				PlayerTextDrawSetString(playerid, Rewind_PTD[playerid][0], Rewind_TD_String);
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][0]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][1]); 
				PlayerTextDrawShow(playerid, Rewind_PTD[playerid][2]); 
			}
		}
	}
	return 1;
}

public StartSavingRewindData(playerid)
{
	if(Rewind_Player[playerid][PRewindAuto] || Rewind_World[RewindAuto]) return 0;

	new PlayerVehicleID = GetPlayerVehicleID(playerid),
		PlayerCurrentState = GetPlayerState(playerid);

	if(Rewind_Player[playerid][PRewind] || Rewind_Player[playerid][BRewind]) return 0;
	if(PlayerCurrentState == PLAYER_STATE_NONE || PlayerCurrentState == PLAYER_STATE_WASTED 
	|| PlayerCurrentState == PLAYER_STATE_SPECTATING || PlayerCurrentState == PLAYER_STATE_EXIT_VEHICLE
	|| PlayerCurrentState == PLAYER_STATE_ENTER_VEHICLE_DRIVER 
	|| PlayerCurrentState == PLAYER_STATE_ENTER_VEHICLE_PASSENGER) return 0;

	if(Rewind_Player[playerid][RewindPhases] == REWIND_MAXIMUM_PHASES)
	{
		for(new x; x < REWIND_MAXIMUM_PHASES + 1; x++)
		{
			Rewind_Player[playerid][ROldState][x] = -1;
			Rewind_Player[playerid][RPInterior][x] = 0;
			Rewind_Player[playerid][RPVWorld][x] = 0;
		}
		Rewind_Player[playerid][RewindPhases] = 0;
	}

	Rewind_Player[playerid][RPInterior][ Rewind_Player[playerid][RewindPhases] ] = GetPlayerInterior(playerid);
	Rewind_Player[playerid][RPVWorld][ Rewind_Player[playerid][RewindPhases] ]   = GetPlayerVirtualWorld(playerid);
	Rewind_Player[playerid][ROldState][ Rewind_Player[playerid][RewindPhases] ] = PlayerCurrentState;

	switch(PlayerCurrentState)
	{
		case PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER:
		{
			GetVehiclePos(PlayerVehicleID, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]);
			Rewind_Player[playerid][RewindVehicle][ Rewind_Player[playerid][RewindPhases] ] = PlayerVehicleID;
			GetVehicleZAngle(PlayerVehicleID, Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ] );
		}
		case PLAYER_STATE_ONFOOT:
		{
			GetPlayerPos(playerid, Rewind_Player[playerid][RPosX][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosY][ Rewind_Player[playerid][RewindPhases] ], Rewind_Player[playerid][RPosZ][ Rewind_Player[playerid][RewindPhases] ]);
			GetPlayerFacingAngle(playerid, Rewind_Player[playerid][RPosAngle][ Rewind_Player[playerid][RewindPhases] ]);
		}
	}

	if(Rewind_Player[playerid][RewindPhases] == 0)
	{
		GetPlayerHealth(playerid, Rewind_Player[playerid][RewindHP]);
		GetPlayerArmour(playerid, Rewind_Player[playerid][RewindArmour]);
		Rewind_Player[playerid][RewindPhases]++;
	}
	else if(REWIND_MAXIMUM_PHASES > Rewind_Player[playerid][RewindPhases] > 0)
	{
		Rewind_Player[playerid][RewindPhases]++;
	}
	return 1;
}

public SyncedRewindProcess(playerid)
{
	if(!RewindOldEqualsNew(playerid, Rewind_Player[playerid][SyncedRewindFlags]-1))
	{
		Rewind_Player[playerid][SyncedRewindFlags]++;
		SaveRewindFlag(playerid);
	}

	if(Rewind_Player[playerid][SyncedRewindFlags] == SYNCEED_REWIND_BUFFER)
	{
		KillTimer(Rewind_Player[playerid][SyncedRewindTimer]);
		RewindMakePlayerChoose(playerid);
	}
	return 1;
}

public StartRerollingTapeSynced(playerid, RewindFlag)
{
	if(!Rewind_Player[playerid][PRewindFlash] && !Rewind_World[RewindFlash])
	{
		TextDrawShowForPlayer(playerid, Rewind_TD[13]);
		SetTimerEx("HideThisEffect", 70, false, "i", playerid);
	}

	switch(Rewind_Player[playerid][SyncedRewindFlags])
	{
		case 0:
		{
			SetPlayeRSPosByPhaseSynced(playerid);
			SyncedRewindResetTools(playerid);

			#if defined OnPlayerFinishFastRewind
				OnPlayerFinishFastRewind(playerid);
			#endif

			return 1;
		}

		case 1..SYNCEED_REWIND_BUFFER:
		{
			if(Rewind_Player[playerid][SyncedRewindFlags] == SYNCEED_REWIND_BUFFER)
			{
				if(!Rewind_Player[playerid][PRewindCamera] && !Rewind_World[RewindCamera]) 
				{
					InterpolateCameraPos(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ], 
					Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ]+10, 200);

					InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ], 
					Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] + 1, 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] - 1, 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] + 1, 200);
				}
			}
			else
			{
				if(!Rewind_Player[playerid][PRewindCamera] && !Rewind_World[RewindCamera])
				{
					InterpolateCameraPos(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] +1 ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] +1 ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] +1 ], 
					Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ]+10, 200);

					InterpolateCameraLookAt(playerid, Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] +1 ] , 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] +1 ] , 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] +1 ], 
					Rewind_Player[playerid][RSPosX][ Rewind_Player[playerid][SyncedRewindFlags] ] + 1, 
					Rewind_Player[playerid][RSPosY][ Rewind_Player[playerid][SyncedRewindFlags] ] - 1, 
					Rewind_Player[playerid][RSPosZ][ Rewind_Player[playerid][SyncedRewindFlags] ] + 1, 200);
				}
			}

			if(!Rewind_Player[playerid][PRewindCamera] && !Rewind_World[RewindCamera]) SetTimerEx("SetPlayersCamBehind", 250, false, "i", playerid);
			SetPlayeRSPosByPhaseSynced(playerid);
			Rewind_Player[playerid][SyncedRewindFlags]--;
			return 1;
		}
	}
	return 1;
}

//Hookings: 
#if defined _ALS_OnDialogResponse
  #undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif

#define OnDialogResponse RTC_OnDialogResponse

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
