#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Utils/Resource.h"
#include <MediaSource.h>
#include "Controllers/TaskCont.h"
#include "Ui/WidgetActor.h"
#include "GameCont.generated.h"

class AVehicleCont;

UCLASS()
class METALEAGLE_API AGameCont : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameCont();

	

	virtual void BeginPlay() override;
	virtual void StartLevel();
	virtual void Tick(float DeltaTime) override;
	void PreBeginPlay();
	static AGameCont* INS;
	/* DebugMode : (0) no debug, (1) enable debug lines */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetalEagle) int DebugMode;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetalEagle) UIWidgetDebug* UIDebug;
	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventOnLevelStarted();

	UPROPERTY() UResource* Resource;
private:

	void checkVREnabled();
};


UCLASS()
class METALEAGLE_API AGameTakeoff : public AGameCont
{
	GENERATED_BODY()



public:
	virtual void BeginPlay() override;
	virtual void StartLevel() override;
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadOnly, Category = MetalEagle)  UTaskCont* TaskCont;
	UPROPERTY(EditAnywhere, Category = MetalEagle) AWidgetActor*  DebugPanel;
private:

};