// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreAnimInstance.h"
#include "BodyAnimInstance.generated.h"

class APlayerPawn;
/**
 * 
 */
UCLASS()
class METALEAGLE_API UBodyAnimInstance : public UCoreAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	FTransform GetHandTransform(EControllerHand handId);
	UPROPERTY(BlueprintReadOnly, Category = METALEAGLE) FTransform TargetHandLeftTransform;
	UPROPERTY(BlueprintReadOnly, Category = METALEAGLE) FTransform TargetHandRightTransform;
private:
	APlayerPawn* player;
	
};
