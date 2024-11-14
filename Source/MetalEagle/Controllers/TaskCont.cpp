#include "TaskCont.h"
#include "MetalEagle.h"
#include<stdarg.h>
#include "Utils/Resource.h"
#include "GameCont.h"
#include "Core/CoreActor.h"


UTaskCont* UTaskCont::INS = NULL;
FString UTask::IDPREFIX = "task";

FString URule::IDPREFIX = "rule";


/*___________________ TASK CONTROLLER ________________ 
#Process key patterns :
{ int }		-> regular key. value[0] == key[0]
{ int,int }	-> array keys.  value[0] == key[0] || value[1] == key[1] || value[n] == key[n] || ....
{ int:int }	-> clamps keys. value[0] >= key[0] && value[0] <= key[1]
*/
 UTaskCont::UTaskCont()
{
	UTaskCont::INS = this;
	eventDisp = NewObject<UEventDispatcher>();
}
void UTaskCont::Init()
{
	//UTask* t = 0;
	isGameStarted = true;

	//t = AddTask({ "func:(Print),msg:(Starting...)" });
	//t->AddRule(CreateRule(AddModel(FCoreValue("Timeout:(20)")), "Timeout=1", FCommand::Create({  "func:(TaskComp)"  })));

	/*t = AddTask({ "func:(Print),msg:(Started Task 1)",  "func:(Run),actor:(FirstButton),cmd:(OnActive)", "func:(SetAlarm),status:(0)" });
	t->AddRule(CreateRule(AddModel(FCoreValue("Timeout:(10)")), "Timeout=1", FCommand::Create({  "func:(SetAlarm),status:(1)",  "func:(Print),msg:(Alarm!)"  })));
	t->AddRule(CreateRule(AddModel(FCoreValue("Timeout:(20)")), "Timeout=1", FCommand::Create({ "func:(Print),msg:(Game Over. Go to restart -> )"  })));
	t->AddRule(CreateRule(AddModel(FCoreValue("PressButton:(FirstButton)")), "PressButton=1", FCommand::Create({ "func:(Run),actor:(FirstButton),cmd:(OnDeactive)", "func:(TaskComp)"  })));

	t = AddTask( { "func:(Print),msg:(Started Task 2)" , "func:(Run),actor:(SecondButton),cmd:(OnActive)", "func:(SetAlarm),status:(0)" });
	t->AddRule(CreateRule(AddModel(FCoreValue("Timeout:(10)")), "Timeout=1", FCommand::Create({  "func:(SetAlarm),status:(1)" ,  "func:(Print),msg:(Alarm!)" })));
	t->AddRule(CreateRule(AddModel(FCoreValue("Timeout:(20)")), "Timeout=1", FCommand::Create({ "func:(Print),msg:(Game Over. Go to restart -> )" })));
	t->AddRule(CreateRule(AddModel(FCoreValue("PressButton:(SecondButton)")), "PressButton=1", FCommand::Create({ "func:(Run),actor:(SecondButton),cmd:(OnDeactive)", "func:(SetAlarm),status:(0)" , "func:(TaskComp)" })));
	*/

	




}

void UTaskCont::CreateButtonTask(FString ButtonId, float Time, float BtnAndAlarmDelay , bool IsAlarmNotify)
{
	int8 indexTask = listTasks.Num();
	FString taskName = "Task" + FString::FromInt(indexTask);

	
	float timeBtn = (Time > BtnAndAlarmDelay) ? Time - BtnAndAlarmDelay : BtnAndAlarmDelay;
	float timeAlarm = timeBtn + BtnAndAlarmDelay;
	float timeout = Time + BtnAndAlarmDelay;

	FString timeBtnStr = FString::SanitizeFloat(timeBtn);
	FString timeAlarmStr = FString::SanitizeFloat(timeAlarm);
	FString timeoutStr = FString::SanitizeFloat(timeout);
	AActor* btn = UResource::INS->GetActor(ButtonId);
	if (!btn)
	{
		Debug::Fatal(ButtonId + " ADLI BIR ACTOR BULUNAMADI!");
		return;
	}
	UTask* t = 0;
	t = AddTask({ 
		"func:(Print),msg:(Started "+ taskName +")",
		 "func:(SetAlarm),status:(0)"
	});

	t->AddRule(
		CreateRule(AddModel(FCoreValue("Timeout:("+ timeBtnStr +")")), "Timeout=1",
			FCommand::Create({ 
				"func:(Run),actor:(" + ButtonId + "),cmd:(OnActive)",
				"func:(Print),msg:("+ taskName +" : "+ ButtonId +" OnActive!)"
	})));

	t->AddRule(
		CreateRule(AddModel(FCoreValue("Timeout:("+ timeAlarmStr +")")), "Timeout=1",
			FCommand::Create({ 
				"func:(SetAlarm),status:(1)", 
				"func:(Print),msg:("+ taskName +" : Alarm Started!)"
				
	})));

	t->AddRule(
		CreateRule(AddModel(FCoreValue("Timeout:(" + timeoutStr + ")")), "Timeout=1",
			FCommand::Create({
				"func:(TaskComp)"
				})));

	t->AddRule(CreateRule(AddModel(FCoreValue("PressButton:("+ ButtonId +")")), "PressButton=1",
		FCommand::Create({ 
			"func:(Run),actor:("+ ButtonId +"),cmd:(OnDeactive)",
			"func:(SetAlarm),status:(-1)",
			"func:(TaskSucc)"
	})));
}

