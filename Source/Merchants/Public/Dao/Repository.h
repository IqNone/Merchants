// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "functional"
#include "Repository.generated.h"

template <typename T>
struct RepositoryResponse 
{
	bool bSuccess;
	FString ErrorMessage;
	TSharedPtr<T> Payload;

	RepositoryResponse(bool bSuccess, FString ErrorMessage, TSharedPtr<T> Payload) : bSuccess(bSuccess), ErrorMessage(ErrorMessage), Payload(Payload) {};
	
	static RepositoryResponse<T> Success(T* Payload) { return RepositoryResponse<T>(true, "", TSharedPtr<T>(Payload)); };
	static RepositoryResponse<T> Failure(FString ErrorMessage) { return RepositoryResponse<T>(false, ErrorMessage, TSharedPtr<T>()); };
};

UCLASS(Config=Game)
class MERCHANTS_API URepository : public UObject
{
	GENERATED_BODY()
	
public:

	URepository();

	UPROPERTY(Config)
	FString RepositoryURL;

#if defined WITH_EDITOR || defined UE_SERVER 	
	FString RepositoryUsername;
	FString RepositoryPassword;
#endif

	void Authenticate(FString Username, FString Password, const std::function <void(RepositoryResponse<FString>)>& Callback);
};
