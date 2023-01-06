// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "NPCDialogComponent.generated.h"

class ANPCharacter;
class AMainCharacter;

UCLASS(Blueprintable)
class UDialogWidget : public UUserWidget
{

	GENERATED_BODY()

public:
	UDialogWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {};

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDisplay(UDialogNode* Node, ANPCharacter* NPC, AMainCharacter* Player);
};

UCLASS(Abstract, BlueprintType)
class UDialogNode: public UObject
{
	GENERATED_BODY()

public:
	UDialogNode() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	FName NodeName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	FText NPCText;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TSubclassOf<UDialogWidget> GetWidgetClass() const { return nullptr; };
};

USTRUCT(BlueprintType)
struct FTextAnswer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Next;
};

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class UTextAndAnswers : public UDialogNode
{
	GENERATED_BODY()
public:
	UTextAndAnswers();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Answers")
	TArray<FTextAnswer> Answers;

	virtual TSubclassOf<UDialogWidget> GetWidgetClass() const override;

private:
	TSubclassOf<UDialogWidget> WidgetClass;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MERCHANTS_API UNPCDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPCDialogComponent();

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	TArray<UDialogNode*> Dialog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	FName StartNode;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Start();
	
private:

	UPROPERTY()
	TMap<FName, UDialogNode*> NodeByName;

	UPROPERTY()
	UDialogNode* CurrentNode;

public:
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UDialogNode* GetCurrentNode() const { return CurrentNode; }
};
