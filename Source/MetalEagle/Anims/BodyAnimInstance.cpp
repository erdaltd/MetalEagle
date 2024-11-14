// Fill out your copyright notice in the Description page of Project Settings.

#include "BodyAnimInstance.h"
#include "MetalEagle.h"
#include "MainCont.h"
#include "Items/HandCont.h"
#include "Items/PlayerPawn.h"

void UBodyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	player = Cast<APlayerPawn>(TryGetPawnOwner());
}

//#define r    "opencv-haar-classifier"


void UBodyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	TargetHandLeftTransform = GetHandTransform(EControllerHand::Left);
	TargetHandRightTransform = GetHandTransform(EControllerHand::Right);
	
}

FTransform UBodyAnimInstance::GetHandTransform(EControllerHand handId )
{
	FTransform r;
	//if (!player) player = Cast<APlayerPawn>(TryGetPawnOwner());
	if (!player)return r;

	FString handRigName;
	FString handSocketName;
	AHandCont* targetHand= 0;
	 FRotator rotOffset = FRotator::ZeroRotator;
	 FRotator rotScale = FRotator(1,1,1);

	if (handId == EControllerHand::Left)
	{
		handRigName = UMainCont::INS->BODY_RIG_HANDL;
		handSocketName = UMainCont::INS->BODY_RIG_HANDL_SOCKET;
		targetHand = player->handLeft;
		rotOffset = FRotator(13, 90, -86);

	}
	else if (handId == EControllerHand::Right)
		{
			handRigName = UMainCont::INS->BODY_RIG_HANDR;
			handSocketName = UMainCont::INS->BODY_RIG_HANDR_SOCKET;
			targetHand = player->handRight;
			rotScale = FRotator(-1, 1, -1);
			rotOffset = FRotator(-13, -90, -86);
		}
	if ( targetHand)
	{
		FVector rigLoc = player->CompBody->GetSocketLocation(FName(*handRigName));
		FVector socketLoc = player->CompBody->GetSocketLocation(FName(*handSocketName));
		FRotator socketRot = player->CompBody->GetSocketRotation(FName(*handSocketName));

		FVector loc = targetHand->GetMotionLoc() - (socketLoc - rigLoc);
		//Debug("erdal targetHand->GetActorLocation() " + targetHand->GetMotionLoc().ToString());
		//FVector loc = targetHand->GetParentComponent()->GetRelativeTransform().GetLocation() - (socketLoc - rigLoc);
		//FQuat rot = targetHand->GetParentComponent()->GetRelativeTransform().GetRotation();
		//FRotator rot = targetHand->GetMotionRelRot() + rotOffset;
		//FRotator rot = targetHand->GetMotionRot() * rotOffset;
		FRotator rot = targetHand->GetMotionRelRot() + player->CompBody->GetComponentRotation() + rotOffset;
		rot.Pitch *= rotScale.Pitch;
		rot.Yaw *= rotScale.Yaw;
		rot.Roll *= rotScale.Roll;
			
		r.SetLocation(loc);
		r.SetRotation(rot.Quaternion());
	}

	return r;
}
