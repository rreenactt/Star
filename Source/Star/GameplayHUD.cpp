// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayHUD.h"
#include "GameFramework/OnlineSession.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void AGameplayHUD::BeginPlay()
{
	Super::BeginPlay();
    
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) 
	{
		IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
		if (SessionInterface.IsValid()) 
		{
			FNamedOnlineSession* CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
			if (CurrentSession)
			{
			}
			else 
			{
				GEngine->AddOnScreenDebugMessage(1, 1.5, FColor::Red, TEXT("Faild"));
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(1, 1.5, FColor::Blue, TEXT("Success. Session Name : "));

}
