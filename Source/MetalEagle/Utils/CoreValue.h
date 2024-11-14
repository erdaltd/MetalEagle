#pragma once

#include "Resource.h"
#include "CoreValue.generated.h"

struct FCoreValue;
class UCoreUtils;
struct FCoreChar;
using namespace std;

/* <---------- ValueType -------------> */
UENUM(BlueprintType)
enum class ECoreValueType : uint8
{
	STRING,
	INT,
	FLOAT,
	RANGE,
	PAIR
};




USTRUCT(BlueprintType)
struct METALEAGLE_API FCoreChar
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	ECoreValueType Type;
	FCoreChar() { IsValid = false; };
	FCoreChar(FString _str);
	

	void Reset()
	{
		ch.Empty();
		charRange.Empty();
		IsValid = false;
	}


	FString ToString()
	{
		return  ch;
	}



	FString GetString()
	{ 
		return ch;
	}
	int8 GetInt();
	bool GetBool();
	float GetFloat();
	TArray<float> GetRange()
	{
		return charRange;
	}
	float GetRange(int8 _index)
	{
		return charRange[_index];
	}
	float GetRangeAvarage()
	{
		return GetRange(0) + ((GetRange(1) - GetRange(0) ) / 2);
	}
	bool IsNumeric()
	{
		return (Type == ECoreValueType::FLOAT || Type == ECoreValueType::INT);
	}



	bool GetCompare(FCoreChar _other)
	{
		bool _result = false;
		FCoreChar _this = *this;
		if (Type == _other.Type)
		{
			_result = (ch == _other.ch);
		}
		else
		{
			if (_this.IsNumeric() && _other.IsNumeric())
			{
				float c1 = GetFloat();
				float c2 = _other.GetFloat();
				_result = (c1 == c2);
			}
			else if (Type == ECoreValueType::RANGE && _other.IsNumeric())
			{
				_result = getRangeComparison(_other.GetFloat(), GetRange());
			}
			else if (IsNumeric() && _other.Type == ECoreValueType::RANGE)
			{
				_result = getRangeComparison(GetFloat(), _other.GetRange());
			}
		
		}
		return _result;
	}

	bool getRangeComparison(float _char, TArray<float> _range)
	{
		float _min = _range[0];
		float _max = _range[1];
		return (_char >= _min && _char <= _max);
	}


private:
	FString ch;
	TArray<float> charRange;

	ECoreValueType getType();

	



};





USTRUCT(BlueprintType)
struct METALEAGLE_API FCoreCharList
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	int8 CountOfChar;
	ECoreValueType Type;
	FCoreCharList() { CountOfChar = 0; IsValid = false; };
	FCoreCharList(FString _data);
	void Update(FString _data);


	void Reset();
	FCoreChar GetChar(int8 _index);
	FString ToString();

	TArray<FString> GetStringList();
	FString GetString();
	FString GetString(int8 _index);
	TArray<int8> GetInt();
	int8 GetInt(int8 _index=0);
	bool GetBool(int8 _index = 0);
	TArray<float> GetFloat();
	float GetFloat(int8 _index, bool _getAvarage=true);
	FVector GetFVector(bool _getAvarage = true);

	bool GetCompare(FCoreCharList _other);


private:

	TArray<FCoreChar> listChar;

	void setType();



};





USTRUCT(BlueprintType)
struct METALEAGLE_API FCoreValue 
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	int8 CountOfPair;
	FCoreValue() {};
	FCoreValue(FString _data);
	void Reset();
	static FCoreValue CreateFromVector(vector<FString> _value);
	FString ToString();
	
	void UpdateValue(FString _key, FString _data, bool _createPairIfNoExist);
	void Add(FCoreValue _other, bool _createPairIfNoExist);
	FCoreCharList GetValue(FString _key);
	FCoreCharList GetValue();
	TArray<FCoreCharList> GetValues();
	TArray<FString> GetKeys();

	FCoreValue GetCompare(FCoreValue _other);


private:
	
	TMap<FString, FCoreCharList> listPairs;



};











USTRUCT(BlueprintType)
struct METALEAGLE_API FCommand
{
	GENERATED_USTRUCT_BODY()

	TArray<FCoreValue> list;
	bool IsValid;


	FCommand()
	{
		IsValid = false;

	};
	FCommand(FString _str)
	{
		Add(_str);

	};

	FCoreValue Add(FString _str)
	{
		FCoreValue ov = FCoreValue(_str);
		if (ov.IsValid)
		{
			list.Add(ov);
			IsValid = true;
		}
		return ov;
	};

	static FCommand Create(vector<FString> _commands)
	{
		FCommand c = FCommand();
		if (_commands.size() > 0) c.IsValid = true;
		for (FString str : _commands)
		{
			if (!c.Add(str).IsValid)
			{
				c.IsValid = false;
			}
		}
		return c;
	};





	FCoreValue GetCoreValue(int _index);
	/* get params */
	FString GetFunc(int _index = 0);

	FString ToString()
	{
		FString s = "";
		for (FCoreValue c : list)
		{
			if (c.IsValid)
			{
				s.Append(c.ToString());
				s.Append(",");

			}

		}
		return s;
	}
};