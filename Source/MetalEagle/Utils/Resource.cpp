#include "Resource.h"
#include "MetalEagle.h"
#include "Core/CoreActor.h"
#include "GameCont.h"


UResource * UResource::INS = NULL;



UResource::UResource()
{
	INS = this;
	//eventDisp = NewObject<UEventDispatcher>();

	
		

}


void UResource::Init()
{

	createObjects();

	//class UObject* classTest = GetClass("test_actor");
	 //UObject* classTest = GetClass("test_actor");
	//UClass* c = classTest::StaticClass();
	//if (classTest) Debug("classTest sorun yok");
	//if (c) Debug("classTest UClass sorun yok");
	
	//GetWorld()->SpawnActor<ACoreActor>(classTest->StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

}

void UResource::createObjects()
{

	int8 indexOfItem = 0;
	for (TActorIterator<AActor> act(AGameCont::INS->GetWorld()); act; ++act)
	{
		AActor* actor = Cast<AActor>(*act);
		listActors.Add(actor->GetName(), actor);
		//Debug("UResource::actor " + actor->GetName());
		//AToolItem* toolItem = Cast<AToolItem>(*actor);
		//AEqItem* eq = Cast<AEqItem>(*actor);

		//if(item)
		//{
			//FString idItem = item->Create(indexOfItem);
			
			//AddItemToList(idItem, item);
			//indexOfItem++;

	}
		

		//if (eq)
		//{
			//listEquipments.Add(eq->Id, eq);
		//}

		//else if (toolItem)
		//{
		/*	ListToolItems.Add(toolItem->Id, toolItem);
		*/
//}


	//}


	
	
}




bool  UResource::AddItemToList(FString _id, AActor* _item)
{
	bool result = false;
	if (!listActors.Contains(_id))
	{
		listActors.Add(_id, _item);
		//TArray<UOmComp*> arr;
		TArray<USceneComponent*> arr;
		_item->GetComponents(arr);
		int8 indexOfComp = 0;
		for (USceneComponent* comp : arr)
		{

			FString idComp = _item->GetName() + "/" + comp->GetName(); //generate id
			listComps.Add(idComp, comp);
			//UOmComp* omComp = Cast<UOmComp>(comp);
			//if (omComp)
			//{
				//omComp->OwnerItem = _item;
				//omComp->Create(indexOfComp, idComp);
			//}
			
			indexOfComp++;
		}
		result = true;
	}
	
	return result;
}


//class UObject* UResource::GetClass(FString _id)
//{
//	class UObject* c = 0;
//	c = AGameCont::INS->ResourceClasses[_id];
//	return c;
//}



AActor* UResource::GetActor(FString _id, bool _fatal /*= false*/)
{
	AActor* obj = 0;
	if (listActors.Find(_id))
	{
		obj = listActors[_id];
	}
	else {
		//if (_fatal) Debug::Fatal("The Item " + _id + " not found!");
	}
	return obj;
}

USceneComponent* UResource::GetComp(FString _id, bool _fatal /*= false*/)
{
	USceneComponent* comp = 0;
	if (listComps.Find(_id))
	{
		comp = listComps[_id];
	}
	else {
		//if (_fatal) Debug::Fatal("The Item " + _id + " not found!");
	}
	return comp;
}

USceneComponent* UResource::GetComp(AActor* _actor, FString _id)
{
	USceneComponent* comp = 0;
	if (!_actor) return 0;

	TArray<USceneComponent*> arrAll;
	_actor->GetComponents(arrAll,true);
	int8 indexOfComp = 0;
	for (USceneComponent* c : arrAll)
	{
		if (c->GetName() == _id) comp = c;
	}

	return comp;
}


AActor* UResource::GetChildActor(AActor* _actor, FString _compName)
{
	AActor* a = 0;
	UChildActorComponent* caComp = Cast<UChildActorComponent>(GetComp(_actor, _compName));
	if (caComp) a = caComp->GetChildActor();
	return a;

}
