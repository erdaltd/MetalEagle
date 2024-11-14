#pragma once

#include "CoreEvent.generated.h"

UENUM(BlueprintType)
enum EEventType
{
	INIT,
	START,
	UPDATE,
	END,
	SUCCESS,
	ERROR,
	COMPLETE,
	PAWN_START,
	PAWN_UPDATE,
	PAWN_END, //pawn sonucucnun observera gonderilmeye hazir oldugu durumlar
	PAWN_SUCCESS, //pawn sonucu observer disinda belirlenen durumlar
	PAWN_COMPLETE, //pawn sonucunun observer tarafindan belirlenen ve pawnin taamlandigi durumlar
	ON_COLLISION,
	ON_COLLISION_EXIT,
	ON_HIT,
	INPUT_MOVE_x,
	INPUT_MOVE_Y,
	INPUT_MOUSE_DOWN,
	INPUT_MOUSE_UP,
	ON_CLICK,
	ON_CLOSE_UI,
	ON_CALL_FROM_BP,
	TO_CONNECTED,
	FROM_CONNECTED,
	TO_UNCONNECTED,
	FROM_UNCONNECTED,
	INPUT_KEY_PRESS,
	INPUT_KEY_DOUBLE_CLICK,
	INPUT_KEY_RELEASE,
	LOOK_AT_START,
	LOOK_AT_END,
	DEBUG,
};



class UEventDispatcher;


//UCLASS()
UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class METALEAGLE_API UEvent : public UObject
{
	GENERATED_BODY()
		friend UEventDispatcher;
public:
	UEvent() {};
	void SetType(EEventType type);
	void Destroy();
	void *target() { return _target; };
	void *listener() { return _listener; };
	void *params() { return _params; };
	EEventType  type() { return _type; }
private:
	EEventType _type;
	void *_target;
	void *_listener;
	void *_params;
};

typedef void(*UEventFunctionPtr)(UEvent &UEvent);


USTRUCT()
struct FEventData
{
	GENERATED_USTRUCT_BODY()
		void* listener;
	UEventFunctionPtr handler;

	void Destroy()
	{
		//FMemory::Free(listener);
	}
};

USTRUCT()
struct FEventPair
{
	GENERATED_USTRUCT_BODY()

		EEventType type;
	UPROPERTY()	TArray<FEventData> arr;

	FEventPair() { };
	FEventPair(EEventType _type) :type(_type) { };
};


UCLASS()
class METALEAGLE_API UEventDispatcher : public UObject
{
	GENERATED_BODY()


public:

	UEventDispatcher();
	//~UEventDispatcher();
	void dispatchEvent(EEventType type, void* _target, void* _params = 0);
	void addEventListener(EEventType type, void* listener, UEventFunctionPtr handler, bool useCapture = false, int priority = 0, bool useWeakReference = false);
	void removeEventListener(EEventType type, UEventFunctionPtr handler);
	bool hasEventListener(EEventType type);
	void Destroy();
private:
	TMap<int, TArray<FEventData> > list;

};
