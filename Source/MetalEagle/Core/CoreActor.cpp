#include "CoreActor.h"
#include "MetalEagle.h"
#include "Utils/CoreValue.h"






// Sets default values
ACoreActor::ACoreActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACoreActor::BeginPlay()
{



	TArray<USceneComponent*> _listComps;
	GetComponents(_listComps,true);
	for (USceneComponent* c : _listComps)
	{
		listAllComps.Add(c->GetName(), c);
		UPrimitiveComponent* primComp = Cast<UPrimitiveComponent>(c);
		if (primComp)
		{
			if (primComp->BodyInstance.bSimulatePhysics)
			{
				physicComp = primComp;
				Debug("physicComp : " + GetName() + "  /  " + physicComp->GetName());
			}
			listCollisionComps.Add(primComp->GetCollisionProfileName().ToString(), primComp);
		}
	}
	
	Super::BeginPlay();

	
}

USceneComponent* ACoreActor::GetComp(FString _name)
{
	USceneComponent* c = 0;
	if (listAllComps.Contains(_name))
		c = listAllComps[_name];
	return c;
}

// Called every frame
void ACoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoreActor::OnOverlap(bool _status, FContactResult _contact)
{

}

void ACoreActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor != this)
	{
		int8 foundIndex = findOverlapInList(OverlappedComponent, OtherComp);

		if (foundIndex == -1)
		{
			FContactResult newContact = FContactResult(OverlappedComponent, OtherComp);

			listOverlaps.Add(newContact);
			OnOverlap(true, newContact);
			Debug("(+) Overlap " + newContact.ToString());
		}
		
	}
}

void ACoreActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		int8 foundIndex = findOverlapInList(OverlappedComponent, OtherComp);
		
		if (foundIndex > -1)
		{
			FContactResult contact = listOverlaps[foundIndex];
			Debug("(-) Overlap " + contact.ToString());
			OnOverlap(false, contact);
			listOverlaps.RemoveAt(foundIndex);

			
		}
		
	}
}


int8 ACoreActor::findOverlapInList(UPrimitiveComponent* _fromComp, UPrimitiveComponent* _toComp)
{
	int8 r = -1;
	int8 i = 0;
	for (FContactResult contact : listOverlaps)
	{
		if (contact.fromComp == _fromComp && contact.toComp == _toComp) r = i;
		i++;
	}
	return r;
}



void ACoreActor::SetPhysicEnable(bool _enable)
{
	if (physicComp)
	{
		physicComp->SetSimulatePhysics(_enable);
		Debug( physicComp->GetName() + " SetSimulatePhysics : ", _enable);
	}


}



void ACoreActor::Run(FString cmd)
{
	FCoreValue cValue = FCoreValue(cmd);
	if (cValue.IsValid)
	{
		FString strFunc = cValue.GetValue("func").GetString();
		BpEventRun(strFunc, cmd);
	}
}

void ACoreActor::SetCollisionPreset(FString _presetName)
{
	for (auto& Elem : listCollisionComps)
	{
		//FString defaultPreset = Elem.Value;
		UPrimitiveComponent* comp = Elem.Value;
		comp->SetCollisionProfileName(FName(*_presetName));
		Debug(comp->GetName() + " SetCollisionPreset : "+ _presetName);
	}


}
void ACoreActor::ResetCollisionPreset()
{
	for (auto& Elem : listCollisionComps)
	{
		FString defaultPreset = *Elem.Key;
		UPrimitiveComponent* comp = Elem.Value;
		comp->SetCollisionProfileName(FName(*defaultPreset));
		Debug(comp->GetName() + " SetCollisionPreset : "+ defaultPreset);
	}

}
