// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreActor.h"
#include "Core/CoreUserWidget.h"
#include "Components/WidgetComponent.h"
#include "WidgetActor.generated.h"

/**
 * 
 */
UCLASS()
class METALEAGLE_API AWidgetActor : public ACoreActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();
	void SetVisible(bool _visible);
	UUserWidget* GetUserWidget() { return ui; };

	void Print(FString _id, FString _msg);
	//void PrintTime(FString _msg);s
	
protected:
	UCoreUserWidget* ui;
	UWidgetComponent* compWidget;

};
