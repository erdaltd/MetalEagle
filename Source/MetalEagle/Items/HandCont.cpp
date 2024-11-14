
#include "HandCont.h"
#include "MetalEagle.h"
#include "Core/CoreAnimInstance.h"
#include <MotionControllerComponent.h>
#include "Items/InteractActor.h"
#include "Controllers/CorePawn.h"
#include "GameCont.h"




void AHandCont::BeginPlay()
{
	compMotionCont = Cast< UMotionControllerComponent>(UResource::INS->GetComp(this, "MotionController"));
	compCollGrab = Cast< UPrimitiveComponent>(UResource::INS->GetComp(this, "GrabCollision"));
	compCollUi = Cast< UPrimitiveComponent>(UResource::INS->GetComp(this, "UiCollision"));
	//animIns = Cast <UCoreAnimInstance>( compHandMesh->GetAnimInstance());
	if (compCollGrab)
	{
		compCollGrab->OnComponentBeginOverlap.AddDynamic(this, &AHandCont::BeginOverlap);
		compCollGrab->OnComponentEndOverlap.AddDynamic(this, &AHandCont::EndOverlap);
	}
	if (compCollUi)
	{
		compCollUi->OnComponentBeginOverlap.AddDynamic(this, &AHandCont::BeginOverlap);
		compCollUi->OnComponentEndOverlap.AddDynamic(this, &AHandCont::EndOverlap);
	}
	Super::BeginPlay();
}


void AHandCont::SetActive()
{
	if (Hand == EControllerHand::Left)
	{
		const FName handType = TEXT("Left");
		compMotionCont->MotionSource = handType;
	}
	else if (Hand == EControllerHand::Right)
	{
		const FName handType = TEXT("Right");
		compMotionCont->MotionSource = handType;
	}
}





void AHandCont::SetHandAnimTarget(UCoreAnimInstance* _animIns)
{
	animIns = _animIns;
}

void AHandCont::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	isPressGrabKey = (Hand == EControllerHand::Left) ? ACorePawn::INS->PressGrabLeft : ACorePawn::INS->PressGrabRight;
	if (listOverlaps.IsValidIndex(0))
	{
		//for (FContactResult contact : listOverlaps)
		for (int8 i = 0; i < listOverlaps.Num(); i++)


		{
			FContactResult contact = listOverlaps[i];
			AInteractActor* interactActor = Cast<AInteractActor>(contact.toActor);
			if (interactActor)
			{
				if (contact.fromComp == compCollGrab)
				{/* grab functions */
					if (interactActor->Type == EInteractType::PICKUP)
					{
						if (isPressGrabKey)
						{
							Hold((AActor*)interactActor);
						}
					}
				}
				else if (contact.fromComp == compCollUi)
				{/* ui functions */
					if (interactActor->Type == EInteractType::UI_ELEMENT)
					{
						if (isPressGrabKey)
						{
							interactActor->OnPress(true, (AActor*)this);
							RunAnimFunction("CanGrapHand");
						}
						else
						{
							interactActor->OnPress(false, (AActor*)this);
							RunAnimFunction("OpenHand");
						}
					}
				}
			}

		}
	}
	else
	{
		if (isPressGrabKey)
		{
			RunAnimFunction("GrapHand");
		}
		else
		{
			RunAnimFunction("OpenHand");
		}
	}
	
	

	if (holdActor)
	{
		if (!isPressGrabKey)
		{
			Release();
		}
	}
}

void AHandCont::OnOverlap(bool _status, FContactResult _contact)
{
	Super::OnOverlap(_status, _contact);

	AInteractActor* interactActor = Cast<AInteractActor>(_contact.toActor);

	if (_status)
	{
		
		if (interactActor)
		{

			if (
				(_contact.fromComp == compCollGrab && interactActor->Type == EInteractType::PICKUP) 
				//(_contact.fromComp == compCollUi && interactActor->Type == EInteractType::UI_ELEMENT)
				)
			{
				Debug("interact to actor " + _contact.toActor->GetName());
				interactActor->OnCanInteract(true, this);
				RunAnimFunction("CanGrapHand");
			}
			else if (_contact.fromComp == compCollUi && interactActor->Type == EInteractType::UI_ELEMENT)
			{
				interactActor->OnCanInteract(true, this);
			}

			
		
		}
		else
		{
			Debug("interact actor degil " + _contact.toActor->GetName());
			//interactActor->OnCanInteract(true, false);
			RunAnimFunction("OpenHand");
		}
	}
	else
	{
		if (!holdActor)
		{
			RunAnimFunction("OpenHand");
		}
		if (interactActor) interactActor->OnCanInteract(false, false);
	}
}

FVector AHandCont::GetMotionLoc()
{
	FVector r = FVector::ZeroVector;
	if (compMotionCont) r = compMotionCont->GetComponentLocation();
	return r;

}
FRotator AHandCont::GetMotionRot()
{
	FRotator r = FRotator::ZeroRotator;
	if (compMotionCont) r = compMotionCont->GetComponentRotation();
	return r;

}

FRotator AHandCont::GetMotionRelRot()
{
	FRotator r = FRotator::ZeroRotator;
	if (compMotionCont) r = compMotionCont->GetRelativeTransform().GetRotation().Rotator();
	return r;

}





void AHandCont::Hold(AActor* _actor)
{
	AInteractActor* interactActor = Cast<AInteractActor>(_actor);

	if (interactActor)
	{
		interactActor->SetPhysicEnable(false);
		interactActor->SetCollisionPreset("NoCollision");
		if (UCoreUtils::AttachCompToComp(_actor->GetRootComponent(), (USceneComponent*)compMotionCont, false))
		{
			holdActor = interactActor;
			RunAnimFunction("GrapHand");
			interactActor->OnHold(true, (AActor*)this);

				Debug("coreActor disable edildi " + interactActor->GetName());
		}
	}
	
	
	


}

void AHandCont::Release()
{
	if (holdActor)
	{
		if (UCoreUtils::DetachCompToComp(holdActor->GetRootComponent(), (USceneComponent*)compMotionCont))
		{
			holdActor->SetPhysicEnable(true);
			holdActor->ResetCollisionPreset();
			holdActor->OnHold(false, (AActor*)this);
			holdActor = 0;
			RunAnimFunction("OpenHand");
		}
	}


	

	
}


void AHandCont::RunAnimFunction(FString _funcName)
{
	FString handId = (Hand == EControllerHand::Left) ? "Left" : "Right";
	if(animIns) animIns->Run("func:("+ _funcName + "_"+handId +")");
}