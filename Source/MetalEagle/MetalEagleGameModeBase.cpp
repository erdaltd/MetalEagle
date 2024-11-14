// Fill out your copyright notice in the Description page of Project Settings.

#include "MetalEagleGameModeBase.h"
#include "GameCont.h"




void AMetalEagleGameModeBase::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	if(AGameCont::INS) AGameCont::INS->PreBeginPlay();
}




