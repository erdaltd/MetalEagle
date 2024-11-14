
#pragma once

#include <vector>
#include "CoreUtils.generated.h"


class AItem;
class UOmComp;
struct FCoreValue;
typedef TPair<FString, TArray<FString>> OmCommandParam;


UCLASS(Blueprintable)
class METALEAGLE_API UCoreUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static TArray<FHitResult>  RayCast(ECollisionChannel _channel, FVector _from, FVector _to, bool _drawDEbug = false, bool _persistance = false, bool _forceDebugLine = false);
	static void DrawLine(FVector _from, FVector _to, FColor _color, bool _persistance = false, bool _forceDebugLine = false);
	static void DrawPoint(FVector _point, FColor _color, bool _persistance = false);
	static OmCommandParam ParseCommand(FString _command);
	static FTransform GetSubtractTrans(FTransform _trans1, FTransform _trans2);
	static FTransform GetAdditionTrans(FTransform _trans1, FTransform _trans2);
	static FString	FVectorToCoreValueStr(FVector _vec);
	static FVector CoreValueStrToFVector(FString _strData, FString _valueName);
	static	float CoreValueStrToFloat(FString _strData, FString _valueName);
	static FRotator	GetLimitedRotator(FRotator _origin, FRotator _limit);
	static TArray<FString> ParseString(FString _str, FString _delim, bool _isCulling=true);
	static int	ToInt(FString _intStr);
	static float ToFloat(FString _intStr);
	static FString	FromVectorToString(std::vector<FString> _vec);
	static FString FromArrayToString(TArray<FString> _arrStr);
	static	FString IntToString(TArray<int8> _arr);
	static	FString FloatToString(TArray<float> _arr);
	static FString FloatToString(float _float);
	static	FRotator FVectorToFRotator(FVector _vec);
	static TArray<int8> ToIntArray(TArray<FString> _arrStr);
	static TArray<float> ToFloatArray(TArray<FString> _arrStr);
	//static bool AttachActor(AActor* _actor, AActor* _parent, bool _isSnapTarget);
	static bool AttachCompToComp(USceneComponent* _comp, USceneComponent* _parentComp, bool _isSnapTarget);
	static bool DetachCompToComp(USceneComponent* _comp, USceneComponent* _parentComp);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE)	static FString GetCmdParam(FString _cmd, FString _name, int _cmdIndex, bool _toLower = true);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE)	static FString GetCmdParam_Func(FString _cmd,  int _cmdIndex);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE) static bool GetCmdParam_Boolean(FString _cmd, FString _name, int _cmdIndex);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE) static float GetCmdParam_Float(FString _cmd, FString _name, int _cmdIndex);
	UFUNCTION(BlueprintCallable, Category = METALEAGLE)	static	FVector GetCmdParam_Vec(FString _cmd, FString _name, int _cmdIndex);
};





/* ___________________ #FContactResult ___________________ */
USTRUCT(BlueprintType)
struct METALEAGLE_API FContactResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)	AActor* fromActor;
	UPROPERTY(BlueprintReadWrite)	UPrimitiveComponent* fromComp;
	UPROPERTY(BlueprintReadWrite)	AActor* toActor;
	UPROPERTY(BlueprintReadWrite)	UPrimitiveComponent* toComp;
	UPROPERTY(BlueprintReadWrite) TArray<FVector> ArrPoints;
	UPROPERTY(BlueprintReadWrite) FVector point;
	FContactResult() {}
	
	/* for collision */
	FContactResult(UPrimitiveComponent* _fromComp, UPrimitiveComponent* _toComp)
	{
		toComp = _toComp;
		toActor = toComp->GetOwner();
		fromComp = _fromComp;
		fromActor = fromComp->GetOwner();
	}

	FString ToString()
	{
		FString fi = "";//fromItem ? fromItem->GetName() : "";
		FString p = point.ToString();
		FString s = "fromActor:(" + fi + "), loc("+p+")";
		return s;
	}
};

/* ___________________ #FCollisionList ___________________ */
USTRUCT()
struct METALEAGLE_API FCollisionList
{
	GENERATED_USTRUCT_BODY()

	TArray<FContactResult*> List;
	FCollisionList() {}
	void Add(FContactResult* _collInfo);
	void Remove(FContactResult* _collInfo);
	bool GetIsValid();
	bool Check(UPrimitiveComponent* _otherOmBody);
	FContactResult* GetCollision(UPrimitiveComponent* _otherOmBody);
};




typedef TPair<FTransform, FTransform> PairTrans;

/* <---------- Status -------------> */
UENUM(BlueprintType)
enum class EStatus : uint8
{
	ON_PRE_START,
	ON_STARTED,
	ON_PROGRESS,
	ON_PROGRESS_ERR,
	ON_ENDED,
	ON_COMPLETED,
	ON_COMPLETED_FAIL,
	ON_COMPLETED_SUCC
};


