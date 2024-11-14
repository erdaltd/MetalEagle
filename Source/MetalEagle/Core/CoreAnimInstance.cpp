// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreAnimInstance.h"
#include "MetalEagle.h"
#include "Utils/CoreValue.h"




void UCoreAnimInstance::Run(FString cmd)
{
	FCoreValue cValue = FCoreValue(cmd);
	if (cValue.IsValid)
	{
		FString strFunc = cValue.GetValue("func").GetString();
		BpEventRun(strFunc, cmd);
	}
	
}
