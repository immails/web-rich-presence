Outfile "WebRichPresenceSetup.exe"
InstallDir "$PROGRAMFILES\WebRichPresence"

Section
  SetOutPath $INSTDIR
  File /r "build\bin\*"
SectionEnd

Function .onInstSuccess
  Exec "$INSTDIR\WebRichPresenceW.exe"
FunctionEnd