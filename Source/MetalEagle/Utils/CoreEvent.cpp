#include "CoreEvent.h"
#include "MetalEagle.h"



void UEvent::SetType(EEventType __type)
{
	_type = __type;
}

void UEvent::Destroy()
{
	_target = nullptr;
	_listener = nullptr;
	_params = nullptr;
	FMemory::Free(_target);
	FMemory::Free(_listener);
	FMemory::Free(_params);
}


UEventDispatcher::UEventDispatcher() {
}

void UEventDispatcher::addEventListener(EEventType _type, void* _listener, UEventFunctionPtr handler, bool useCapture, int priority, bool useWeakReference)
{

	FEventData data;
	data.listener = _listener;
	data.handler = handler;


	if (list.Contains((int)_type))
	{
		
		list[(int)_type].Add(data);
	}
	else
	{
		TArray<FEventData> newArr;
		newArr.Add(data);
		list.Add((int)_type, newArr);
	}




	

}


void UEventDispatcher::dispatchEvent(EEventType type, void* __target, void* _params)
{
	UObject* tarr = (UObject*)__target;
	if (!hasEventListener(type)) return;

	UEvent* event = NewObject<UEvent>();
	event->SetType(type);
	event->_target = __target;
	if (_params != 0) event->_params = _params;
	UObject* tar = (UObject*)__target;
	TArray<FEventData>  arrEventData = list[(int)type];
	for (FEventData edata : arrEventData) {
		if (edata.listener)
		{
			event->_listener = edata.listener;
			(*edata.handler)(*event);


		}

	}



}






void UEventDispatcher::removeEventListener(EEventType _type, UEventFunctionPtr _handler)
{
	if (!hasEventListener(_type)) return;
	TArray<FEventData>  funcList = list[_type];
	
	int8 removeIndex = -1;
	for (int8 i = 0; i < funcList.Num(); i++)
	{
		FEventData eventData = funcList[i];
		if (eventData.handler == _handler)
		{
			removeIndex = i;
		}
	}

	if(removeIndex > -1) list[(int)_type].RemoveAt(removeIndex);



}
bool UEventDispatcher::hasEventListener(EEventType type)
{
	return list.Contains((int)type);

}

void UEventDispatcher::Destroy()
{



}






