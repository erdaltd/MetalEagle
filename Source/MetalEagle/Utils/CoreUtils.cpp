
#include "CoreUtils.h"
#include "MetalEagle.h"
#include "GameCont.h"
#include "CoreValue.h"





/*____________________________________________________
/*___________________    #STRUCT    ___________________
/*___________________ COLLISION LIST __________________
/*____________________________________________________
*/
void FCollisionList::Add(FContactResult* _collInfo)
{
	List.Add(_collInfo);
}
void FCollisionList::Remove(FContactResult* _collInfo)
{
	List.Remove(_collInfo);
}

bool FCollisionList::Check(UPrimitiveComponent* _other)
{
	bool _result = 0;
	if (List.Num() > 0)
	{
		for (FContactResult* info : List)
		{
			if (info->toComp == _other)
				_result = true;
		}
	}
	return _result;
}
bool FCollisionList::GetIsValid()
{
	return (List.Num() > 0);
}


//FContactResult* FCollisionList::GetCollision(UOmComp* _other)
FContactResult* FCollisionList::GetCollision(UPrimitiveComponent* _other)
{
	FContactResult* _result = 0;
	if (List.Num() > 0)
	{
		for (FContactResult* info : List)
		{
			if (info->toComp == _other)
				_result = info;
		}
	}
	return _result;
}



TArray<FHitResult>  UCoreUtils::RayCast(ECollisionChannel _channel, FVector _from, FVector _to, bool _drawLine, bool _drawPersistance, bool _forceDebugLine)
{
	TArray<FHitResult> result;
	FCollisionQueryParams RV_TraceParams ;

	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;

	bool DidTrace = AGameCont::INS->GetWorld()->LineTraceMultiByChannel(
		result,        //result
		_from,        //start
		_to,        //end
		_channel,    //collision channel
		RV_TraceParams
	);
	if (_drawLine)
	{
		DrawLine(_from, _to, FColor::Green, _drawPersistance, _forceDebugLine);
	}
	return result;
}

void UCoreUtils::DrawLine(FVector _from, FVector _to, FColor _color, bool _persistance, bool _forceDebugLine)
{
	if (AGameCont::INS->DebugMode < 2 && !_forceDebugLine) return;
	_color =  _color.WithAlpha(64);
	DrawDebugLine(
		AGameCont::INS->GetWorld(),
		_from,
		_to,
		_color,
		_persistance, -1, 0,
		0.1f
	);
}

void UCoreUtils::DrawPoint(FVector _point, FColor _color, bool _persistance)
{
	if (AGameCont::INS->DebugMode < 2) return;
	DrawDebugPoint(
		AGameCont::INS->GetWorld(),
		_point,
		1,
		_color,
		_persistance, -1, 0
	);
}



OmCommandParam UCoreUtils::ParseCommand(FString _command)
{
	OmCommandParam _result;
	FString _func;
	TArray<FString> _params;



	if (_command.Len() > 0)
	{
		TArray<FString> _parseArr;
		_command.ParseIntoArrayWS(_parseArr, TEXT(":"));
		if (_parseArr.Num() > 0)
		{
			_func = _parseArr[0];
			_result.Key = _func;
			if (_parseArr.Num() > 1)
			{
				
				TArray<FString> _arrParams;
				_parseArr[1].ParseIntoArrayWS(_arrParams, TEXT(","));
				_result.Value = _arrParams;
				
			}
		}
	}



	return _result;

}

FTransform UCoreUtils::GetSubtractTrans(FTransform _trans1, FTransform _trans2)
{
	FTransform _result;
	_result.SetLocation(_trans2.GetLocation() - _trans1.GetLocation());
	_result.SetRotation(_trans2.GetRotation() - _trans1.GetRotation());
	return _result;
}

FTransform UCoreUtils::GetAdditionTrans(FTransform _trans1, FTransform _trans2)
{
	FTransform _result;
	_result.SetLocation(_trans1.GetLocation() + _trans2.GetLocation());
	_result.SetRotation(_trans1.GetRotation() + _trans2.GetRotation());
	return _result;
}


