// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/AESDecryptor.h"
#include "Misc/AES.h"

AESDecryptor::AESDecryptor(const FString& HexKey, const FString& HexIv)
{
	Key = new uint8[AES_KEY_SIZE];
	HexToBytes(HexKey, Key);

	Iv = new uint8[AES_BLOCK_SIZE];
	HexToBytes(HexIv, Iv);
}

AESDecryptor::~AESDecryptor()
{
	delete Key;
	delete Iv;
}

FString AESDecryptor::DescryptHex(const FString& Hex) const
{
	uint32 DataLen = Hex.Len() / 2;
	uint8* Data = new uint8[DataLen];
	HexToBytes(Hex, Data);

	uint32 Remaining = DataLen;

	while (Remaining > 0)
	{
		uint32 Pos = (Remaining - AES_BLOCK_SIZE);
		FAES::DecryptData(Data + Pos, AES_BLOCK_SIZE, Key, AES_KEY_SIZE);
		Xor(Data + Pos, Pos == 0 ? Iv : (Data + Pos - AES_BLOCK_SIZE), AES_BLOCK_SIZE);

		Remaining -= AES_BLOCK_SIZE;
	}

	uint32 Padding = Data[DataLen - 1];
	FString String = BytesToString(Data, DataLen - (Padding <= AES_BLOCK_SIZE ? Padding : 0));

	delete Data;

	return String;
}

FString AESDecryptor::BytesToString(const uint8* Bytes, int32 Count) const
{
	FString Result;
	Result.Empty(Count);

	while (Count)
	{
		int16 Value = *Bytes;

		Result += TCHAR(Value);

		++Bytes;
		Count--;
	}
	return Result;
}

void AESDecryptor::Xor(uint8* Array1, const uint8* Array2, const uint32 Count) const
{
	for (uint32 Idx = 0; Idx < Count; ++Idx)
	{
		Array1[Idx] ^= Array2[Idx];
	}
}