// Fill out your copyright notice in the Description page of Project Settings.


#include "MainmenuUserWidget.h"
#include "Components/Button.h"



void UMainmenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Construct Test"));

	CreateSessionButton = (UButton*) GetWidgetFromName(TEXT("CreateSessionButton"));
	CreateSessionButton->OnClicked.AddDynamic(this, &UMainmenuUserWidget::CreateSession);
	
	JoinSessionButton = (UButton*)GetWidgetFromName(TEXT("JoinSessionButton"));
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainmenuUserWidget::JoinSession);
}

void UMainmenuUserWidget::CreateSession()
{
	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, TEXT("Create Session Test"));
}

void UMainmenuUserWidget::JoinSession()
{
	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, TEXT("Join Session Test"));
}
