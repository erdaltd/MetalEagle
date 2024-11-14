// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreValue.h"
#include "MetalEagle.h"
#include "CoreUtils.h"


/* ______________ OM VALUE ______________ *
/* 
/* unit types :
/*	- int : ('2')
/*	- range : ('2/1')
/*	- range : ('2.3/0.12')
/*	- float : ('2.3')
/*	- string : ('test')
/*	- string : ('2/a')
/*	- string : ('b.1')
*/
/* ______________________________________________ OmChar ______________________________________________ */
FCoreChar::FCoreChar(FString _str)
{
	IsValid = true;
	ch = _str;
	Type = getType();
	if (Type == ECoreValueType::RANGE)
	{
		TArray<FString> listStrRange = UCoreUtils::ParseString(_str, "/");
		charRange.Init(0, 2);
		charRange[0] = UCoreUtils::ToFloat(listStrRange[0]);
		charRange[1] = UCoreUtils::ToFloat(listStrRange[1]);
	}
}


int8 FCoreChar::GetInt()
{
	return UCoreUtils::ToInt(ch);
}

bool FCoreChar::GetBool()
{
	return (UCoreUtils::ToInt(ch) != 0);
}

float FCoreChar::GetFloat()
{
	return UCoreUtils::ToFloat(ch);
}

ECoreValueType FCoreChar::getType()
{
	ECoreValueType _type = ECoreValueType::STRING;
	if (ch.Len() > 0)
	{
		if (ch.Contains("/"))
		{
			TArray<FString> _list = UCoreUtils::ParseString(ch, "/");
			if (_list.Num() > 1)
			{
				if (_list[0].IsNumeric() && _list[1].IsNumeric())
				{
					_type = ECoreValueType::RANGE;
				}
			}


		}
		else if (ch.IsNumeric())
		{

			if (ch.Contains("."))
			{
				_type = ECoreValueType::FLOAT;
			}
			else {
				_type = ECoreValueType::INT;
			}
		}

	}
	return _type;
}

/* ______________________________________________ OmCharList ______________________________________________ */
FCoreCharList::FCoreCharList(FString _data)
{
	CountOfChar = 0;
	Update(_data);
	
	


}

void FCoreCharList::Update(FString _data)
{
	if (_data.Len() > 0)
	{
		TArray<FString> listStrChar = UCoreUtils::ParseString(_data, ",");
		listChar.Empty(0);
		//listChar.Reserve(listStrChar.Num());
		IsValid = true;
		for (FString s : listStrChar)
		{
			FCoreChar c = FCoreChar(s);
			listChar.Add(c);
		}
		CountOfChar = listChar.Num();
		setType();
	}
}

void FCoreCharList::Reset()
{

	for (FCoreChar c : listChar)
	{
		c.Reset();
	}
}


FCoreChar FCoreCharList::GetChar(int8 _index)
{
	FCoreChar c;
	if (_index < CountOfChar)
		c = listChar[_index];
	return c;
}


void FCoreCharList::setType()
{
	if (!listChar.IsValidIndex(0))
	{
		Type = ECoreValueType::STRING;
	}
	else
	{
		int countStr = 0;
		FCoreChar lastChar = listChar[0];
		ECoreValueType lastType = lastChar.Type;
		for (FCoreChar c : listChar)
		{
			ECoreValueType cType = c.Type;
			if (cType != lastType)
			{
				if (lastChar.IsNumeric() && c.IsNumeric())
				{
					cType = ECoreValueType::FLOAT;
				}
				else {
					lastType = ECoreValueType::STRING;
					break;
				}

			}
			lastType = cType;
			

		}

		Type = lastType;





		
	}

	
}

bool FCoreCharList::GetCompare(FCoreCharList _other)
{
	bool _result = false;
	for (int8 i = 0; i < listChar.Num(); i++)
	{
		FCoreChar c = GetChar(i);
		FCoreChar cOther = _other.GetChar(i);
		bool boolCompare = false;
		if (cOther.IsValid)
		{
			boolCompare = c.GetCompare(cOther);
			_result = boolCompare;
		}
		if (!boolCompare)
		{
			return false;
		}
		
	}

	return _result;
}




