; Inno Setup script for GHEControl
; Placeholder — not functional yet. Fill in paths after the Windows CI build
; produces a deployable directory via windeployqt.

; [Setup]
; AppName=GHEControl
; AppVersion=0.1.0
; AppPublisher=<Your Name / Callsign>
; AppPublisherURL=https://example.com
; AppSupportURL=https://example.com
; DefaultDirName={autopf}\GHEControl
; DefaultGroupName=GHEControl
; OutputBaseFilename=GHEControl-0.1.0-setup
; Compression=lzma
; SolidCompression=yes
; WizardStyle=modern

; [Languages]
; Name: "english"; MessagesFile: "compiler:Default.isl"

; [Tasks]
; Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

; [Files]
; Source: "deploy\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs

; [Icons]
; Name: "{group}\GHEControl"; Filename: "{app}\GHEControl.exe"
; Name: "{commondesktop}\GHEControl"; Filename: "{app}\GHEControl.exe"; Tasks: desktopicon

; [Run]
; Filename: "{app}\GHEControl.exe"; Description: "{cm:LaunchProgram,GHEControl}"; Flags: nowait postinstall skipifsilent
