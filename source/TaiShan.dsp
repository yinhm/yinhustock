# Microsoft Developer Studio Project File - Name="TaiShan" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TaiShan - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TaiShan.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaiShan.mak" CFG="TaiShan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaiShan - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TaiShan - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/TaiShan", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaiShan - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "hfiles" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 shlwapi.lib wininet.lib htmlhelp.lib userreg.lib StockTools.lib Version.lib /nologo /subsystem:windows /machine:I386 /out:"c:\temp\tc.exe" /libpath:"GDFX_Lib"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "TaiShan - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "hfiles" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib wininet.lib htmlhelp.lib userreg.lib StockTools.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"e:\天成源码\tcym源码\debug\taishan.exe" /pdbtype:sept /libpath:"GDFX_Lib"

!ENDIF 

# Begin Target

# Name "TaiShan - Win32 Release"
# Name "TaiShan - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\alcIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BjChenBenSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\BjDlgAddData.cpp
# End Source File
# Begin Source File

SOURCE=.\BjDlgChengBenSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockTypeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ByjDrawRightRect.cpp
# End Source File
# Begin Source File

SOURCE=.\CaiwuData.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlertPopDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlertSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlertSetAdditional.cpp
# End Source File
# Begin Source File

SOURCE=.\CAlertSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CFormularCompute.cpp
# End Source File
# Begin Source File

SOURCE=.\CFormularComputeParent.cpp
# End Source File
# Begin Source File

SOURCE=.\CFormularContent.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckData.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChooseReceiveDataForm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChuquanData.cpp
# End Source File
# Begin Source File

SOURCE=.\ClearData.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorCombox.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\CompanyInfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesBaseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesBigBillData.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesBigBillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesCompute.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailDays.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailList.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharesRealTimeDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\CStatusBarTai.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiChuQuanInDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiChuQuanSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiShanApp.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiShanDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiShanTesting.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiStockInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdCloseWork.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdExportFormular.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdOverWriteF.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdSelectFormu.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdSetParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CwdShowFormu.cpp
# End Source File
# Begin Source File

SOURCE=.\DayLineSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DBZLLBSH.cpp
# End Source File
# Begin Source File

SOURCE=.\DBZLLBSZ.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\dtpicker.cpp
# End Source File
# Begin Source File

SOURCE=.\EditAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\EditInvest.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportData.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font1.cpp
# End Source File
# Begin Source File

SOURCE=.\FyAddMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\FyRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\GetHardDiskID1.cpp
# End Source File
# Begin Source File

SOURCE=.\GetSetReg.cpp
# End Source File
# Begin Source File

SOURCE=.\GMemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiZhouTvView.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryFile.cpp
# End Source File
# Begin Source File

SOURCE=.\HistorySelDate.cpp
# End Source File
# Begin Source File

SOURCE=.\HqDetailList.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlShow.cpp
# End Source File
# Begin Source File

SOURCE=.\hzpy.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportData.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoBar.cpp
# End Source File
# Begin Source File

SOURCE=.\INFORMATION_SH.cpp
# End Source File
# Begin Source File

SOURCE=.\INFORMATION_SZ.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.cpp
# End Source File
# Begin Source File

SOURCE=.\InvestLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\JiShuColumnInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\JiShuColumnInfo.h
# End Source File
# Begin Source File

SOURCE=.\KEYBRODEANGEL.cpp
# End Source File
# Begin Source File

SOURCE=.\LBsInfoRS.cpp
# End Source File
# Begin Source File

SOURCE=.\ldao.cpp
# End Source File
# Begin Source File

SOURCE=.\lineRightBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ListOleDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\ListTool.cpp
# End Source File
# Begin Source File

SOURCE=.\LKeyBoardDial.cpp
# End Source File
# Begin Source File

SOURCE=.\LogonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mainFrmD.cpp
# End Source File
# Begin Source File

SOURCE=.\managedata.cpp
# End Source File
# Begin Source File

SOURCE=.\ManageInvest.cpp
# End Source File
# Begin Source File

SOURCE=.\ManagerChoose.cpp
# End Source File
# Begin Source File

SOURCE=.\ManagerStockType.cpp
# End Source File
# Begin Source File

