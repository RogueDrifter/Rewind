# Rewind

[![sampctl](https://shields.southcla.ws/badge/sampctl-Rewind-2f2f2f.svg?style=for-the-badge)](https://github.com/RogueDrifter/Rewind)

A rewind/time machine script that takes you back a few steps/actions! Includes textdraws and 3D text labels.

## Installation

Simply install to your project:

```bash
sampctl package install RogueDrifter/Rewind
```

Include in your code and begin using the library (Only in one script):

```pawn
#include <Rewind>
```

## Usage

Callbacks:
```C
public OnPlayerRewindForPlayer(playerid, forplayerid);
public OnPlayerStartRewind(playerid);
public OnPlayerFinishRewind(playerid);

public OnPlayerFastRewind(playerid);
public OnPlayerFinishFastRewind(playerid);
```

Functions:
```C
ShowTimeMachineSynced(playerid);
HideTimeMachineSynced(playerid);

StartRewinding(playerid);
StartRecordingRewindSynced(playerid);

bool:IsPlayerInvolvedInRewind(playerid);
bool:IsPlayerRewinding(playerid);
bool:IsPlayerFastRewinding(playerid);

ToggleRewindFlash(bool:toggle);
ToggleRewindFlashForPlayer(playerid, bool:toggle);

ToggleRewindCameraMove(bool:toggle);
ToggleRewindCameraMoveForPlayer(playerid, bool:toggle);

ToggleRewindAuto(bool:toggle);
ToggleRewindAutoForPlayer(playerid, bool:toggle);

ToggleWorldRewind(bool:toggle);
ToggleWorldRewindForPlayer(playerid, bool:toggle);

```

More add-ons:
```C
#define REWIND_MAXIMUM_PHASES [NUMBER] //Can be changed just put the line with your number before the include.
#define REWIND_CHOOSING_DIALOG [NUMBER] //Dialog ID for the fast rewind you can change for whatever reason
#define REWiND_FULLY_SYNCED_TIMER [NUMBER] //This is in MS you can change it depending on how fast you want the timers to record/play
#define SYNCEED_REWIND_BUFFER [NUMBER] //This is the capacity of the fast rewind cells, don't change this if you don't know what you're doing
```

## Testing
To test, simply run the package:

```bash
sampctl package run
```
## Extras
```
1- A timer that rolls over to sync all player's ability to rewind.

2- Textdraws that define the rewind ability.

3- 3D Text labels to mark for other players your ability to rewind.
```
Currently supports vehicle interactions (not full) and on-foot interactions (not full either) but goes back up to 29 takes with an audio stream and a flashing extra textdraw.
