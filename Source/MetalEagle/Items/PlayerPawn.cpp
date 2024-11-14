// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "MetalEagle.h"
#include "Utils/Resource.h"
#include "HandCont.h"


// Sets default values
APlayerPawn::APlayerPawn():ACorePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	cam = Cast<UCameraComponent>(UResource::INS->GetComp(this, "Camera"));
	CompBody = Cast<USkeletalMeshComponent>(UResource::INS->GetComp(this, "Body"));
	compBodyRoot = Cast<USceneComponent>(UResource::INS->GetComp(this, "BodyRoot"));
	if (CompBody)
	{
		animIns = Cast <UBodyAnimInstance>(CompBody->GetAnimInstance());
	}




	compVrRoot = UResource::INS->GetComp(this, "VrRoot");

	handLeft = Cast<AHandCont>(UResource::INS->GetChildActor(this, "LeftHand"));
	if (handLeft)
	{
		handLeft->Hand = EControllerHand::Left;
		handLeft->SetHandAnimTarget(animIns);
		handLeft->SetActive();
	}

	handRight = Cast<AHandCont>(UResource::INS->GetChildActor(this, "RightHand"));
	if (handRight)
	{
		handRight->Hand = EControllerHand::Right;
		handRight->SetHandAnimTarget(animIns);
		handRight->SetActive();
	}

	

	if (UMainCont::INS->IsVrMode)
	{
		if (cam) cam->SetWorldLocation(FVector::ZeroVector);
		//if (handLeft) handLeft->GetParentComponent()->SetWorldLocation(FVector::ZeroVector);
		//if (handRight) handRight->GetParentComponent()->SetWorldLocation(FVector::ZeroVector);
	}

	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (compBodyRoot && cam)
	{
		//if (!isCamBodySyncCompleted)
		//{
			//compBodyRoot->SetWorldLocation(cam->GetComponentLocation());
			//isCamBodySyncCompleted = true;
		//}

		
	}

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

