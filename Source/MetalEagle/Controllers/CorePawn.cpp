// Fill out your copyright notice in the Description page of Project Settings.

#include "CorePawn.h"



ACorePawn * ACorePawn::INS = NULL;



ACorePawn::ACorePawn()
{
	ACorePawn::INS = this;
	//PrimaryActorTick.bCanEverTick = true;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ACorePawn::SetupPlayerInputComponent(class UInputComponent* _InputComp)
{
	Super::SetupPlayerInputComponent(_InputComp);

	



	//_InputComp->BindAxis("GrabLeft", this, &AInputs::InputKeyAxisUpDown);



	_InputComp->BindAction("GrabLeft", IE_Pressed, this, &ACorePawn::pressGrapLeft);
	_InputComp->BindAction("GrabLeft", IE_Released, this, &ACorePawn::_pressGrapLeft);
	_InputComp->BindAction("GrabRight", IE_Pressed, this, &ACorePawn::pressGrapRight);
	_InputComp->BindAction("GrabRight", IE_Released, this, &ACorePawn::_pressGrapRight);


	//if (!AMainCont::INS->IsHapticMode) Cast<UMouseInput>(InputManager)->SetupPlayerInputComponent(_InputComp);


}

/* INPUT PRESS (ACTION) */
void ACorePawn::pressGrapLeft()
{
	PressGrabLeft = true;
}
void ACorePawn::_pressGrapLeft()
{
	PressGrabLeft = false;
}
void ACorePawn::pressGrapRight()
{
	PressGrabRight = true;
}
void ACorePawn::_pressGrapRight()
{
	PressGrabRight = false;
}
