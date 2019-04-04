#pragma once
#include <filesystem>

/*
	@param <path>: a path with or without .sav appended.
	@return: a path with .sav appended once.
*/
std::filesystem::path getSaveFile(std::filesystem::path path)
{
	auto ext = path.extension();
	ext.replace_extension(".sav");
	return path.replace_extension(ext);
}

/*
	@param path: A path without .sav appended.
	@return: A path with .sav appended if there exists one, otherwise without .sav appended.
*/
std::filesystem::path getLoadFile(std::filesystem::path path)
{
	auto savFile = getSaveFile(path);

	std::error_code ec;
	if (std::filesystem::exists(savFile, ec))
		return savFile;
	
	return path;
}