// Fill out your copyright notice in the Description page of Project Settings.


#include "MainmenuUserWidget.h"
#include "Components/Button.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"



void UMainmenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Construct Test"));

	CreateSessionButton = (UButton*) GetWidgetFromName(TEXT("CreateSessionButton"));
	CreateSessionButton->OnClicked.AddDynamic(this, &UMainmenuUserWidget::StartCreateSession);
	
	JoinSessionButton = (UButton*)GetWidgetFromName(TEXT("JoinSessionButton"));
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainmenuUserWidget::JoinSession);
	
}

void UMainmenuUserWidget::StartCreateSession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 10;
	
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMainmenuUserWidget::OnCreateSessionComplete);
	SessionInterface->CreateSession(0, TEXT("1234asdjfao"), SessionSettings);

	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, TEXT("Create Session Test"));

}

void UMainmenuUserWidget::JoinSession()
{
	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, TEXT("Join Session Test"));
}

void UMainmenuUserWidget::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString option = TEXT("/Game/Mycontents/Maps/MyMap.MyMap");
		FString levelName = L"/Game/Mycontents/Maps/MyMap.MyMap";
		UGameplayStatics::OpenLevel(this, FName(levelName), true, option);
	}
}