FString FCoreCharList::GetString()
{
	FString s = "";

	for (int8 i = 0; i < CountOfChar; i++)
	{
		FCoreChar c = GetChar(i);
		s.Append(c.GetString());
		if (i < CountOfChar - 1) s.Append(",");
	}

	return s;
}
FString FCoreCharList::GetString(int8 _index)
{
	FString v;
	if (_index < CountOfChar)
		v = listChar[_index].GetString();
	return v;
}

TArray<FString> FCoreCharList::GetStringList()
{
	TArray<FString> arr;
	for (FCoreChar c : listChar)
	{
		arr.Add(c.GetString());
	}
	return arr;
}

TArray<int8> FCoreCharList::GetInt()
{
	TArray<int8> arr;
	for (FCoreChar c : listChar)
	{
		arr.Add(c.GetInt());
	}
	return arr;
}
int8 FCoreCharList::GetInt(int8 _index)
{
	int8 v = 0;
	if (_index < CountOfChar)
		v = listChar[_index].GetInt();
	return v;
}


bool FCoreCharList::GetBool(int8 _index /*= 0*/)
{
	bool v = false;
	if (_index < CountOfChar)
		v = listChar[_index].GetBool();
	return v;
}

TArray<float> FCoreCharList::GetFloat()
{
	TArray<float> arr;
	for (FCoreChar c : listChar)
	{
		arr.Add(c.GetFloat());
	}
	return arr;
}
float FCoreCharList::GetFloat(int8 _index, bool _getAvarage)
{
	float v = 0.0;
	if (_index < CountOfChar)
	{
		v = listChar[_index].GetFloat();
		if (_getAvarage)
		{
			if (GetChar(0).Type == ECoreValueType::RANGE)
				v = GetChar(0).GetRangeAvarage();
		}
	}
		
	return v;
}

FVector FCoreCharList::GetFVector(bool _getAvarage)
{
	FVector v = FVector::ZeroVector;
	if (CountOfChar >=  3)
	{
		v.X = GetFloat(0);
		v.Y = GetFloat(1);
		v.Z = GetFloat(2);
		if (_getAvarage)
		{
			if (GetChar(0).Type == ECoreValueType::RANGE)
				v.X = GetChar(0).GetRangeAvarage();
			if (GetChar(1).Type == ECoreValueType::RANGE)
					v.Y = GetChar(1).GetRangeAvarage();
			if (GetChar(2).Type == ECoreValueType::RANGE)
				v.Z = GetChar(2).GetRangeAvarage();
		}
	}
	return v;
}

FString FCoreCharList::ToString()
{
	FString _str;
	FString _type = "null";
	FString _values = "";
	
	_type = FString::FromInt((int)Type);

	for (int8 i = 0; i < CountOfChar; i++)
	{
		FCoreChar c = GetChar(i);
		_values.Append( c.ToString());
		if (i < CountOfChar - 1) _values.Append(",");
	}
	




	_str.Append("("+_values + ")");
	return _str;

}





/* _____________________________________________________________________________________________________ */
/* ______________________________________________ CoreValue ______________________________________________ */
/* _____________________________________________________________________________________________________ */


FCoreValue::FCoreValue(FString _data)
{
	TMap<FString, FString> listStrPairs;

	if (_data.EndsWith(",")) _data.RemoveAt(_data.Len()-1, 1); //delete ","
	if( _data.Contains("(") && _data.Contains(")") )
	{
		TArray<FString> listFirst = UCoreUtils::ParseString(_data, ")");
		for (int8 i = 0; i < listFirst.Num(); i++)
		{
			FString k;
			FString v;
			FString s = listFirst[i];
			if (s.StartsWith(",")) s.RemoveAt(0, 1); //delete ","
			TArray<FString> listStr;
			if (_data.Contains(":"))
			{
				listStr = UCoreUtils::ParseString(s, ":(", true);
			}
			else
			{
				listStr = UCoreUtils::ParseString(s, "(", true);
			}

			//if(listStr.IsValidIndex(0))
			if (listStr.Num() > 1)
			{
				k = listStr[0];
				v = listStr[1];
			}
			else
			{
				k = FString::FromInt(i);
				v = listStr[0];
			}

			listStrPairs.Add(k, v);

		}
	}
	else
	{
		listStrPairs.Add("0", _data);
	}



	for (TPair<FString, FString> pairStr : listStrPairs)
	{
		FCoreCharList v = FCoreCharList(pairStr.Value);
		listPairs.Add(pairStr.Key, v);
	}

	


		IsValid = true;

		CountOfPair = listPairs.Num();
	//}
	

}




