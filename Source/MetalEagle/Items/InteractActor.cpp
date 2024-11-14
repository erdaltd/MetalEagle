// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractActor.h"
#include "Utils/CoreValue.h"




void AInteractActor::BeginPlay()
{
	Super::BeginPlay();

	if (physicComp)
	{
		RootComponent = physicComp;
	}
}

/* onHighlight & onDisableHighlight */
void AInteractActor::OnCanInteract(bool _status, AActor* _parent)
{
	BpEventOnHighlight(_status, "");
}
/* onpress & onrelease (just ui) */
void AInteractActor::OnPress(bool _status, AActor* _parent)
{
	if (Type == EInteractType::UI_ELEMENT)
	{
		BpEventOnPress(_status, "");
		if (_status)
		{
			//SetPhysicEnable(false);
			//SetCollisionPreset("NoCollision");
		}
		else
		{
			//SetPhysicEnable(true);
			//SetCollisionPreset("NoCollision");
		}
	}
}

/* hold & release */
void AInteractActor::OnHold(bool _status, AActor* _parent)
{
	if (Type == EInteractType::PICKUP)
	{
		BpEventOnHold(_status, "");
	}
}