void UTaskCont::SetTask(FString _id)
{
	Debug("task.settask " + _id);
	UTask* t = listTasks[_id];
	
	if (t)
	{
		currTask = t;
		onTaskStatus(EStatus::ON_STARTED);
		
	}

}
void UTaskCont::SetTask(int8 _index)
{
	
	for (auto& Elem : listTasks)
	{
		UTask* t = Elem.Value;
		if (t->Index == _index)
			SetTask(t->Id);
	}

}


void UTaskCont::onTaskStatus(EStatus _status)
{
	if (_status == EStatus::ON_STARTED)
	{
		startTimer();
		if (currTask->StartCommand.IsValid)
			RunCommands(currTask->StartCommand);
	}

	 if (_status == EStatus::ON_COMPLETED)
	{
		 stopTimer();
		currTask->IsCompleted = true;
		if (currTask->Index == listTasks.Num() - 1) //all task completed!
		{
			if (Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel) Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel->Print("debug", "Great! Level Completed!");
		}
		else {
			//if (currTask->isAutoNext)
			if(currTask->TaskStatus < 1)
			{
				if (Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel) Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel->Print("debug", "Game Over!!!");
				isGameStarted = false;
			
			}
			else
			{
				FString _nextTaskId = UTask::IDPREFIX + FString::FromInt(currTask->Index + 1);
				SetTask(_nextTaskId);
			}

		}
	}
	//AMainCont::INS->OnTask(currTask->Index, _status);


}





void UTaskCont::Send(FString _coreValueStr)
{
	if (!isGameStarted) return;
	Debug("--> Send : " + _coreValueStr);
	FCoreValue cValue = FCoreValue(_coreValueStr);
	if (currTask)
	{
		currTask->UpdateRequest(cValue);
		FResultProcess result = currTask->Check();
		if (result.status == 1)
		{
			if (result.rule->cmd.IsValid)
				RunCommands(result.rule->cmd);
		}
	}
}




void UTaskCont::GoToTask(FString _id)
{
	SetTask(_id);
}

void UTaskCont::RunCommands(FCommand _commands)
{
	if (!_commands.IsValid) return;
	
	for (FCoreValue ov : _commands.list)
	{
		if (ov.IsValid)
			runCommand(ov);

	}

}

void UTaskCont::runCommand(FCoreValue ov)
{
	FString func;
	ACoreActor* actor = 0;

	Debug("		--->>RunCommand(" + ov.ToString() + ") -> "  + currTask->ToString());


	func = ov.GetValue("func").GetString(0).ToLower();
	actor = Cast<ACoreActor>(UResource::INS->GetActor(ov.GetValue("actor").GetString(0)));
	
	

	if (func == "TaskComp")
	{
		onTaskStatus(EStatus::ON_COMPLETED);
	}

	if (func == "TaskFail")
	{
		currTask->TaskStatus = 0;
	}

	if (func == "TaskSucc")
	{
		currTask->TaskStatus = 1;
	}


	if (func == "GoToTask")
	{
		FString id =  ov.GetValue("task").GetString(0).ToLower();
		GoToTask(id);

	}

			
			
	if (func == "Run" && actor)
	{
		 ov.UpdateValue("func", ov.GetValue("cmd").GetString(0),false);
		 Debug("erdal ilgin : " + ov.ToString());
		 actor->Run(ov.ToString());
	}
	
	if (func == "Print")
	{
		FString msg = ov.GetValue("msg").GetString(0);
		if (Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel) Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel->Print("debug", msg);
	}
	if (func == "SetAlarm")
	{
		ACoreActor* alarmActor =Cast< ACoreActor>( UResource::INS->GetActor("Alarm"));
		//if (Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel) Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel->Print("debug", msg);
		if (alarmActor)
		{
			FString id = ov.GetValue("status").GetString(0).ToLower();
			if (id == "-1")
				alarmActor->Run("func:(Deactive)");
			if (id == "0")
				alarmActor->Run("func:(Active)");
			if (id == "1")
				alarmActor->Run("func:(Open)");
			if (id == "2")
				alarmActor->Run("func:(Close)");
		}
	}
	
}





void UTaskCont::startTimer()
{
	stopTimer();
	timeCurrTask = 0;
	//AGameCont::INS->GetWorldTimerManager().SetTimer(timer, timerDelegate, 1, true);
	AGameCont::INS->GetWorldTimerManager().SetTimer(timer, this, &UTaskCont::onTimerHandler, 1, true);
	Debug("task.starttimer");
}

void UTaskCont::stopTimer()
{
	if (timer.IsValid())
		AGameCont::INS->GetWorld()->GetTimerManager().ClearTimer(timer);
}