SOURCE=.\ManagerStockTypeData.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifyStockType.cpp
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MYMAINTB.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRichView.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplash.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTipWin.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\NineGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NineShowFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NineShowView.cpp
# End Source File
# Begin Source File

SOURCE=.\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\PageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\picture2.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayGif.cpp
# End Source File
# Begin Source File

SOURCE=.\PopWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RealTimeDetailList.cpp
# End Source File
# Begin Source File

SOURCE=.\RecvStockCode.cpp
# End Source File
# Begin Source File

SOURCE=.\ReorganizeDayLineData.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditFormular.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor.cpp
# End Source File
# Begin Source File

SOURCE=.\rowcursor2.cpp
# End Source File
# Begin Source File

SOURCE=.\SBDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\secretdrvhyt.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectStock.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectStockHsDetail2.cpp
# End Source File
# Begin Source File

SOURCE=.\SetOtherDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetParam.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTimeInter.cpp
# End Source File
# Begin Source File

SOURCE=.\SHEETBHADD.cpp
# End Source File
# Begin Source File

SOURCE=.\SHEETBKCYBK.cpp
# End Source File
# Begin Source File

SOURCE=.\SHEETQT.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SHEETWG.cpp
# End Source File
# Begin Source File

SOURCE=.\SortIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\STKDRV.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolContrast.cpp
# End Source File
# Begin Source File

SOURCE=.\Sysface.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSetParaPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Syssetup.cpp
# End Source File
# Begin Source File

SOURCE=.\TaiShan.rc
# End Source File
# Begin Source File

SOURCE=.\TechNnetBP.cpp
# End Source File
# Begin Source File

SOURCE=.\TestDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Ticker.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TransactFee.cpp
# End Source File
# Begin Source File

SOURCE=.\UserRegPart2.cpp
# End Source File
# Begin Source File

SOURCE=.\WideDownLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WideDownLoadTimeSet.cpp
# End Source File
# Begin Source File

SOURCE=.\XgnDlgSelID.cpp
# End Source File
# Begin Source File

SOURCE=.\XgnExtDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XgnExtModify.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\alcIndex.h
# End Source File
# Begin Source File

SOURCE=.\AutoRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BjChenBenSetting.h
# End Source File
# Begin Source File

SOURCE=.\BjDlgAddData.h
# End Source File
# Begin Source File

SOURCE=.\BjDlgChengBenSetting.h
# End Source File
# Begin Source File

SOURCE=.\BlockTypeManager.h
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ByjDrawRightRect.h
# End Source File
# Begin Source File

SOURCE=.\CaiwuData.h
# End Source File
# Begin Source File

SOURCE=.\CAlertPopDlg.h
# End Source File
# Begin Source File

SOURCE=.\CAlertSet.h
# End Source File
# Begin Source File

SOURCE=.\CAlertSetAdditional.h
# End Source File
# Begin Source File

SOURCE=.\CAlertSystem.h
# End Source File
# Begin Source File

SOURCE=.\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\CFormularCompute.h
# End Source File
# Begin Source File

SOURCE=.\CFormularComputeParent.h
# End Source File
# Begin Source File

SOURCE=.\CFormularContent.h
# End Source File
# Begin Source File

SOURCE=.\CheckData.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChooseReceiveDataForm.h
# End Source File
# Begin Source File

SOURCE=.\ChuquanData.h
# End Source File
# Begin Source File

SOURCE=.\ClearData.h
# End Source File
# Begin Source File

SOURCE=.\ColorCombox.h
# End Source File
# Begin Source File

SOURCE=.\ColumnSelect.h
# End Source File
# Begin Source File

SOURCE=.\CompanyInfoView.h
# End Source File
# Begin Source File

SOURCE=.\CoolPopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\CSharesBaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\CSharesBigBillData.h
# End Source File
# Begin Source File

SOURCE=.\CSharesBigBillDlg.h
# End Source File
# Begin Source File

SOURCE=.\CSharesCompute.h
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailDays.h
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CSharesDetailList.h
# End Source File
# Begin Source File

SOURCE=.\CSharesInformation.h
# End Source File
# Begin Source File

SOURCE=.\CSharesRealTimeDetail.h
# End Source File
# Begin Source File

SOURCE=.\CStatusBarTai.h
# End Source File
# Begin Source File

