#include "GameCont.h"
#include "MetalEagle.h"
#include "IXRTrackingSystem.h"
#include "Runtime/HeadMountedDisplay/Public/IHeadMountedDisplay.h"
#include "Utils/Resource.h"
#include "MainCont.h"
#include "Core/CoreActor.h"


AGameCont* AGameCont::INS = NULL;

AGameCont::AGameCont()
{
	PrimaryActorTick.bCanEverTick = true;
	AGameCont::INS = this;
}

void AGameCont::PreBeginPlay()
{
	
	/* init & load resource */
	Resource = NewObject<UResource>();
	Resource->Init();
}



void AGameCont::BeginPlay()
{

	checkVREnabled();


	Super::BeginPlay();


	

	//GetWorld()->Exec(GetWorld(), TEXT("stat fps"));
	 //Debug("run!!!", DebugMode);
	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AGameCont::StartLevel, 1, false);
}


void AGameCont::StartLevel()
{
	

}
	

void AGameCont::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameCont::checkVREnabled()
{
	if (GEngine->XRSystem.IsValid())
	if (GEngine->XRSystem->GetHMDDevice()->IsHMDEnabled()) //check pointer is valid
	{
		if (GEngine->XRSystem->GetHMDDevice()->IsHMDConnected()) 
		if (GEngine->XRSystem->IsHeadTrackingAllowed())
		{
			UMainCont::INS->IsVrMode = true;
		}
		else
		{
			UMainCont::INS->IsVrMode = false;
		}
	}
	else
	{
		UMainCont::INS->IsVrMode = false;
	}
}





/*-----------------------------[ LEVEL TAKEOFF */

void AGameTakeoff::BeginPlay()
{
	TaskCont = NewObject<UTaskCont>();
	TaskCont->Init();

	Super::BeginPlay();

	

}


void AGameTakeoff::StartLevel()
{

	Super::StartLevel();

	

	BpEventOnLevelStarted();
	Debug("start takeoeff");

	TaskCont->SetTask(0);
}