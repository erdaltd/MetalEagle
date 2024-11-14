// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePawn.generated.h"

/**
 * 
 */
UCLASS()
class METALEAGLE_API ACorePawn : public APawn
{
	GENERATED_BODY()
	
	
public:
	ACorePawn();
	static ACorePawn* INS;
	void SetupPlayerInputComponent(class UInputComponent* _InputComp) override;
	bool PressGrabLeft;
	bool PressGrabRight;
private:
	void pressGrapLeft(); void _pressGrapLeft();
	void pressGrapRight();  void _pressGrapRight();
};
