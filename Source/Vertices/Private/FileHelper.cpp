#include "FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FileHelper::CopyDirectory(const FString& SourceDir, const FString& DestDir, bool OverwriteExisting)
{
	IFileManager& FileManager = IFileManager::Get();

	// Create the destination directory if it doesn't exist
	if (!FileManager.DirectoryExists(*DestDir))
	{
		if (!FileManager.MakeDirectory(*DestDir, true))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create directory: %s"), *DestDir);
			return false;
		}
	}

	// Find files and directories recursively in the source directory
	TArray<FString> FoundFiles;
	FileManager.FindFilesRecursive(FoundFiles, *SourceDir, TEXT("*"), true, false);

	for (const FString& FoundFile : FoundFiles)
	{
		const FString SourceFile = FoundFile;
		const FString DestFile = FPaths::Combine(DestDir, FPaths::GetCleanFilename(SourceFile));

		// Copy the file
		if (!FileManager.FileExists(*DestFile) && FileManager.Copy(*DestFile, *SourceFile, OverwriteExisting))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to copy file: %s to %s"), *SourceFile, *DestFile);
			return false;
		}
	}

	return true;
}