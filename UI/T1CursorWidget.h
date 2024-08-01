// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T1CursorWidget.generated.h"

/**
 * 
 */
UCLASS()
class T1_API UT1CursorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Cursor")
    void SetCursorImage(UTexture2D* NewCursorImage);

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> CursorImage;


};
