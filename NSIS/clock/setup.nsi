;--------------------------------
; Includes

  !include "MUI2.nsh"
  !include "logiclib.nsh"

;--------------------------------
; Custom defines
  !define NAME "ADHD Clock"
  !define APPFILE "captura.exe"
  !define VERSION "7.0.0"
  !define SLUG "${NAME} v${VERSION}"
;--------------------------------
; General

  Name "${NAME}"
  OutFile "${NAME} Setup.exe"
  InstallDir "$PROGRAMFILES\${NAME}"
  InstallDirRegKey HKCU "Software\${NAME}" ""
  RequestExecutionLevel admin

;--------------------------------
; UI
  
  !define MUI_ICON "assets\adhdLogo.ico"
  !define MUI_HEADERIMAGE
  !define MUI_WELCOMEFINISHPAGE_BITMAP "assets\adhd_installer_banner.png"
  !define MUI_HEADERIMAGE_BITMAP "assets\adhdLogo.png"
  !define MUI_ABORTWARNING
  !define MUI_WELCOMEPAGE_TITLE "${SLUG} Setup"
;--------------------------------
; Pages
  
  ; Installer pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  ; Uninstaller pages
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
  ; Set UI language
  !insertmacro MUI_LANGUAGE "English"

  ;--------------------------------
; Section - MSVC

  Section "DirectX" DirectX
    SetOutPath "$INSTDIR\Redist"
    File "redist\vc_redist.x64.exe"
    DetailPrint "Running MSVC Setup..."
    ExecWait "$INSTDIR\Redist\vc_redist.x64.exe"
    DetailPrint "Finished DirectX Setup"
  SectionEnd
;--------------------------------
; Section - Install App

  Section "-hidden app"
    SectionIn RO
    SetOutPath "$INSTDIR"
    File /r "app\*.*" 
    WriteRegStr HKCU "Software\${NAME}" "" $INSTDIR
    WriteUninstaller "$INSTDIR\Uninstall.exe"
  SectionEnd
;--------------------------------
; Section - Shortcut

  Section "Desktop Shortcut" DeskShort
    CreateShortCut "$DESKTOP\${NAME}.lnk" "$INSTDIR\${APPFILE}"
  SectionEnd
;--------------------------------
; Descriptions

  ;Language strings
  LangString DESC_DeskShort ${LANG_ENGLISH} "Create Shortcut on Dekstop."
  LangString DESC_DirectX ${LANG_ENGLISH} "Microsoft DirectX is a collection of application programming interfaces for handling tasks related to multimedia."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${DeskShort} $(DESC_DeskShort)
    !insertmacro MUI_DESCRIPTION_TEXT ${DirectX} $(DESC_DirectX)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
;--------------------------------
; Remove empty parent directories

  Function un.RMDirUP
    !define RMDirUP '!insertmacro RMDirUPCall'

    !macro RMDirUPCall _PATH
          push '${_PATH}'
          Call un.RMDirUP
    !macroend

    ; $0 - current folder
    ClearErrors

    Exch $0
    ;DetailPrint "ASDF - $0\.."
    RMDir "$0\.."

    IfErrors Skip
    ${RMDirUP} "$0\.."
    Skip:

    Pop $0

  FunctionEnd

;--------------------------------
; Section - Uninstaller

Section "Uninstall"

  ;Delete Shortcut
  Delete "$DESKTOP\${NAME}.lnk"

  ;Delete Uninstall
  Delete "$INSTDIR\Uninstall.exe"

  ;Delete Folder
  RMDir /r "$INSTDIR"
  ${RMDirUP} "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\${NAME}"

SectionEnd