## WHAT'S THIS FOR?

This simple command tool let you launch PC Game Pass (UWP) games from Steam/GOG, just specifying the EXACT full PackageFamilyName as argument. It will not open any temporary window to launch the app. You can now (v1.1) optionally specify also the real taskname launched by the UWP game as a 2nd argument, and wait 'till it actually quits. 




FAQ\
Q: can I use additional launcher parameters to launch f.e. "game.exe -fullscreen"?\
A: unfortunately due to Microsoft current limitations you can't pass arguments to UWP apps

## ARGUMENTS
UWPLauncher.exe \<FullPackageFamilyName\> [processname]

## EXAMPLE

You want to add Oblivion Remastered for PC Game Pass (XBox app) to your Steam or GOG library...

Open a PowerShell prompt, and enter:

    Get-AppxPackage | Where-Object {$_.Name -match "bethesda"} | ForEach-Object { $appId = Select-String -Path "$($_.InstallLocation)\AppxManifest.xml" -Pattern '<Application Id="([^"]+)"' | ForEach-Object { $_.Matches.Groups[1].Value }; "$($_.PackageFamilyName)!$appId" }

This will give us as output the list of installed Apps with their PackageFamilyName and Application ID matching the string "bethesda":

    BethesdaSoftworks.Doom32004_3275kfvn8vcwc!Game
    BethesdaSoftworks.ProjectAltar_3275kfvn8vcwc!AppUEGameShipping

... our game is of course "BethesdaSoftworks.ProjectAltar...".
This will be our argument to launch the game (without opening PS window):

    UWPLauncher.exe BethesdaSoftworks.ProjectAltar_3275kfvn8vcwc!AppUEGameShipping
For exampe, [from Steam](https://help.steampowered.com/en/faqs/view/7D01-D2DD-D75E-2955), using the full PackageFamilyName as argument:

![image](https://github.com/user-attachments/assets/25e1b898-2bf3-4e0e-9bee-b483cf3398a8)

this will launch the UWP game and quit the launcher immediately.

## NEW: 
Additionally (v1.1) you can now specify also the actual task name launched by the UWP game, e.g.:

    UWPLauncher.exe BethesdaSoftworks.ProjectAltar_3275kfvn8vcwc!AppUEGameShipping "OblivionRemastered-WinGDK-Shipping.exe"

This is useful to make Steam show your status is "still playing" 'till you actually quit the game, as for any other Steam library game: if 2nd argument is present, the launcher won't exit immediately and wait for the specified task to quit.

![image](https://github.com/user-attachments/assets/ca88773a-4cea-4f4b-bce2-dbef6ebe446b)

Remember to wrap the task name with "" if the game.exe contains spaces, e.g.:

    Microsoft.TheBardsTaleARPGRR_8wekyb3d8bbwe!Game "The Bard's Tale.exe"

Enjoy!

 


[![image](https://github.com/user-attachments/assets/405b06a2-b225-4644-bd5c-2791a321f6e3)](https://paypal.me/lucalongone)


 


