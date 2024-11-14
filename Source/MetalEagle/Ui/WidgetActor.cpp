// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetActor.h"
#include "MetalEagle.h"




void AWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	for (auto& Elem : listAllComps)
	{
		USceneComponent* c = Elem.Value;
		if (Cast<UWidgetComponent>(c)) compWidget = Cast<UWidgetComponent>(c);
	}
	
	
	if(compWidget) ui = Cast< UCoreUserWidget>( compWidget->GetUserWidgetObject());
	
}



void AWidgetActor::Print(FString _id, FString _msg)
{
	if (!ui) return;
	if (_id == "debug")
	{
		ui->Run("func:(Debug),msg:(" + _msg + ")");
	}
	else if (_id == "time")
	{
		ui->Run("func:(Time),msg:(" + _msg + ")");
	}
}




