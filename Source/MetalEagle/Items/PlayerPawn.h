// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/CoreActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "Controllers/CorePawn.h"
#include "Anims/BodyAnimInstance.h"
#include "PlayerPawn.generated.h"

class AHandCont;

UCLASS()
class METALEAGLE_API APlayerPawn : public ACorePawn, public IActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY() USkeletalMeshComponent* CompBody;
	UPROPERTY() AHandCont* handLeft;
	UPROPERTY() AHandCont* handRight;
private:
	UPROPERTY() USceneComponent* compVrRoot;
	UPROPERTY() UCameraComponent* cam;
	UPROPERTY() UBodyAnimInstance* animIns;
	UPROPERTY() bool isCamBodySyncCompleted = false;


	
	
	USceneComponent* compBodyRoot;
};