FString UCoreUtils::FVectorToCoreValueStr(FVector _vec) 
{
	FString _x = FString::FromInt(FMath::RoundToInt(_vec.X));
	FString _y = FString::FromInt(FMath::RoundToInt(_vec.Y));
	FString _z = FString::FromInt(FMath::RoundToInt(_vec.Z));
	FString _result = _x + "," + _y + "," + _z;
	return _result;
}
FVector UCoreUtils::CoreValueStrToFVector(FString _strData, FString _valueName)
{
	FVector v = FVector::ZeroVector;
	FCoreValue omV = FCoreValue(_strData);
	if (omV.GetValue(_valueName).IsValid)
		v = omV.GetValue(_valueName).GetFVector();
	return v;
}
float UCoreUtils::CoreValueStrToFloat(FString _strData, FString _valueName)
{
	float v = 0;
	FCoreValue omV = FCoreValue(_strData);
	if (omV.GetValue(_valueName).IsValid)
		v = omV.GetValue(_valueName).GetFloat(0);
	return v;
}

FRotator UCoreUtils::GetLimitedRotator(FRotator _origin, FRotator _limit)
{
	FRotator _result;
	_result.Pitch = FMath::Clamp(_origin.Pitch, -_limit.Pitch, _limit.Pitch);
	_result.Yaw = FMath::Clamp(_origin.Yaw, -_limit.Yaw, _limit.Yaw);
	_result.Roll = FMath::Clamp(_origin.Roll, -_limit.Roll, _limit.Roll);
	return _result;
}

TArray<FString> UCoreUtils::ParseString(FString _str, FString _delim, bool _isCulling)
{
	TArray<FString> _parseArr;
	_str.ParseIntoArray(_parseArr, *_delim);
	return _parseArr;
}




int UCoreUtils::ToInt(FString _intStr)
{
	return (int)FCString::Atoi(*_intStr);
}

float UCoreUtils::ToFloat(FString _intStr)
{
	return (float)FCString::Atof(*_intStr);
}


TArray<int8> UCoreUtils::ToIntArray(TArray<FString> _arrStr)
{
	TArray<int8> _arr;
	for (FString s : _arrStr)
	{
		TArray<FString> listRange = ParseString(s, "/");
		if (listRange.Num() > 0)
		{
			_arr.Add(ToInt(listRange[0]));
			_arr.Add(ToInt(listRange[1]));
		}
		else
		{
			_arr.Add(ToInt(s));
		}
		
	}
	return _arr;
}


TArray<float> UCoreUtils::ToFloatArray(TArray<FString> _arrStr)
{
	TArray<float> _arr;
	for (FString s : _arrStr)
	{
		TArray<FString> listRange = ParseString(s, "/");
		if (listRange.Num() > 0)
		{
			_arr.Add(ToFloat(listRange[0]));
			_arr.Add(ToFloat(listRange[1]));
		}
		else
		{
			_arr.Add(ToFloat(s));
		}

	}
	return _arr;
}




/* ______________________ # AttachActor / DetachActor  _______________________ */
//bool UCoreUtils::AttachActor(AActor* _actor, AActor* _parent, bool _isSnapTarget)
//{
//	bool r = 0;
//	if (_actor == _parent) return r;
//
//			Debug(_actor->GetName() + " + attact to -> " + _parent->GetName());
//			{ /* attach */
//				FAttachmentTransformRules attachRule = FAttachmentTransformRules::KeepWorldTransform;
//				if (_isSnapTarget) 	attachRule = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
//				//_actor->OwnerItem = this;
//				//if (!listConnectedItems.Contains(_actor)) listConnectedItems.Add(_actor);
//				
//
//				//if (_parent->CompRel)
//					//_actor->AttachToComponent(_parent->RootComponent, attachRule, NAME_None);
//				//else
//					_actor->AttachToActor(_parent, attachRule, NAME_None);
//
//				r = true;
//			}
//
//			{ /* set trans */
//				/*UProcessComp* parentComp = Cast<UProcessComp>(UResource::INS->GetOmComp(_params.GetValue("parentComp").GetString(0)));
//				if (parentComp)
//				{
//					FTransform trans = parentComp->GetTransByRatio(1);
//					_actor->SetActorTransform(trans);
//				}
//				else
//				{
//					FString connectPointName = _params.GetValue("ConnectPoint").GetString();
//					UOmComp* connectPoint = Cast<UOmComp>(_actor->GetComponent(connectPointName));
//					if (connectPoint)
//					{
//						_actor->SetActorRelativeLocation(_actor->RootComponent->GetRelativeTransform().GetLocation() - connectPoint->GetRelativeTransform().GetLocation());
//
//					}
//				}*/
//			}
//
//
//
//
//
//
//			//IsToConnected = true;
//			//BpEventOnStatus(EEventType::TO_CONNECTED);
//
//
//
//
//
//
//	return r;
//
//}
bool UCoreUtils::AttachCompToComp(USceneComponent* _comp, USceneComponent* _parentComp, bool _isSnapTarget)
{
	bool r = 0;
	if (_comp->GetOwner() == _parentComp->GetOwner()) return r;

	Debug(_comp->GetName() + " + attact to comp -> " + _parentComp->GetName());
	{ 
		FAttachmentTransformRules attachRule = FAttachmentTransformRules::KeepWorldTransform;
		if (_isSnapTarget) 	attachRule = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		_comp->AttachToComponent(_parentComp, attachRule, NAME_None);

		r = true;
	}

	return r;

}

