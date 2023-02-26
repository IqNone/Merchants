// Fill out your copyright notice in the Description page of Project Settings.


#include "Dao/Repository.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"

URepository::URepository()
{
#if defined WITH_EDITOR || defined UE_SERVER 	
	RepositoryUsername = FPlatformMisc::GetEnvironmentVariable(TEXT("MERCHANTS_REPOSITORY_USERNAME"));
	RepositoryPassword = FPlatformMisc::GetEnvironmentVariable(TEXT("MERCHANTS_REPOSITORY_PASSWORD"));;
#endif
}

void URepository::Authenticate(FString Username, FString Password, const std::function <void(RepositoryResponse<FString>)>& Callback)
{
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(TEXT("username=") + Username + TEXT("&password=") + Password);
	Request->SetURL(RepositoryURL + TEXT("/public/users/authenticate"));

	Request->OnProcessRequestComplete().BindLambda([&, Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {
		if (bConnectedSuccessfully)
		{			
			if (Response->GetResponseCode() == 200)
			{
				Callback(RepositoryResponse<FString>::Success(new FString(Response->GetContentAsString())));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Request failed: %d"), Response->GetResponseCode());
				Callback(RepositoryResponse<FString>::Failure(FString("Invalid username or passowrd")));
			}			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to connect to the server"));
			Callback(RepositoryResponse<FString>::Failure(FString("Could not connect to the server")));
		}
	});

	Request->ProcessRequest();
}
