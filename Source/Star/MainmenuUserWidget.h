// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* JoinSessionButton;

	void CreateSession();

	void JoinSession();
};