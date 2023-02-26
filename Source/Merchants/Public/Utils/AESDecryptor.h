// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

/**
 * 
 */
class MERCHANTS_API AESDecryptor
{
public:
	AESDecryptor(const FString& HexKey, const FString& HexIv);
	~AESDecryptor();

	FString DescryptHex(const FString& Hex) const;

private:
	
	uint8* Key;
	uint8* Iv;

	FString BytesToString(const uint8* bytes, int32 Count) const;
	void Xor(uint8* Array1, const uint8* Array2, const uint32 Count) const;
};
