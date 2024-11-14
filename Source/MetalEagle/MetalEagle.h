// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "MainCont.h"
#include "CoreMinimal.h"

class UMainCont;

#define MainCont UMainCont::INS;


class  Debug
{
public:

	/****************[ UE_LOG ]*********************/

	Debug(FString _msg)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *_msg);
		//if(_onScreen) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, *_msg);
	}


	Debug(FString _label, float _float)
	{
		FString _msg = _label.Len() > 0 ? _label + "-> " : "";
		Debug(_msg + *FString::SanitizeFloat(_float));
	}
	Debug(FString _label, int _int)
	{
		FString _msg = _label.Len() > 0 ? _label + "-> " : "";
		Debug(_msg + *FString::FromInt(_int));
	}
	Debug(FString _label, bool _bool)
	{
		FString _msg = _label.Len() > 0 ? _label + "-> " : "";
		FString _strBool = _bool ? "true" : "false";
		Debug(_msg + _strBool);
	}


	static void Fatal(FString _msg)
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s"), *_msg);
	}
	static void Warning(FString _msg)
	{
		FString m = "#OMCEMERR -> " + _msg;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *m);

	}





};
