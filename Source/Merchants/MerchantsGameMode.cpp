// Copyright Epic Games, Inc. All Rights Reserved.

#include "MerchantsGameMode.h"
#include "MainCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Utils/AESDecryptor.h"
#include "Kismet/GameplayStatics.h"

AMerchantsGameMode::AMerchantsGameMode()
{
	AuthTokenMaxSeconds = 30;

	FString AESHexKey = FPlatformMisc::GetEnvironmentVariable(TEXT("MERCHANTS_AES_KEY"));
	FString AESHexIv = FPlatformMisc::GetEnvironmentVariable(TEXT("MERCHANTS_AES_IV"));

	Decryptor = new AESDecryptor(AESHexKey, AESHexIv);
}

AMerchantsGameMode::~AMerchantsGameMode()
{
	if (Decryptor)
	{
		delete Decryptor;
	}	
}

void AMerchantsGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	FString Token = UGameplayStatics::ParseOption(Options, TEXT("Token"));

#if defined WITH_EDITOR
	EditorAuthenticate(UniqueId, Token, ErrorMessage);
#else
	Authenticate(UniqueId, Token, ErrorMessage);
#endif
}

APlayerController* AMerchantsGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	FLoginData* LoginData = PendingLogin.FindAndRemoveChecked(UniqueId);
	if (!LoginData)
	{
		ErrorMessage = TEXT("Invalid login");
		return nullptr;
	}

	// TODO setup the controller;
	delete LoginData;

	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);	
}

void AMerchantsGameMode::Authenticate(const FUniqueNetIdRepl& UniqueId, FString& Token, FString& ErrorMessage)
{
	if (Token.IsEmpty())
	{
		ErrorMessage = TEXT("No authentication token provided");
		return;
	}

	FString Json;
	try
	{
		Json = Decryptor->DescryptHex(Token);
	}
	catch (const std::exception& e)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid token: %s"), e.what());
		ErrorMessage = TEXT("Invalid authentication token");
		return;
	}

	TSharedPtr<FJsonObject> JsonObject = Parse(Json);
	if (IsExpired(JsonObject))
	{
		ErrorMessage = TEXT("Expired authentication token");
		return;
	}

	FLoginData* LoginData = CreateLoginData(JsonObject);
	PendingLogin.Emplace(UniqueId, LoginData);
}

TSharedPtr<FJsonObject> AMerchantsGameMode::Parse(FString& Json)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	return JsonObject;
}

bool AMerchantsGameMode::IsExpired(TSharedPtr<FJsonObject> JsonObject)
{
	int64 TokenCreationDate = GetCreationDate(JsonObject);
	int64 Now = FDateTime::UtcNow().ToUnixTimestamp();
	return Now - TokenCreationDate > AuthTokenMaxSeconds;
}

FLoginData* AMerchantsGameMode::CreateLoginData(TSharedPtr<FJsonObject> JsonObject)
{
	TSharedPtr<FJsonObject> UserObject = JsonObject->GetObjectField(TEXT("user"));
	FLoginData* LoginData = new FLoginData();
	LoginData->Username = UserObject->GetStringField("username");

	return LoginData;
}

int64 AMerchantsGameMode::GetCreationDate(TSharedPtr<FJsonObject> JsonObject)
{
	int64 TokenCreationDate;
	JsonObject->TryGetNumberField(TEXT("ct"), TokenCreationDate);
	return TokenCreationDate;
}

#if defined WITH_EDITOR
void AMerchantsGameMode::EditorAuthenticate(const FUniqueNetIdRepl& UniqueId, FString& Token, FString& ErrorMessage)
{
	if (Token.IsEmpty()) 
	{
		//we get here from "Play from here" and skip the login map
		//todo create a default user		
		FLoginData* LoginData = new FLoginData();
		LoginData->Username = "editor";
		PendingLogin.Emplace(UniqueId, LoginData);
	}
	else
	{
		//we have a token so we must come from the login map, go with the normal authentication
		Authenticate(UniqueId, Token, ErrorMessage);
	}
}
#endif