bool UCoreUtils::DetachCompToComp(USceneComponent* _comp, USceneComponent* _parentComp )
{
	bool r = 0;
	if (_comp->GetOwner() == _parentComp->GetOwner()) return r;

		//if (listConnectedItems.Contains(_actor))
		{
			Debug(_comp->GetName() + " + detach to -> " + _parentComp->GetName());
			//listConnectedItems.Remove(_actor);
			//_actor->OwnerItem = nullptr;
			_comp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			r = true;


		}

	
	return r;
}





/* ------------------------ parse CoreValues ---------------------- */
FString UCoreUtils::GetCmdParam(FString _cmd, FString _name, int _cmdIndex, bool _toLower)
{
	FString r;
	FCommand c = FCommand(_cmd);
	if (c.IsValid)
		r = c.GetCoreValue(_cmdIndex).GetValue(_name).GetString(0);
	r = _toLower ? r.ToLower() : r;
	return r;
}
FString UCoreUtils::GetCmdParam_Func(FString _cmd,  int _cmdIndex)
{
	FString r;
	FCommand c = FCommand(_cmd);
	if (c.IsValid)
		r = GetCmdParam(_cmd, "cmd", _cmdIndex, true);
	return r;
}
bool UCoreUtils::GetCmdParam_Boolean(FString _cmd, FString _name, int _cmdIndex)
{
	bool r = false;
	FCommand c = FCommand(_cmd);
	if (c.IsValid)
		r = c.GetCoreValue(_cmdIndex).GetValue(_name).GetBool(0);
	return r;
}
float UCoreUtils::GetCmdParam_Float(FString _cmd, FString _name, int _cmdIndex)
{
	float r = 0;
	FCommand c = FCommand(_cmd);
	if (c.IsValid)
		r = c.GetCoreValue(_cmdIndex).GetValue(_name).GetFloat(0);
	return r;
}

//
//AItem* UCoreUtils::GetCmdParam_Item(FString _cmd, FString _name, int _cmdIndex)
//{
//	AItem* r=0;
//	FCommand c = FCommand(_cmd);
//	if (c.IsValid)
//		r = c.GetItem(_cmdIndex, _name);
//	return r;
//}

FVector UCoreUtils::GetCmdParam_Vec(FString _cmd, FString _name, int _cmdIndex)
{
	FVector r = FVector::ZeroVector;
	FCommand c = FCommand(_cmd);
	if (c.IsValid)
		r = c.GetCoreValue(_cmdIndex).GetValue(_name).GetFVector(false);
	return r;
}


FString UCoreUtils::FromVectorToString(std::vector<FString> _vec)
{
	FString str = "";
	for (int8 i = 0; i < (int)_vec.size(); i++)
	{
		str.Append(_vec[i]);
		if (i < _vec.size() - 1) str.Append(",");
	}
	return str;
}


FString UCoreUtils::FromArrayToString(TArray<FString> _arrStr)
{
	FString str = "";
	for (int8 i = 0; i < _arrStr.Num(); i++)
	{
		str.Append(_arrStr[i]);
		if (i < _arrStr.Num() - 1) str.Append(",");
	}
	return str;
}

FString UCoreUtils::IntToString(TArray<int8> _arr)
{
	FString str = "";
	int8 _total = _arr.Num();
	for (int8 i = 0; i < _total; i++)
	{
		str.Append(FString::FromInt(_arr[i]));
		if (i < _total - 1) str.Append(",");
	}
	return str;
}

FString UCoreUtils::FloatToString(TArray<float> _arr)
{
	FString str = "";
	int8 _total = _arr.Num();
	for (int8 i = 0; i < _total; i++)
	{
		str.Append(FString::SanitizeFloat(_arr[i]));
		if (i < _total - 1) str.Append(",");
	}
	return str;
}

FString UCoreUtils::FloatToString(float _float)
{
	FString str =  FString::SanitizeFloat(_float);
	return str;
}


FRotator UCoreUtils::FVectorToFRotator(FVector _vec)
{
	FRotator r = FRotator(_vec.Y, _vec.Z, _vec.X);
	return r;
}
