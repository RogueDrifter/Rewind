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
```

Functions:
```C
ShowTimeMachineSynced(playerid);
HideTimeMachineSynced(playerid);

StartRewinding(playerid);

bool:IsPlayerInvolvedInRewind(playerid);
bool:IsPlayerRewinding(playerid);
```

More add-ons:
```C
#define REWIND_MAXIMUM_PHASES 5 //Can b changed just put the line with your number before the include.
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
