// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreActor.h"
#include <MotionControllerComponent.h>
#include "Core/CoreAnimInstance.h"
#include "HandCont.generated.h"

class AInteractActor; 
UCLASS()
class METALEAGLE_API AHandCont : public ACoreActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetalEagle)  EControllerHand Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetalEagle)  int GripStatus = -1;

	void SetActive();
	void SetHandAnimTarget(UCoreAnimInstance* _animIns);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnOverlap(bool _status, FContactResult _contact) override;
	
	UFUNCTION(BlueprintCallable, Category = MetalEagle) UMotionControllerComponent* GetMotionComp() {
		return compMotionCont;
	};
	UFUNCTION(BlueprintCallable,  Category = MetalEagle) FVector GetMotionLoc();
	FRotator GetMotionRot();
	UFUNCTION(BlueprintCallable, Category = MetalEagle) FRotator GetMotionRelRot();
	void Hold(AActor* _item);
	void Release();
	void RunAnimFunction(FString _funcName);
private:
	UPROPERTY() UMotionControllerComponent* compMotionCont;
	UPROPERTY() UPrimitiveComponent* compCollGrab;
	UPROPERTY() UPrimitiveComponent* compCollUi;
	UPROPERTY()  UCoreAnimInstance* animIns;
	UPROPERTY() AInteractActor* holdActor;
	bool isPressGrabKey;
	//UPROPERTY() UPickupActorInterface* holdActor2;
};