SOURCE=.\CTaiChuQuanInDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTaiChuQuanSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTaiShanApp.h
# End Source File
# Begin Source File

SOURCE=.\CTaiShanDoc.h
# End Source File
# Begin Source File

SOURCE=.\CTaiShanTesting.h
# End Source File
# Begin Source File

SOURCE=.\CTaiStockInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\CwdCloseWork.h
# End Source File
# Begin Source File

SOURCE=.\CwdExportFormular.h
# End Source File
# Begin Source File

SOURCE=.\CwdOverWriteF.h
# End Source File
# Begin Source File

SOURCE=.\CwdSelectFormu.h
# End Source File
# Begin Source File

SOURCE=.\CwdSetParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\CwdShowFormu.h
# End Source File
# Begin Source File

SOURCE=.\DayLineSelect.h
# End Source File
# Begin Source File

SOURCE=.\DBZLLBSH.h
# End Source File
# Begin Source File

SOURCE=.\DBZLLBSZ.h
# End Source File
# Begin Source File

SOURCE=.\DetailGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\dtpicker.h
# End Source File
# Begin Source File

SOURCE=.\EditAccount.h
# End Source File
# Begin Source File

SOURCE=.\EditInvest.h
# End Source File
# Begin Source File

SOURCE=.\ExportData.h
# End Source File
# Begin Source File

SOURCE=.\FloatEdit.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\font1.h
# End Source File
# Begin Source File

SOURCE=.\FuncList.h
# End Source File
# Begin Source File

SOURCE=.\FyAddMoney.h
# End Source File
# Begin Source File

SOURCE=.\FyRegister.h
# End Source File
# Begin Source File

SOURCE=.\GetHardDiskID1.h
# End Source File
# Begin Source File

SOURCE=.\GetSetReg.h
# End Source File
# Begin Source File

SOURCE=.\GMemDC.h
# End Source File
# Begin Source File

SOURCE=.\Grid.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GuiZhouTvView.h
# End Source File
# Begin Source File

SOURCE=.\HistoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\HistoryFile.h
# End Source File
# Begin Source File

SOURCE=.\HistorySelDate.h
# End Source File
# Begin Source File

SOURCE=.\HqDetailList.h
# End Source File
# Begin Source File

SOURCE=.\htmlhelp.h
# End Source File
# Begin Source File

SOURCE=.\HtmlShow.h
# End Source File
# Begin Source File

SOURCE=.\hzpy.h
# End Source File
# Begin Source File

SOURCE=.\ImportData.h
# End Source File
# Begin Source File

SOURCE=.\InfoBar.h
# End Source File
# Begin Source File

SOURCE=.\INFORMATION_SH.h
# End Source File
# Begin Source File

SOURCE=.\INFORMATION_SZ.h
# End Source File
# Begin Source File

SOURCE=.\InfoView.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=.\InvestLogin.h
# End Source File
# Begin Source File

SOURCE=.\KEYBRODEANGEL.h
# End Source File
# Begin Source File

SOURCE=.\LBsInfoRS.h
# End Source File
# Begin Source File

SOURCE=.\ldao.h
# End Source File
# Begin Source File

SOURCE=.\lineRightBox.h
# End Source File
# Begin Source File

SOURCE=.\ListOleDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\ListTool.h
# End Source File
# Begin Source File

SOURCE=.\LKeyBoardDial.h
# End Source File
# Begin Source File

SOURCE=.\LogonDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mainFrmD.h
# End Source File
# Begin Source File

SOURCE=.\managedata.h
# End Source File
# Begin Source File

SOURCE=.\ManageInvest.h
# End Source File
# Begin Source File

SOURCE=.\ManagerChoose.h
# End Source File
# Begin Source File

SOURCE=.\ManagerStockType.h
# End Source File
# Begin Source File

SOURCE=.\ManagerStockTypeData.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\ModifyStockType.h
# End Source File
# Begin Source File

SOURCE=.\msflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\MYMAINTB.h
# End Source File
# Begin Source File

SOURCE=.\MyRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyRichView.h
# End Source File
# Begin Source File

SOURCE=.\MySplash.h
# End Source File
# Begin Source File

SOURCE=.\MySplitter.h
# End Source File
# Begin Source File

SOURCE=.\GDFX_Include\MyStkWinApp.h
# End Source File
# Begin Source File

