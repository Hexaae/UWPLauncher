## WHAT'S THIS FOR?

This simple command tool let you launch PC Game Pass (UWP) games from Steam/GOG, just specifying the EXACT full PackageFamilyName as argument. It will not open any temporary window to launch the app.

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

