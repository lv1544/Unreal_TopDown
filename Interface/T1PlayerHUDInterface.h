// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "T1PlayerHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UT1PlayerHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class T1_API IT1PlayerHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(class UT1HUDWidget* InHUDWidget) = 0;
};