void FCoreValue::Reset()
{
	for (FCoreCharList cl : GetValues())
	{
		cl.Reset();
	}
}
FCoreValue FCoreValue::CreateFromVector(vector<FString> _vec)
{
	FString str;
	int8 tot = (int8)_vec.size();
	for (int8 i = 0; i < tot; i++)
	{
		FString s = _vec[i];
		str.Append(s);
		if (i < tot - 1) str.Append(",");
	}
	return FCoreValue(str);
}

void FCoreValue::UpdateValue(FString _key, FString _data, bool _createPairIfNoExist)
{
	FCoreCharList v = GetValue(_key);

	if (v.IsValid)
	{
		v.Update(_data);
		listPairs[_key] = v;
	}
	else
	{
		if (_createPairIfNoExist)
		{
			FCoreCharList vNew = FCoreCharList(_data);
			listPairs.Add(_key, vNew);
		}
		
	}
}


void FCoreValue::Add(FCoreValue _other, bool _createPairIfNoExist)
{
	int8 t = _other.CountOfPair;
	for (int8 i = 0; i < t; i++)
	{
		UpdateValue(_other.GetKeys()[i], _other.GetValues()[i].GetString(), _createPairIfNoExist);
	}
}

FCoreCharList FCoreValue::GetValue(FString _key)
{
	FCoreCharList v;
	if(listPairs.Contains(_key))
		 v = listPairs[_key];
	return v;
}
FCoreCharList FCoreValue::GetValue()
{
	FCoreCharList v = GetValue("0");
	return v;
}

TArray<FString> FCoreValue::GetKeys()
{
	TArray<FString> arr;
	for (TPair<FString, FCoreCharList> p : listPairs)
	{
		arr.Add(p.Key);
	}
	return arr;
}



TArray<FCoreCharList> FCoreValue::GetValues()
{
	TArray<FCoreCharList> arr;
	for (TPair<FString, FCoreCharList> p: listPairs)
	{
		arr.Add(p.Value);
	}
	return arr;
}




FCoreValue FCoreValue::GetCompare(FCoreValue _other)
{
	FString s = "";
	int _total = listPairs.Num();
	int8 i = 0;
	for (TPair<FString, FCoreCharList> p : listPairs)
	{
		FString k = p.Key;
		FCoreCharList charList = p.Value;
		FCoreCharList otherCharList = _other.GetValue(k);
		if (otherCharList.IsValid)
		{
			bool boolCompare = charList.GetCompare(otherCharList);
			FString strCompare = FString::FromInt((int)boolCompare);
			s.Append(p.Key + ":(" + strCompare + ")");
			if (i < _total - 1) s.Append(",");
		}
		i++;
	}

	return FCoreValue(s);
}



FString FCoreValue::ToString()
{
	FString _str;
	FString _values = "";
	
	int _total = listPairs.Num();
	int8 i = 0;
	for(TPair<FString, FCoreCharList> p : listPairs)
	{
		FString _key = p.Key;
		FCoreCharList _value = p.Value;
		_str.Append(p.Key + ":" + _value.ToString());
		if (i < _total - 1) _str.Append(",");
		i++;

	}

	return _str;
	

}



FCoreValue FCommand::GetCoreValue(int _index)
{
	FCoreValue c;
	if (_index < list.Num())
		c = list[_index];
	return c;
}

FString FCommand::GetFunc(int _index /*= 0*/)
{
	FString f;
	if (_index < list.Num())
		f = list[_index].GetValue("func").GetString(0);
	return f;
}

/* _____________________________________________________________________________________________________ */
/* ______________________________________________ FCommand ______________________________________________ */
/* _____________________________________________________________________________________________________ */


//AItem* FCommand::GetItem(int _index /*= 0*/, FString _paramName /*= "item"*/)
//{
//	AItem* i = 0;
//	if (_index < list.Num())
//		i = Cast<AItem>(UResource::INS->GetItem(list[_index].GetValue(_paramName).GetString(0)));
//	return i;
//}
//
//UOmComp* FCommand::GetComp(int _index /*= 0*/, FString _paramName /*= "comp"*/)
//{
//	UOmComp* i = 0;
//	if (_index < list.Num())
//		i = UResource::INS->GetOmComp(list[_index].GetValue(_paramName).GetString(0));
//	return i;
//}
