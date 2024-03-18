// Fill out your copyright notice in the Description page of Project Settings.


#include "MainmenuHUD.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

void AMainmenuHUD::CreateSession(FString SessionName, bool IsLan)
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IsLan;
	SessionSettings.NumPublicConnections = 10;
	SessionSettings.NumPrivateConnections = 0;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set("GameName", SessionName);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
		}
	}
}
