#include <a_samp>
#include <fixes>

#include "client-check.inc"


main() {}

public OnGameModeInit() 
{
	printf("Loading...");
	return 1;
}

public OnPlayerClientChecked(playerid, ClientStatus:status)
{
	print("Is it called? (4)");
	new szMessages[64];
	format(szMessages, sizeof(szMessages), "Menggunakan %s", IsUsingAndroid(status) ? ("Android") : ("PC"));
	SendClientMessage(playerid, -1, szMessages);
	return 1;
}
