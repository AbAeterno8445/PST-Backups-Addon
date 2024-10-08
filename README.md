# Passive Skill Trees - Save Backups Addon

This little DLL addon allows the Passive Skill Trees mod to periodically create backups of your mod's savefiles to try and protect against data loss/corruption. Bear in mind that this only deals with mod data, the game's savefiles are unaffected by this or the mod itself.

## **Installation**

Requires REPENTOGON to be installed.

Grab the [latest release](https://github.com/AbAeterno8445/PST-Backups-Addon/releases) and drop the .dll file in your Isaac installation directory.

## **How it works**

Whenever you start a run, a backup will be created in the same directory as the mod's save data (Isaac installation dir -> data/passive skill trees/), named after the savefile # such as "save3_backup1.dat", "save1_backup3.dat", etc.

If the mod detects data loss and backups are present, a prompt will appear when opening the skill tree, allowing you to pick one of the available backups to try and load. The mod considers it a 'data loss' situation when your global level is 1 and backup files are available.

By default up to 3 backup files per savefile are created, and then it cycles through the backups again, replacing the first, then the second, etc.

You can increase the max amount of backups per savefile by going to the "PST_config.lua" file in the mod's directory, and changing the "maxBackups" option.

Bear in mind that backups don't save mid-run tree snapshot changes, meaning that loading a backup then continuing a run will likely make you lose temporary effects and buffs granted by the tree gained throughout that run.
