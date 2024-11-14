#pragma once
#include <vector>
#include "Object.h"
#include "Process.h"
#include "Utils/CoreUtils.h"
#include "Utils/CoreValue.h"
#include "Utils/CoreEvent.h"
#include "TimerManager.h"
#include "TaskCont.generated.h"

class UTaskCont;
class UResource;

using namespace std;




/*____________________ Model ____________________ */
UCLASS() class METALEAGLE_API UPModel : public UObject
{
	GENERATED_BODY()
public:
	//UPROPERTY() TArray<UPValue*> listValues;
	UPROPERTY()	FString Id;
	UPROPERTY() int Index = -1;
	UPROPERTY() bool isSaved = false;

	UPModel() {};
	static UPModel* Create(FCoreValue _key);
	bool Save(FCoreValue _request);
	FCoreValue GetKey() { return key; };
	FCoreValue GetRequest() { return request; };
	void Reset();
	virtual FString ToString();

private:
	FCoreValue key;
	FCoreValue request;

};

/*____________________ Rule ____________________ */
UCLASS() class METALEAGLE_API URule : public  UObject
{
	GENERATED_BODY()
public:
	static FString IDPREFIX;
	UPROPERTY()	int8 counter;
	UPROPERTY()	FString Id;
	UPROPERTY()	int Index;
	UPROPERTY() UPModel* model;
	UPROPERTY()	FCommand cmd;
	UPROPERTY()	FString comparison;
	UPROPERTY()	bool status;
	UPROPERTY()	FString msg;
	UPROPERTY()	bool isPrint;
	UPROPERTY()	 int8 printAfterCounter; /* rule eslesmesi belirli bir repeat sayisina geldiginde print et (isPrintCounter==counter) " */;
	/* @IsSkippable : Bir eslesme yoksa bu rule skip edilir ve diger rule'a bakýlýr */
	UPROPERTY()	bool IsSkippable;

	void Init(UPModel* _model, FString _comparison, FCommand _cmd)
	{
		counter = 0;
		printAfterCounter = 0;
		model = _model;
		comparison = _comparison;
		cmd = _cmd;
		msg = "";

	}
	static URule* Create(UPModel* _model, FString _comparison, FCommand _cmd)
	{
		URule* r = NewObject<URule>();
		r->Init(_model, _comparison, _cmd);
		return r;
	}

	FString ToString()
	{
		return "Id:(" + Id + "), comparison:(" + comparison + "),cmd:(" + cmd.ToString() + "),status:(" + FString::FromInt((int)status) + ")";
	}

	int8 GetComparisionsResult();

};


/* <---------- ResultTaskProcess -------------> */
USTRUCT()
struct METALEAGLE_API FResultProcess
{
	GENERATED_USTRUCT_BODY()
		/* (-1) listening devam ediyor,  (0) bu process'e kayitli bir rule yok,   (1) bir rule eslesmesi var, (2) tum rule'lar kontorol edildi. eslesme yok. */
		int8 status;
	URule* rule;
	FCommand command;
	FResultProcess()
	{
		rule = 0;
		command = FCommand();
	};

	FString ToString()
	{
		FString _rule = (rule == 0) ? "null" : rule->ToString();
		return "FResultProcess : status : " + FString::FromInt(status) + " / rule : " + _rule + " / command : " + command.ToString();
	}
};






/* TASK */
UCLASS()
class METALEAGLE_API UTask : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY() TArray<URule*> listRules;
	int totalRule;
