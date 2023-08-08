# Sonic CD 2011 Decompilation
A Full Decompilation of Sonic CD 2011, Based on the PC remake with improvements & tweaks from the mobile remakes. Original decompilation by Rubberduckycooly.

# **SUPPORT THE OFFICIAL RELEASE OF SONIC CD**
+ Without assets from the official release this decompilation will not run.

+ You can get the official release of Sonic CD from:
  * [Windows (Via Steam)](https://store.steampowered.com/app/200940/Sonic_CD/)
  * [IOS (Via the App Store)](https://apps.apple.com/us/app/sonic-cd-classic/id454316134)
  * [Android (Via Google Play)](https://play.google.com/store/apps/details?id=com.sega.soniccd.classic&hl=en&gl=US)
  * [Android (Via Amazon)](https://www.amazon.com/Sega-of-America-Sonic-CD/dp/B008K9UZY4/ref=sr_1_2?dchild=1&keywords=Sonic+CD&qid=1607930514&sr=8-2)

Even if your platform isn't supported by the official releases, you **must** buy it for the assets (you dont need to run the official release, you just need the game assets)

# Advantages over the PC version of Sonic CD
* Sharp, pixel-perfect display
* Controls are completely remappable via the settings.ini file
* The window allows windows shortcuts to be used
* Complete support for using mobile/updated scripts, allowing for features the official PC version never got to be played on PC
* Native Windows x64 version, as well as an x86 version

# Advantages over the Mobile versions of Sonic CD
* The rendering backend is based off the PC version, so palettes are fully supported (tidal tempest water in particular)

# Additional Tweaks
* There is now a settings.ini file that the game uses to load all settings, similar to Sonic Mania
* Dev menu can now be accessed from anywhere by pressing `ESC` if enabled in the config
* The `F12` pause, `F11` step over & fast forward debug features from Sonic Mania have all be ported and are enabled if `devMenu` is enabled in the config
* If `devMenu` is enabled in the config, pressing `F10` will activate a palette overlay that shows the game's 8 internal palettes in real time
* If `useSteamDir` is set in the config, and the user is on Windows, the game will try to load savedata from Steam's `userdata` directory (where the Steam version saves to)

# How to build:
## Windows:
* Clone the repo, then follow the instructions in the [depencencies readme for Windows](./dependencies/windows/dependencies.txt) to setup dependencies, then build via the Visual Studio solution
* or grab a prebuilt executable from the releases section

## Windows UWP (Phone, Xbox, etc.):
* Clone the repo, then follow the instructions in the [depencencies readme for Windows](./dependencies/windows/dependencies.txt) and [depencencies readme for UWP](./dependencies/win-uwp/dependencies.txt) to setup dependencies, copy your `Data.rsdk` and `videos` folder into `SonicCDDecompUWP`, then build and deploy via the UWP Visual Studio solution