void UTaskCont::onTimerHandler()
{
	Debug("task.onTimerHandler");
	if (Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel) Cast<AGameTakeoff>(AGameCont::INS)->DebugPanel->Print("time", FString::SanitizeFloat(timeCurrTask));
	Send("TimeOut:(" + FString::FromInt(timeCurrTask) + ")");
	timeCurrTask++;

}







void UTaskCont::AddTask(UTask* _task)
{
	int _index = listTasks.Num();
	FString _id = UTask::IDPREFIX + FString::FromInt(_index);
	_task->Index = _index;
	_task->Id = _id;
	listTasks.Add(_id, _task);
	
}



UTask* UTaskCont::AddTask( vector<FString> _commands)
{
	UTask* t = UTask::Create(FCommand::Create(_commands));
	AddTask(t);
	return t;

}

UPModel* UTaskCont::AddModel(FCoreValue _key)
{
	UPModel* m = UPModel::Create(_key);
	m->Index = listModels.Num();
	m->Id = "model" + FString::FromInt(m->Index);
	listModels.Add(m->Id, m);
	return m;
}

URule* UTaskCont::CreateRule(UPModel* _model, FString _comparison, FCommand _command, FString _msg , bool _isSkippabble , bool _isPrint, int8 _printAfterCounter)
{

	URule* r = URule::Create(_model, _comparison, _command);
	r->IsSkippable = _isSkippabble;
	r->msg = _msg;
	r->isPrint = _isPrint;
	r->printAfterCounter = _printAfterCounter;
	r->Id = URule::IDPREFIX + FString::FromInt((int)listRules.Num());
	//ebug("		rule.created! -> id : "+ r->Id+ " model:" + _model->ToString() + " - comparison : " + _comparison);
	listRules.Add(r->Id, r);
	return r;
}








UTask* UTaskCont::GetTask(FString _id)
{
	UTask* t = 0;
	if(listTasks.Contains(_id))
		t = listTasks[_id];	
	return t;
}
UTask* UTaskCont::GetTask(int8 _index)
{
	FString _id = UTask::IDPREFIX + FString::FromInt(_index);
	return GetTask(_id);
}

FString UTaskCont::GetTaskLabel(FString _id)
{
	FString s = "";
	UTask* t = GetTask(_id);
	if (t)
		s = t->Name;
	return s;
}
URule* UTaskCont::GetRule(FString _id)
{
	URule* r = listRules[_id];
	return r;

}
UPModel* UTaskCont::GetModel(FString _id)
{
	UPModel* m = listModels[_id];
	return m;
}



















/*____________________ Rule ____________________ */
int8 URule::GetComparisionsResult()
{

	TArray<FString> _parseArrComparisons;
	int foundAnd = comparison.Find("&");
	int foundOr = comparison.Find("|");
	bool isAndOperator = false;
	if (foundAnd > -1)
	{
		isAndOperator = true;
		_parseArrComparisons = UCoreUtils::ParseString(comparison, "&");
	}
	else if (foundOr > -1)
	{
		isAndOperator = false;
		_parseArrComparisons = UCoreUtils::ParseString(comparison, "|");
	}
	else
	{
		isAndOperator = true;
		_parseArrComparisons.Add(comparison);
	}

	int8 _result = isAndOperator ? 1 : 0;

	bool comprasionResult = false;

	FCoreValue compareValue = model->GetKey().GetCompare(model->GetRequest());
	//Log(Id + " key / req / compareVal : " + model->GetKey().ToString() + "/" + model->GetRequest().ToString() + " / " +   compareValue.ToString());

	for (FString _comparison : _parseArrComparisons)
	{

		TArray<FString> _parseComparison = UCoreUtils::ParseString(_comparison, "=");
		FString idKey = _parseComparison[0];

		FString patternComprasion = _parseComparison[1]; //eðer patternValueId karþýlýðý bu ise..

		FString compare = compareValue.GetValue(idKey).GetString();

		comprasionResult = (compare == patternComprasion);

		if (compare.IsEmpty()) //bir key eslemesi yok demektir
		{
			_result = -1;
		}
		else
		{
			if (isAndOperator) //and
			{
				if (!comprasionResult)	_result = 0;
			}
			else { //or
				if (comprasionResult)	_result = 1;
			}
		}




	}
	status = (_result > 0);
	return _result;
}



/*____________________ PModel ____________________ */

UPModel* UPModel::Create(FCoreValue _key)
{

	//int _totalValue = (int)_keys.size();
	UPModel* model = NewObject<UPModel>();
	model->key = _key;


	return model;
}


bool UPModel::Save(FCoreValue _request)
{
	//bool saveResult;
	/* key'de olmayan bi _request key aldiysak kaydetmeyelim */
	//bool allReqToKeyMatch = false;
	FCoreValue _key = GetKey();
	//if (key.GetKeys().Num() > 0) allReqToKeyMatch = true;
	for (FString k : _key.GetKeys())
	{
		if (!_request.GetKeys().Contains(k)) return false;
	}

	request = _request;
	return true;
}

void UPModel::Reset()
{
	isSaved = false;
	request.Reset();
	request = FCoreValue("");

}

FString UPModel::ToString()
{
	FString s = Id + " (" + key.ToString() + ")";

	return s;
}



