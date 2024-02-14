// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Interfaces/OnlineSessionInterface.h"
#include "MainmenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STAR_API UMainmenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	UFUNCTION()
	void StartCreateSession();

	UFUNCTION()
	void JoinSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//IOnlineSessionPtr OnlineSessionInterface;

	//FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
private:
	UGameInstance* GameInstance;
};
