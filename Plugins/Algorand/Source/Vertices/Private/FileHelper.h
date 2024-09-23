#pragma once

class FileHelper
{
public:
	static bool CopyDirectory(const FString& SourceDir, const FString& DestDir, bool OverwriteExisting);
};
