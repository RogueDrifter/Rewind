// generated by "sampctl package generate"

#include "Rewind.inc"

main() {
	// write tests for libraries here and run "sampctl package run"
}

public OnPlayerRewindForPlayer(playerid, forplayerid)
{
	new Rewind_String[40];
	format(Rewind_String, sizeof(Rewind_String), "%d Is rewinding for %d", playerid, forplayerid);
	SendClientMessageToAll(-1, Rewind_String);
	return 1;
}

public OnPlayerStartRewind(playerid)
{
	new Rewind_String[40];
	format(Rewind_String, sizeof(Rewind_String), "%d Is rewinding", playerid);
	SendClientMessageToAll(-1, Rewind_String);
	return 1;
}

public OnPlayerFinishRewind(playerid)
{
	new Rewind_String[40];
	format(Rewind_String, sizeof(Rewind_String), "%d Is finished rewinding", playerid);
	SendClientMessageToAll(-1, Rewind_String);
	return 1;
}

public OnPlayerConnect(playerid)
{
	ShowTimeMachineSynced(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	HideTimeMachineSynced(playerid);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(!IsPlayerInvolvedInRewind(playerid) && !IsPlayerRewinding(playerid) && newstate == PLAYER_STATE_ONFOOT)
	{
		StartRewinding(playerid);
	}
	return 1;
}