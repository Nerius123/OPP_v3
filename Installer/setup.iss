[Setup]
AppName=NerijusPonomariovas
AppVersion=1.5
DefaultDirName={pf}\VU\NerijusPonomariovas
DefaultGroupName=VU\NerijusPonomariovas
OutputDir=.
OutputBaseFilename=Setup
PrivilegesRequired=admin
DisableProgramGroupPage=yes
Compression=lzma
SolidCompression=yes

[Files]
Source: "dist\NerijusPonomariovas.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\students_10000.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\students_100000.txt"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\NerijusPonomariovas"; Filename: "{app}\NerijusPonomariovas.exe"
Name: "{commondesktop}\NerijusPonomariovas"; Filename: "{app}\NerijusPonomariovas.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Sukurti nuorodą darbalaukyje"; GroupDescription: "Nuorodos:"; Flags: unchecked
