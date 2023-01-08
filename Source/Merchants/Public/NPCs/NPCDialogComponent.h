// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "NPCDialogComponent.generated.h"

/**
* Dialog example:
*             ___________________________________________________________________ 
*             |                | Hello, How can i help you? |                   |
*             |                     |       |        |                          |
* Node1:      |                |            |              |                    |
*             |           |                 |                    |              |
*             |  | Buy Something |  | Sell Something |  | Nothing, Goodbye |    |
*       	  |____________|_________________|__________________________________|
*                          |                 |                    
*                   ______________           |
* Node2:            |   What...? |           |
*                   --------------           |
*                                     ______________
* Node3:                              |  What...?  |
*                                     --------------
* 
* 
* Node1, Node2, Node3: UDialogNode
* 
* UDialogNode:         the base class for the nodes in the dialog graph. Custom implementations of this class 
*	will represent different UI sections on the dialog window. Thus, one main functionality of the class is 
*	to provide the UI widget class (subclass of UDialogWidget) responsible for displaying this node.
* 
* UDialogNode 
*	- NodeName:				the name that identifyes the node
*	- NPCText:				every node contains a text that will be displayed to the user
*   - WidgetClass:          the UI part that will display this node, it will handle the user interaction and 
*    							on OnDialogSectionCompleted will provide the next node name (None = end dialog)
* Example:
*	class UTextAndAnswers: public UDialogNode
* 
* UDialogWidget:       the base UI class. Usually this will be implemented in the editor. It is responsible for triggering the
*	OnDialogSectionCompleted event, providing the next node to be displayed, or None for closing the dialog.
* 
* UDialogWidget
*   - OnDisplay:                   widget has to handle this event in order to properly construct itself
*   - OnDialogSectionCompleted:    the event this widget has to trigger with the name of the next node, or None to close the dialog
* 
* UNPCDialogComponent: defines the graph (the nodes) and the start node. It will be used by the UI (DialogHUD).
* 
* 
* How to use it:
*	In the NPC blueprint editor, select the UNPCDialogComponent component and provide the StartNode and the list of nodes.
*/


class ANPCharacter;
class AMainCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogSectionCompleted, FName, NextNode);

UCLASS(Blueprintable)
class UDialogWidget : public UUserWidget
{

	GENERATED_BODY()

public:
	UDialogWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {};

	UPROPERTY(BlueprintReadOnly, BlueprintCallable, BlueprintAssignable)
	FOnDialogSectionCompleted OnDialogSectionCompleted;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	TSubclassOf<UDialogWidget> WidgetClass;
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

	UFUNCTION(BlueprintCallable)
	void Next(FName NextNodeName);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnNodeChanged();

private:

	UPROPERTY()
	TMap<FName, UDialogNode*> NodeByName;

	UPROPERTY()
	UDialogNode* CurrentNode;

public:
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UDialogNode* GetCurrentNode() const { return CurrentNode; }
};