SOURCE=.\MyTipWin.h
# End Source File
# Begin Source File

SOURCE=.\MYToolbar.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeView.h
# End Source File
# Begin Source File

SOURCE=.\NineGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NineShowFrm.h
# End Source File
# Begin Source File

SOURCE=.\NineShowView.h
# End Source File
# Begin Source File

SOURCE=.\Option.h
# End Source File
# Begin Source File

SOURCE=.\PageWnd.h
# End Source File
# Begin Source File

SOURCE=.\ParamEdit.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\picture2.h
# End Source File
# Begin Source File

SOURCE=.\PlayGif.h
# End Source File
# Begin Source File

SOURCE=.\PopWnd.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\RealTimeDetailList.h
# End Source File
# Begin Source File

SOURCE=.\RecvStockCode.h
# End Source File
# Begin Source File

SOURCE=.\ReorganizeDayLineData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichEditFormular.h
# End Source File
# Begin Source File

SOURCE=.\rowcursor.h
# End Source File
# Begin Source File

SOURCE=.\rowcursor2.h
# End Source File
# Begin Source File

SOURCE=.\SBDestination.h
# End Source File
# Begin Source File

SOURCE=.\secretdrvhyt.h
# End Source File
# Begin Source File

SOURCE=.\SelectStock.h
# End Source File
# Begin Source File

SOURCE=.\SelectStockHsDetail2.h
# End Source File
# Begin Source File

SOURCE=.\SetOtherDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetParam.h
# End Source File
# Begin Source File

SOURCE=.\SetTimeInter.h
# End Source File
# Begin Source File

SOURCE=.\SHEETBHADD.h
# End Source File
# Begin Source File

SOURCE=.\SHEETBKCYBK.h
# End Source File
# Begin Source File

SOURCE=.\SHEETDMDZ.h
# End Source File
# Begin Source File

SOURCE=.\SHEETQT.h
# End Source File
# Begin Source File

SOURCE=.\SheetScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\SHEETWG.h
# End Source File
# Begin Source File

SOURCE=.\SortIndex.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STKDRV.h
# End Source File
# Begin Source File

SOURCE=.\Stockdrv.h
# End Source File
# Begin Source File

SOURCE=.\StructKlineView.h
# End Source File
# Begin Source File

SOURCE=.\StructTaiShares.h
# End Source File
# Begin Source File

SOURCE=.\StructView.h
# End Source File
# Begin Source File

SOURCE=.\SymbolContrast.h
# End Source File
# Begin Source File

SOURCE=.\Sysface.h
# End Source File
# Begin Source File

SOURCE=.\SysSetParaPage.h
# End Source File
# Begin Source File

SOURCE=.\Syssetup.h
# End Source File
# Begin Source File

SOURCE=.\TechNnetBP.h
# End Source File
# Begin Source File

SOURCE=.\TestDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\Ticker.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\totalh.h
# End Source File
# Begin Source File

SOURCE=.\TransactFee.h
# End Source File
# Begin Source File

SOURCE=.\UserRegPart2.h
# End Source File
# Begin Source File

SOURCE=.\WideDownLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\WideDownLoadTimeSet.h
# End Source File
# Begin Source File

SOURCE=.\XgnDlgSelID.h
# End Source File
# Begin Source File

SOURCE=.\XgnExtDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\XgnExtModify.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\111.ico
# End Source File
# Begin Source File

SOURCE=.\res\222.ico
# End Source File
# Begin Source File

SOURCE=.\res\32x32x16_Exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\4way06.cur
# End Source File
# Begin Source File

SOURCE=.\res\67.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BELL.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap15.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap18.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap19.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap23.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bitmap24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_k.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bj_cheng.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00024.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00025.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00026.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00027.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00028.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00029.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00030.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00031.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00032.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00088.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00099.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Clsdfold.ico
# End Source File
# Begin Source File

SOURCE=.\res\cover.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cross01.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor3.cur
# End Source File
# Begin Source File

SOURCE=.\res\dadan.ico
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=.\res\Dragcopy.cur
# End Source File
# Begin Source File

SOURCE=.\res\Dragmove.cur
# End Source File
# Begin Source File

SOURCE=.\Res\drawicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drawline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Ew_07.cur
# End Source File
# Begin Source File

