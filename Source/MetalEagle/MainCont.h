// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameCont.h"
#include "MainCont.generated.h"


UCLASS()
class METALEAGLE_API UMainCont : public UGameInstance
{
	GENERATED_BODY()
		


public:
	UMainCont();
	static UMainCont* INS;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MetalEagle) bool IsVrMode;
	UFUNCTION(BlueprintCallable, Category = MetalEagle) AGameCont* GetGameCont() { return AGameCont::INS; };



	/* __________________________ CONFIG __________________________  */
	/* [ BODY SKELETAL MESH ] */
	UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FString BODY_RIG_HANDL = "hand_l";
	UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FString BODY_RIG_HANDL_SOCKET = "hand_lSocket";
	UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FString BODY_RIG_HANDR = "hand_r";
	UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FString BODY_RIG_HANDR_SOCKET = "hand_rSocket";
	/* ROTATION OFFSET BEETWEEN HAND RIG & MOTION CONTROLLER(ML) */ UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FRotator BODY_ROTOFFSET_HAND_TO_ML = FRotator(0, 90, 180);
	/*  */ UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FVector BODY_IK_HANDL_JOINT_TARGET = FVector(45, -50, 100);
	/*  */ UPROPERTY(BlueprintReadOnly, Category = "METALEAGLE_DEV_CONFIG") FVector BODY_IK_HANDR_JOINT_TARGET = FVector(-45, -50, 100);

	
};
