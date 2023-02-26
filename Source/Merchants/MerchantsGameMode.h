// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MerchantsGameMode.generated.h"

class AESDecryptor;

struct FLoginData {
	FString Username;
};

UCLASS(minimalapi, Config = Game)
class AMerchantsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMerchantsGameMode();
	~AMerchantsGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "Login")
	int64 AuthTokenMaxSeconds;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

private:

	AESDecryptor* Decryptor;

	TMap<FUniqueNetIdRepl, FLoginData*> PendingLogin;

	void Authenticate(const FUniqueNetIdRepl& UniqueId, FString& Token, FString& ErrorMessage);

	TSharedPtr<FJsonObject> Parse(FString& Json);
	int64 GetCreationDate(TSharedPtr<FJsonObject> JsonObject);
	bool IsExpired(TSharedPtr<FJsonObject> JsonObject);
	FLoginData* CreateLoginData(TSharedPtr<FJsonObject> JsonObject);

#if defined WITH_EDITOR
	void EditorAuthenticate(const FUniqueNetIdRepl& UniqueId, FString& Token, FString& ErrorMessage);
#endif
};