SOURCE=.\res\Eye.ico
# End Source File
# Begin Source File

SOURCE=.\res\Face02.ico
# End Source File
# Begin Source File

SOURCE=.\res\Graph03.ico
# End Source File
# Begin Source File

SOURCE=.\res\Graph04.ico
# End Source File
# Begin Source File

SOURCE=.\res\Graph16.ico
# End Source File
# Begin Source File

SOURCE=.\res\H_cross.cur
# End Source File
# Begin Source File

SOURCE=.\res\H_point.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\hottoolb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hq.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon10.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon9.ico
# End Source File
# Begin Source File

SOURCE=.\Res\idb_bmp_menu.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\idb_every.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\idb_statusbar_indicator.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mytr.ico
# End Source File
# Begin Source File

SOURCE=.\res\l_bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Misc05.ico
# End Source File
# Begin Source File

SOURCE=.\res\Misc06.ico
# End Source File
# Begin Source File

SOURCE=.\res\mylist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Nodrop02.cur
# End Source File
# Begin Source File

SOURCE=.\res\Normal08.cur
# End Source File
# Begin Source File

SOURCE=.\res\Note06.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ns_05.cur
# End Source File
# Begin Source File

SOURCE=.\res\Open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\open.ico
# End Source File
# Begin Source File

SOURCE=.\res\Palette.cur
# End Source File
# Begin Source File

SOURCE=.\res\pc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sinewave.ico
# End Source File
# Begin Source File

SOURCE=.\res\statusba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TaiShan.ico
# End Source File
# Begin Source File

SOURCE=.\res\TaiShan.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Tongshi.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarDis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treebmp1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treebmp2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treebmp3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\user.ico
# End Source File
# Begin Source File

SOURCE=.\res\vwbaseDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\WsStock2000Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\wwe.ico
# End Source File
# Begin Source File

SOURCE=.\res\xg.ico
# End Source File
# Begin Source File

SOURCE=.\res\xx.ico
# End Source File
# Begin Source File

SOURCE=.\res\xx1.ico
# End Source File
# Begin Source File

SOURCE=.\res\zx.ico
# End Source File
# Begin Source File

SOURCE=".\res\图形1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\图形2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\图形3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\图形4.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\图形6.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\选股主目录.ico"
# End Source File
# End Group
# Begin Group "dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CTaiKlineColorTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineColorTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogChangKLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogChangKLine.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogCross.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogCross.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogKLineMode.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogKLineMode.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogShiDuanTJ.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDialogShiDuanTJ.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgAddSelfKline.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgAddSelfKline.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgBaseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgBaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgCanUsedPeriod.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgCanUsedPeriod.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgChangeIndexParam.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgChangeIndexParam.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgDealHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgDealHistory.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgEditIndecator.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgEditIndecator.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgEditScreenStock.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgEditScreenStock.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgHistorySelect.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgHistorySelect.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgIndicatorOften.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgIndicatorOften.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgJishuClassify.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgJishuClassify.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgNeuralLearn.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgNeuralLearn.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgNNetEditData.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgNNetEditData.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgParamExplain.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgParamExplain.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgStopLose.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgStopLose.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgYHParam.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgYHParam.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgZhibiaoTongji.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDlgZhibiaoTongji.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineExportExecel.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineExportExecel.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineIndicatorsManage.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineIndicatorsManage.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSetMaxMin.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSetMaxMin.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSetNameTreeGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSetNameTreeGroup.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineShowText.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineShowText.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSplitWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineSplitWnd.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineTransferKline.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineTransferKline.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineWideNet.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineWideNet.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineWndChild.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineWndChild.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreen.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenParent.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenParent.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreensIndicate.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreensIndicate.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTest.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTest.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestBuySellResult.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestBuySellResult.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestDetial.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestDetial.h
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestResult.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiScreenTestResult.h
# End Source File
# Begin Source File

SOURCE=.\DiaCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\DiaCopy.h
# End Source File
# Begin Source File

SOURCE=.\DIAEDITZBGSHS.cpp
# End Source File
# Begin Source File

SOURCE=.\DIAEDITZBGSHS.h
# End Source File
# Begin Source File

