; File 'zh_CN.nsh'
;========================================================================================
; Simplified Chinese  language file for lenmus installer/uninstaller
;---------------------------------------------------------------------------------------------------------

LicenseLangString license ${LANG_SimpChinese} ".\locale\license_en.txt"

;strings to customize MUI pages
LangString MSG_OptionalComponents   ${LANG_SimpChinese} "Please choose the optional components you would like to install"


;section titles, what user sees to select components for installation
LangString TITLE_CreateIcon     ${LANG_SimpChinese} "Shorcut on desktop"
LangString TITLE_Scores         ${LANG_SimpChinese} "Examples of music scores"
LangString TITLE_RegKeys        ${LANG_SimpChinese} "Add to Control Panel (to 'Add/Remove Programs')"

;descriptions for the sections. Displayed to user when mouse hovers over a section
LangString DESC_CreateIcon      ${LANG_SimpChinese} "Create a shorcut icon on the desktop"
LangString DESC_Scores          ${LANG_SimpChinese} "Create a folder containing some examples of music scores"
LangString DESC_RegKeys         ${LANG_SimpChinese} "Create keys in Windows Registry to add LenMus to 'Add/Remove Programs' in the Control Panel."


;error messages and other texts
LangString ERROR_CopyFiles          ${LANG_SimpChinese} "Error copying program files"
LangString ERROR_InstallFonts       ${LANG_SimpChinese} "Error installing font"
LangString ERROR_CreateIcon         ${LANG_SimpChinese} "Error creating shortcut on desktop"
LangString ERROR_CopyScores         ${LANG_SimpChinese} "Error copying examples of music scores"
LangString ERROR_FindingInstall     ${LANG_SimpChinese} "Unable to find LenMus install path. Update aborted."
LangString MSG_CONTINUE             ${LANG_SimpChinese} "Would you like to continue the installation?"
LangString MSG_ABORT                ${LANG_SimpChinese} "Installation canceled"


;start menu & desktop descriptions
LangString SM_PRODUCT_GROUP             ${LANG_SimpChinese} "${APP_NAME}"
LangString SHORTCUT_NAME_EXEC           ${LANG_SimpChinese} "Run ${APP_NAME}"
LangString SHORTCUT_NAME_UNINSTALL      ${LANG_SimpChinese} "Uninstall ${APP_NAME}"

;other strings
LangString OTHER_LangName             ${LANG_SimpChinese} "zn_CN"

;End English language file
;=========================================================================================