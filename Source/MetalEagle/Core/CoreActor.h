#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "Utils/CoreUtils.h"
#include "Utils/Resource.h"
#include "CoreActor.generated.h"


UINTERFACE(BlueprintType)
class METALEAGLE_API UActorInterface : public UInterface
{
	GENERATED_BODY()
};

class METALEAGLE_API IActorInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		int32 OnBroadcastReceived(const FString& Message);
};

//UINTERFACE(BlueprintType)
//class METALEAGLE_API UPickupActorInterface : public UInterface
//{
//	GENERATED_BODY()
//};
//
//class METALEAGLE_API IPickupActorInterface
//{
//	GENERATED_BODY()
//public:
//
//};



UCLASS()
class METALEAGLE_API ACoreActor : public AActor
{
	GENERATED_BODY()

public:
	ACoreActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// Overlap UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult
	virtual void OnOverlap(bool _status, FContactResult _contact);
	UFUNCTION()		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 	bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SetCollisionPreset(FString _presetName);
	void ResetCollisionPreset();
	void SetPhysicEnable(bool _enable);

	UFUNCTION(Category = METALEAGLE) virtual void Run(FString cmd);
	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventRun(const FString &Function, const FString &Params);

protected:
	TMap<FString, USceneComponent*> listAllComps;
	TArray< FContactResult> listOverlaps;
	UPrimitiveComponent* physicComp;
private:
	
	USceneComponent* GetComp(FString _name);
	
	int8 findOverlapInList(UPrimitiveComponent* _fromComp, UPrimitiveComponent* _toComp);

	
	
	TMap<FString, UPrimitiveComponent*> listCollisionComps;
};