SOURCE=.\DIAEDITZJXT.cpp
# End Source File
# Begin Source File

SOURCE=.\DIAEDITZJXT.h
# End Source File
# Begin Source File

SOURCE=.\DiaGetUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\DiaGetUpdate.h
# End Source File
# Begin Source File

SOURCE=.\DIAGSGLMM.cpp
# End Source File
# Begin Source File

SOURCE=.\DIAGSGLMM.h
# End Source File
# Begin Source File

SOURCE=.\DialogCwdLeftPop.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogCwdLeftPop.h
# End Source File
# Begin Source File

SOURCE=.\DIALOGGSGL.cpp
# End Source File
# Begin Source File

SOURCE=.\DIALOGGSGL.h
# End Source File
# Begin Source File

SOURCE=.\DIALOGGSSM.cpp
# End Source File
# Begin Source File

SOURCE=.\DIALOGGSSM.h
# End Source File
# Begin Source File

SOURCE=.\DialogProgressPingce.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProgressPingce.h
# End Source File
# Begin Source File

SOURCE=.\DIALOGTEST.cpp
# End Source File
# Begin Source File

SOURCE=.\DIALOGTEST.h
# End Source File
# Begin Source File

SOURCE=.\DIALOGXZZB.cpp
# End Source File
# Begin Source File

SOURCE=.\DIALOGXZZB.h
# End Source File
# Begin Source File

SOURCE=.\DiaStockdetail.cpp
# End Source File
# Begin Source File

SOURCE=.\DiaStockdetail.h
# End Source File
# Begin Source File

SOURCE=.\DiaTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DiaTest.h
# End Source File
# Begin Source File

SOURCE=.\DIGEDITTJXG.cpp
# End Source File
# Begin Source File

SOURCE=.\DIGEDITTJXG.h
# End Source File
# Begin Source File

SOURCE=.\DlgMYXMZ.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMYXMZ.h
# End Source File
# Begin Source File

SOURCE=.\DlgShowInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShowInformation.h
# End Source File
# Begin Source File

SOURCE=.\DownLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DownLoadDlg.h
# End Source File
# End Group
# Begin Group "views"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CTaiShanKlineShowView.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiShanKlineShowView.h
# End Source File
# Begin Source File

SOURCE=.\CTaiShanReportView.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiShanReportView.h
# End Source File
# End Group
# Begin Group "datacompute"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CTaiKlineDoFenshi.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDoFenshi.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDoKline.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDoKline.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDrawLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineDrawLine.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineFileHS.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineFileHS.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineFileKLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineFileKLine.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineMemFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineMemFile.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineMoveChenben.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineMoveChenben.h
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineTreeCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\CTaiKlineTreeCtl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ZtZip.tlh
# End Source File
# Begin Source File

