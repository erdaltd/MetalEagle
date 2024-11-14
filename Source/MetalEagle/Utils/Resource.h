#pragma once

#include <vector>
#include "Resource.generated.h"





/*_____________________ Resource _____________________ */
UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class  METALEAGLE_API UResource : public UObject
{
	GENERATED_BODY()
public:
	UResource();
	static UResource* INS;
	void Init();
	UFUNCTION(BlueprintCallable, Category = METALEAGLE) USceneComponent* GetComp(FString _id, bool _fatal = false);
	USceneComponent* GetComp(AActor* _actor, FString _id);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE) AActor* GetChildActor(AActor* _actor, FString _compName);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE) AActor* GetActor(FString _id, bool _fatal = false);
	bool AddItemToList(FString _id, AActor* _item);
	//UFUNCTION(BlueprintCallable, Category = METALEAGLE) class UObject* GetClass(FString _id);

private:
	UPROPERTY() TMap<FString, AActor*> listActors;
	UPROPERTY() TMap<FString, USceneComponent*> listComps;
	void createObjects();
};









