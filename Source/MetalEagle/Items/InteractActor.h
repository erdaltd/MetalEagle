// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreActor.h"
#include "InteractActor.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	NONE,
	UI_ELEMENT  UMETA(DisplayName = "UI_ELEMENT"),
	PICKUP  UMETA(DisplayName = "PICKUP")

};
UCLASS()
class METALEAGLE_API AInteractActor : public ACoreActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MetalEagle) EInteractType Type;

	virtual void BeginPlay() override;
	void OnCanInteract(bool _status, AActor* _parent);
	void OnPress(bool _status, AActor* _parent);
	void OnHold(bool _status, AActor* _parent);

	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventOnHighlight(const bool Status, const FString &Params);
	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventOnPress(const bool Status, const FString &Params);
	UFUNCTION(BlueprintImplementableEvent, Category = METALEAGLE) void BpEventOnHold(const bool Status, const FString &Params);
protected:
	TMap<FString, USceneComponent*> listAllComps;
	
	
};
