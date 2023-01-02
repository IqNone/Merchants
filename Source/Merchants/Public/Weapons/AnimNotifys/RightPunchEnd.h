// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RightPunchEnd.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;

/**
 * 
 */
UCLASS()
class MERCHANTS_API URightPunchEnd : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
