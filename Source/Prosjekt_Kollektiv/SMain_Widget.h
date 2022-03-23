// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class PROSJEKT_KOLLEKTIV_API SMain_Widget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMain_Widget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
