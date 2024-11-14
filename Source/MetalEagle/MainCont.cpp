// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCont.h"



UMainCont* UMainCont::INS = NULL;
UMainCont::UMainCont()
{
	UMainCont::INS = this;
}