SOURCE=.\ZtZip.tli
# End Source File
# End Target
# End Project
# Section TaiShan : {0DFCCC88-C3C7-4B8C-893D-0E0D37314881}
# 	1:15:IDD_FY_REGISTER:213
# 	2:16:Resource Include:resource.h
# 	2:12:FyRegister.h:FyRegister.h
# 	2:10:ENUM: enum:enum
# 	2:17:CLASS: FyRegister:FyRegister
# 	2:14:FyRegister.cpp:FyRegister.cpp
# 	2:15:IDD_FY_REGISTER:IDD_FY_REGISTER
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {697FAB96-0F95-4116-AFBD-13A0575EC170}
# 	2:21:TechColorTreeCtrl.cpp:TechColorTreeCtrl.cpp
# 	2:19:TechColorTreeCtrl.h:TechColorTreeCtrl.h
# 	2:25:CLASS: CTechColorTreeCtrl:CTechColorTreeCtrl
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {9F6AA700-D188-11CD-AD48-00AA003C9CB6}
# 	2:5:Class:CRowCursor1
# 	2:10:HeaderFile:rowcursor2.h
# 	2:8:ImplFile:rowcursor2.cpp
# End Section
# Section TaiShan : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:25:CG_IDR_POPUP_MY_TREE_VIEW:114
# End Section
# Section TaiShan : {E6285E23-D559-43F6-9A25-5559C6DBABA1}
# 	2:14:CLASS: CTicker:CTicker
# 	2:19:Application Include:wsstock2000.h
# 	2:10:Ticker.cpp:Ticker.cpp
# 	2:8:Ticker.h:Ticker.h
# End Section
# Section TaiShan : {92A100DA-F0CE-45C9-AC8E-3ABEC95D2625}
# 	1:15:IDD_FY_ADDMONEY:214
# 	2:16:Resource Include:resource.h
# 	2:17:CLASS: FyAddMoney:FyAddMoney
# 	2:15:IDD_FY_ADDMONEY:IDD_FY_ADDMONEY
# 	2:10:ENUM: enum:enum
# 	2:12:FyAddMoney.h:FyAddMoney.h
# 	2:14:FyAddMoney.cpp:FyAddMoney.cpp
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {6262D3A0-531B-11CF-91F6-C2863C385E30}
# 	2:21:DefaultSinkHeaderFile:msflexgrid.h
# 	2:16:DefaultSinkClass:CMSFlexGrid
# End Section
# Section TaiShan : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont1
# 	2:10:HeaderFile:font1.h
# 	2:8:ImplFile:font1.cpp
# End Section
# Section TaiShan : {5F4DF280-531B-11CF-91F6-C2863C385E30}
# 	2:5:Class:CMSFlexGrid
# 	2:10:HeaderFile:msflexgrid.h
# 	2:8:ImplFile:msflexgrid.cpp
# End Section
# Section TaiShan : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:12:IDB_BITMAP81:121
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section TaiShan : {44C5E258-51A0-439E-8F95-CE2E840BB351}
# 	2:18:AutoRichEditCtrl.h:AutoRichEditCtrl.h
# 	2:20:AutoRichEditCtrl.cpp:AutoRichEditCtrl.cpp
# 	2:19:Application Include:wsstock2000.h
# 	2:24:CLASS: CAutoRichEditCtrl:CAutoRichEditCtrl
# End Section
# Section TaiShan : {F86FF346-EE4A-11CE-9DFC-9C7D37B2C919}
# 	2:5:Class:CctMeter
# 	2:10:HeaderFile:ctmeter.h
# 	2:8:ImplFile:ctmeter.cpp
# End Section
# Section TaiShan : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture1
# 	2:10:HeaderFile:picture2.h
# 	2:8:ImplFile:picture2.cpp
# End Section
# Section TaiShan : {F86FF345-EE4A-11CE-9DFC-9C7D37B2C919}
# 	2:21:DefaultSinkHeaderFile:ctmeter.h
# 	2:16:DefaultSinkClass:CctMeter
# End Section
# Section TaiShan : {1E7CB8AF-78EA-4456-8085-33D38AE91E15}
# 	2:13:CLASS: CMemDC:CMemDC
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CButtonST:CButtonST
# 	2:9:BtnST.cpp:BtnST.cpp
# 	2:19:Application Include:wsstock2000.h
# 	2:7:BtnST.h:BtnST.h
# End Section
# Section TaiShan : {9F609F61-7710-11D6-968D-EB9E58A6C175}
# 	2:10:HtmlShow.h:HtmlShow.h
# 	2:12:HtmlShow.cpp:HtmlShow.cpp
# 	2:16:CLASS: CHtmlShow:CHtmlShow
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {A1A61C68-8E57-4B8D-89FA-1A97CEBE88D4}
# 	2:20:BlockTypeManager.cpp:BlockTypeManager.cpp
# 	2:18:BlockTypeManager.h:BlockTypeManager.h
# 	2:23:CLASS: BlockTypeManager:BlockTypeManager
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {988176DF-463A-40A6-BDAD-0B34CE8020A5}
# 	2:16:CLASS: GetSetReg:GetSetReg
# 	2:11:GetSetReg.h:GetSetReg.h
# 	2:13:GetSetReg.cpp:GetSetReg.cpp
# 	2:19:Application Include:wsstock2000.h
# End Section
# Section TaiShan : {5EBAEB05-EB0F-11D1-88EB-0080C8E20736}
# 	2:19:CompanyInfoView.cpp:CompanyInfoView1.cpp
# 	2:17:CompanyInfoView.h:CompanyInfoView1.h
# 	2:23:CLASS: CCompanyInfoView:CCompanyInfoView
# 	2:19:Application Include:wsstock2000.h
# End Section
