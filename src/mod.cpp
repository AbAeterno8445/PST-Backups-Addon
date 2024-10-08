#include <lua.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "LuaCore.h"
#include "IsaacRepentance.h"
#include "HookSystem.h"

std::string baseDir = "data/passive skill trees/";
int backupCycle = 1;

// PST_BackupSave(int targetFileNum, int maxBackups)
// Save a backup for the given savefile, up to maxBackups, after which it will start cycling & replacing between the first and last backups
LUA_FUNCTION(Lua_PST_Backup_Save) {
	int targetFileNum = luaL_checkinteger(L, 1);
	int maxBackups = luaL_checkinteger(L, 2);
	if (maxBackups <= 0) maxBackups = 3;

	std::string savefilePath = baseDir + "save" + std::to_string(targetFileNum) + ".dat";

	// Check that savefile exists
	struct stat tmpBuffer;
	if (stat(savefilePath.c_str(), &tmpBuffer) == 0) {
		// Check for existing backups
		int backupNum = 0;
		for (int i = 1; i <= maxBackups; i++) {
			std::string tmpBackupPath = baseDir + "save" + std::to_string(targetFileNum) + "_backup" + std::to_string(i) + ".dat";
			if (stat(tmpBackupPath.c_str(), &tmpBuffer) != 0) {
				backupNum = i;
				break;
			}
		}
		if (backupNum == 0) {
			backupNum = backupCycle;
			backupCycle++;
			if (backupCycle > maxBackups) backupCycle = 1;
		}

		std::string tmpBackupPath = baseDir + "save" + std::to_string(targetFileNum) + "_backup" + std::to_string(backupNum) + ".dat";
		std::ifstream src(savefilePath, std::ios::binary);
		std::ofstream dst(tmpBackupPath, std::ios::binary);

		dst << src.rdbuf();
		return 1;
	}

	return 0;
}

// PST_BackupExists(int targetFileNum, int targetBackupNum)
// Check if the given backup exists
LUA_FUNCTION(Lua_PST_Backup_Exists) {
	int targetFileNum = luaL_checkinteger(L, 1);
	int targetBackupNum = luaL_checkinteger(L, 2);
	
	std::string backupPath = baseDir + "save" + std::to_string(targetFileNum) + "_backup" + std::to_string(targetBackupNum) + ".dat";
	struct stat tmpBuffer;
	lua_pushboolean(L, stat(backupPath.c_str(), &tmpBuffer) == 0);
	return 1;
}

// PST_BackupFetch(int targetFileNum, int targetBackupNum)
// Fetch data from the given backup
LUA_FUNCTION(Lua_PST_Backup_Fetch) {
	int targetFileNum = luaL_checkinteger(L, 1);
	int targetBackupNum = luaL_checkinteger(L, 2);

	std::string backupPath = baseDir + "save" + std::to_string(targetFileNum) + "_backup" + std::to_string(targetBackupNum) + ".dat";

	// Check that backup exists
	struct stat tmpBuffer;
	if (stat(backupPath.c_str(), &tmpBuffer) == 0) {
		std::ifstream backupFile(backupPath);
		std::ostringstream tmpStr;
		tmpStr << backupFile.rdbuf();
		
		std::string backupContent = tmpStr.str();
		lua_pushstring(L, backupContent.c_str());
		return 1;
	}

	return 0;
}

// PST_BackupReplace(int targetFileNum, int targetBackupNum)
// Attempts to replace the given savefile with the given backup file
LUA_FUNCTION(Lua_PST_Backup_Replace) {
	int targetFileNum = luaL_checkinteger(L, 1);
	int targetBackupNum = luaL_checkinteger(L, 2);

	std::string savePath = baseDir + "save" + std::to_string(targetFileNum) + ".dat";
	std::string backupPath = baseDir + "save" + std::to_string(targetFileNum) + "_backup" + std::to_string(targetBackupNum) + ".dat";

	struct stat tmpBuffer;
	if (stat(backupPath.c_str(), &tmpBuffer) == 0) {
		std::ifstream src(backupPath, std::ios::binary);
		std::ofstream dst(savePath, std::ios::binary);

		dst << src.rdbuf();
		return 1;
	}

	return 0;
}

HOOK_METHOD(LuaEngine, RegisterClasses, () -> void) {
	super();

	lua_State* L = _state;
	lua::LuaStackProtector protector(L);

	lua_pushcfunction(L, Lua_PST_Backup_Save);
	lua_setglobal(L, "PST_BackupSave");

	lua_pushcfunction(L, Lua_PST_Backup_Exists);
	lua_setglobal(L, "PST_BackupExists");

	lua_pushcfunction(L, Lua_PST_Backup_Fetch);
	lua_setglobal(L, "PST_BackupFetch");

	lua_pushcfunction(L, Lua_PST_Backup_Replace);
	lua_setglobal(L, "PST_BackupReplace");
}
