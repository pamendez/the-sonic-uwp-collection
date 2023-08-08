# Sonic The Hedgehog UWP Collection
This repository contains the decompilation efforts of the classic Sonic the Hedgehog games by [Rubberduckycooly](https://github.com/Rubberduckycooly), but adapted to work on the platforms that support the UWP (Univeral Windows Platform) common computing platform, those being Xbox One, Xbox Series, Windows Phone (*Does that even exist?*) and Windows 10/11. All credit goes to Rubberduckycooly and the contributors that made each decompilation possible.

This project was made just for fun and for learning purposes regarding how game porting works, at least on Xbox One/Series consoles. 

Sonic the Hedgehog is a registered trademark owned by SEGA Corporation.

<br>

______________


## Table of contents
1. [Sources and references](#1-sources-and-references)
2. [Converted games](#2-converted-games)
3. [Notes](#3-notes)
4. [Instructions and usage](#4-instructions-and-usage)
5. [To-Do List](#5-to-dos)

<br>

______________

## 1. Sources and references
The following repositories are being used to make this project possible:

1. Rubberduckycooly et al's [Sonic CD Decompilation](https://github.com/Rubberduckycooly/Sonic-CD-11-Decompilation) 
2. Rubberduckycooly et al's [Sonic the Hedgehog 1 & 2 Decompilation](https://github.com/Rubberduckycooly/Sonic-1-2-2013-Decompilation)

## 2. Converted games
The following table contains the Sonic games that are UWP ready:

| Game | UWP Ready |
| ----- | ----- |
| Sonic 1 | ❌ |
| Sonic 2 | ❌ |
| Sonic CD | ✔️ |
| Sonic 3 & Knuckles* | ❌ |

\* No decompilation of Sonic 3 & Knuckles currently exists.

## 3. Notes
1. Performance may vary between all platforms that support UWP. Personally, as of now, I own a Series X and the game runs very well, but your mileage may vary.
2. To run these games on Xbox One/Series, make sure you have a Microsoft account, a Microsoft Developer Partner account and your Xbox has developer mode activated. Read this Microsoft [documentation](https://learn.microsoft.com/en-us/windows/uwp/xbox-apps/devkit-activation#activate-developer-mode-on-your-retail-xbox-console) for more information.
3. It is required to own a legal copy of these games, whether it's the original Steam releases or Sonic Origins, the latest collection of Sonic games.
4. By default, due to how Xbox manages each application window, the game will start in fullscreen.

## 4. Instructions and usage
For each game, the dependencies and the build process are available at the root of each game's folder, inside the README.md. Since this is mostly a project focused on UWP platforms, it is **highly** recommended to use Windows 10/11 and install the following:

* Visual Studio IDE 2017 or 2019, with the C++/Visual C++ and Universal Windows Platform workloads.
* C++ Redistributables from 2010 onwards, both in their x86 and x64 versions if possible.
* Microsoft Visual C (MSVC) v142 build tools.

If you're building for Xbox consoles or Windows Phones, make sure to deploy the solution by selecting `Remote Machine` in the IDE, and specify the hostname or IP address of the device. If you're building for Windows 10/11, select `Local Machine` instead. 

# 5. To-Do's
* Port Sonic 1 & 2 to the UWP platforms with the necessary bugfixes using Rubberduckycooly's decompilation as the base.
