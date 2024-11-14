// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class METALEAGLE_API UCoreUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
		UFUNCTION(Category = METALEAGLE) virtual void Run(FString cmd);
	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventRun(const FString &Function, const FString &Params);
	
	
};
