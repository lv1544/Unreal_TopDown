// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/T1CursorWidget.h"
#include "Components/Image.h"


void UT1CursorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //// �ʱ�ȭ �ڵ�
    if (CursorImage)
    {
        CursorImage->SetVisibility(ESlateVisibility::Visible);
    }


}

void UT1CursorWidget::SetCursorImage(UTexture2D* NewCursorImage)
{
   /* if (CursorImage && NewCursorImage)
    {
        FSlateBrush Brush;s
        Brush.SetResourceObject(NewCursorImage);
        CursorImage->SetBrush(Brush);
    }*/


}