public:
	static FString IDPREFIX;
	UPROPERTY() FString Name;
	UPROPERTY() bool isAutoNext;
	UPROPERTY() FString Id;
	UPROPERTY() int Index;
	UPROPERTY() int8 TaskStatus;
	UPROPERTY() bool IsCompleted;
	UPROPERTY() FCommand StartCommand;


	UTask()
	{ 
		//ebug(">task("+FString::FromInt(Index)+") created -> " + Name ); 
	};
	static UTask* Create(FString _name, bool _autoNext=true)
	{
		UTask* t = NewObject<UTask>();
		t->Name = _name;
		t->isAutoNext = _autoNext;
		t->TaskStatus = -1;
		return t;
	}

	static UTask* Create(FCommand _startCommd)
	{
		UTask* t = Create("", true);
		if (_startCommd.IsValid) t->StartCommand = _startCommd;
		return t;
	}



	void Reset();
	/* TPair<status, rule>  @status :  */
	FResultProcess Check()
	{
		FResultProcess  result;
		result.status = -1;
		if (listRules.Num() == 0) result.status = 0;
		for (URule* currRule : listRules)
		{
			if (currRule->model->isSaved)
			{
				int8 comprasionResult = currRule->GetComparisionsResult();
				//ebug("				"+Id+".Check() -> @rule :	" + currRule->ToString() + "@model:(" + currRule->model->ToString() + ")" + ".GetComparisionsResult() : ", comprasionResult);
				if (comprasionResult == -1) //key eslemesi yok (gelen request icinde beklenen key bulunamadi)
				{
					result.status = -1;
					//return result;
				}
				else if (comprasionResult == 0) //key eslemesi var, value eslemesi yok
				{
					if (currRule->IsSkippable)
					{
						if (currRule->Index == totalRule - 1)
						{
							result.status = 2;
							return result;
						}
					}
				}

				else if (comprasionResult == 1) //key eslesmesi var, values eslesmesi var
				{
					currRule->counter++;
					result.status = 1;
					result.rule = currRule;
					return result;
				}


			}


		}

		return result;



	}



	void AddRule(URule* _rule)
	{
		_rule->Index = listRules.Num();
		listRules.Add(_rule);
		totalRule = listRules.Num();

	}


	void UpdateRequest(FCoreValue _request)
	{
		for (URule* rule : listRules)
		{
			bool saveResult = rule->model->Save(_request);

			rule->model->isSaved = true;
			//Debug("save.rule :" + rule->ToString() + " --------  _request : " + _request.ToString() + "-------------- saveResult "  , saveResult);
			//ebug("save.key :" + rule->model->ToString() + " --------  _request : " + _request.ToString() + "-------------- saveResult "  , saveResult);
		}

	}

	FString ToString()
	{
		FString s = Id;
		for (URule* r : listRules)
		{
			FString strModel = ",@rule.model:(" + r->model->ToString() + ")";
			s += strModel;
		}

		s += ",@strtcmd:(" + StartCommand.ToString() + ")";
		return s;
	}


};


/* #TASKCONTROLLER */
UCLASS(BlueprintType, Blueprintable)
class METALEAGLE_API UTaskCont : public UObject
{
	GENERATED_BODY()
public:
	UTaskCont();
	static UTaskCont* INS;
	UPROPERTY() UTask* currTask;
	UPROPERTY() UEventDispatcher* eventDisp;
	void Init();
	
	UFUNCTION(BlueprintCallable, Category = MetalEagle) void CreateButtonTask(FString ButtonId, float EventTime, float ButtonAndAlarmDelay=10, bool IsAlarmNotify=true);
	void AddTask(UTask* _task);
	UTask* AddTask(vector<FString> _commands);
	UTask* GetTask(FString _id);
	UTask* GetTask(int8 _index);
	FString GetTaskLabel(FString _id);
	URule* GetRule(FString _id);
	UPModel* GetModel(FString _id);
	UPModel* AddModel(FCoreValue _cmd);
	URule* CreateRule(UPModel* _model, FString _comparison, FCommand _command, FString _msg = "", bool _isSkippabble = true, bool _isPrint = true, int8 _printAfterCounter = 0);
	void SetTask(FString _index);
	void SetTask(int8 _index);
	UFUNCTION(BlueprintCallable, Category = MetalEagle) void Send(FString _coreValueStr);
	void GoToTask(FString _id);
	void onTaskStatus(EStatus _status);
	void SetProces(int _process);
	void onProcessStatus(EStatus _status);
	void GoBackTask(FString _idTask);
	void RunCommands(FCommand _commands);
private:
	bool isGameStarted;
	UPROPERTY() TMap<FString, UTask*> listTasks;    
	UPROPERTY() TMap<FString, UPModel*> listModels;
	UPROPERTY() TMap<FString, URule*> listRules;

	static void onPopupClosed(UEvent &e);
	static void onEventProcess(UEvent &e);
	FResultProcess getProcessResult(FResultProcess _result);
	
	void onTaskResult(bool _status, FString _msg = "");

	/* timer */
	float timeCurrTask;
	FTimerDelegate timerDelegate;
	FTimerHandle timer;
	UFUNCTION()	void onTimerHandler();

	void runCommand(FCoreValue _cmd);
	void startTimer();
	void stopTimer();

};



