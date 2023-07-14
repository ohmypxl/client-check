# Android/PC Client Check

[![sampctl](https://img.shields.io/badge/sampctl-client--check-2f2f2f.svg?style=for-the-badge)](https://github.com/ohmypxl/client-check)

A forked forked forked project, using y_android and fixes implementations to simplify everything.
Support`IsPlayerUsingAndroid(playerid)` just like original repo by Fairuz and also support manual checking (More info below).


## Installation

Simply install to your project:

```bash
sampctl package install ohmypxl/client-check
```

Include in your code and begin using the library:

```pawn
#include <client-check>
```

## Guide

This guide will teach you how to use client-check wisely, so if you're interested please continue reading below. If you don't you can just skip it by choosing the following link to get into your destination:

* [Getting Started](#getting-started)
* [Function/Callback list(s)] (#function--callbacks)
* [Macros/Define list(s)](#macros--define)


## Getting Started

Using this lib is easy, you can just use the `OnPlayerClientChecked(playerid, ClientCheck:status)` callback and it'll just works!.

### Example 1 - Automatic Check
```c
#include <a_samp>
#include <client-check>

public OnPlayerClientChecked(playerid, ClientStatus:status)
{
	if (IsUsingAndroid(status))
	{
		Ban(playerid);
	}
}
```

Did you notice there is `IsUsingAndroid` in the example above? yep that's right you can use that function/macro without doing the check manually if you don't know what contents of `ClientStatus:` is.

That is how you can pretty much use this library/include, now let's move on into the next case!

Imagine you still not sure about `OnPlayerClientChecked` and you want to check the player status, well instead of you storing the `ClientStatus:status` value, why don't you use this two functions that i've created for this special case?

That function is: `IsPlayerUsingAndroid(playerid)` and `IsPlayerUsingPC(playerid)`!.

That function will check if the player is using android or using PC, please note that the following function only works if you don't define `CLIENT_NO_AUTO_CHECK`.

### Example 2 - Manual Check

In the last example i was mentioning about `if you don't define CLIENT_NO_AUTO_CHECK` right? well that define will disable the checking automatically after player connecting to the server, this option should be defined if you understand about how Client Check works or if you want to do checking manually via trigger command like this:

```c
#include <a_samp>
#include <Some_Command_Processor>
#include <client-check>

public OnPlayerClientChecked(playerid, ClientStatus:status)
{
	if (IsUsingAndroid(status))
	{
		Ban(playerid);
	}
}

CMD:clientcheck(playerid, params[])
{
	new targetid;
	if (sscanf(params, "r", targetid))
	{
		SendClientMessage(playerid, COLOR_USAGE, "Usage: /clientcheck <playerid/PartOfName>");
		return 1;
	}

	if (targetid == INVALID_PLAYER_ID)
	{
		SendClientMessage(playerid, COLOR_ERROR, "Error: Player invalid!");
		return 1;
	}

	CheckPlayerClient(playerid);
	return 1;
}
```

Everytime you use `CheckPlayerClient`, the `OnPlayerClientChecked` callback will be called. Unfortunately you can't supply custom callback for this one and also it's best to call `CheckPlayerClient` manually when you're defining `CLIENT_NO_AUTO_CHECK` since without that define it would be called automatically thus making call will be pointless.

You might be wondering, how can i define `CLIENT_NO_AUTO_CHECK`? Well you can just place `#define CLIENT_NO_AUTO_CHECK` just before including the `client-check` and that's how you define it. 

## Function & Callbacks
* `CheckPlayerClient(playerid)` will try to send client check into the playerid, but the playerid must be connected of all times.
* `IsPlayerUsingAndroid(playerid)` will try to check if player from the results of `CheckPlayerClient` turns out to be using android or not.
* `IsPlayerUsingPC(playerid)` same as `IsPlayerUsingAndroid` except it checks if user using PC.
* `IsUsingAndroid(status)` same as `IsPlayerUsingAndroid` but it only can be used inside of `OnPlayerClientChecked`.
* `IsUsingAndroid(status)` same as `IsPlayerUsingPC` but it only can be used inside of `OnPlayerClientChecked`.
* `OnPlayerClientChecked(playerid, ClientStatus:status)` is a callback that would be automatically be called whenever the script returned the results from `CheckPlayerClient`

(You cannot use this inside OnPlayerConnect (except for `CheckPlayerClient`, that is why i made a custom one :>)

## Macros / Define
* `#define CLIENT_NO_AUTO_CHECK` will disable automatically calling `CheckPlayerClient` after player connected to the server.
* `#define CLIENT_CHECK_TIME value` will try to change the interval at `value` time (in milisecond) for the android check if defined.

(All of this needed to be placed BEFORE including `client-check`)
