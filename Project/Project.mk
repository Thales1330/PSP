##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=Project
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/Thales/Documents/GitHub/PSP
ProjectPath            :=C:/Users/Thales/Documents/GitHub/PSP/Project
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Thales
Date                   :=23/02/2017
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/pspufu
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)UNICODE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="Project.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := $(shell wx-config --rcflags)
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  $(shell wx-config --libs gl) -mwindows
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -O2 -Wall $(shell wx-config --cflags) $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
WXWIN:=C:\wxWidgets-3.1.0
WXCFG:=gcc_dll\mswu
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/win_resources.rc$(ObjectSuffix) $(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/Element.cpp$(ObjectSuffix) $(IntermediateDirectory)/ArtMetro.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxGLString.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) $(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainFrameBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/WorkspaceBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElementFormBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlEditorBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/MainFrameBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/WorkspaceBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElementForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlEditorBase.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) $(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) $(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) $(IntermediateDirectory)/Load.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) $(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) $(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) $(IntermediateDirectory)/Text.cpp$(ObjectSuffix) $(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) $(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/win_resources.rc$(ObjectSuffix): win_resources.rc
	$(RcCompilerName) -i "C:/Users/Thales/Documents/GitHub/PSP/Project/win_resources.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/win_resources.rc$(ObjectSuffix) $(RcIncludePath)
$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix): ElementDataObject.cpp $(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ElementDataObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix): ElementDataObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix) -MM ElementDataObject.cpp

$(IntermediateDirectory)/ElementDataObject.cpp$(PreprocessSuffix): ElementDataObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElementDataObject.cpp$(PreprocessSuffix) ElementDataObject.cpp

$(IntermediateDirectory)/Element.cpp$(ObjectSuffix): Element.cpp $(IntermediateDirectory)/Element.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Element.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Element.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Element.cpp$(DependSuffix): Element.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Element.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Element.cpp$(DependSuffix) -MM Element.cpp

$(IntermediateDirectory)/Element.cpp$(PreprocessSuffix): Element.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Element.cpp$(PreprocessSuffix) Element.cpp

$(IntermediateDirectory)/ArtMetro.cpp$(ObjectSuffix): ArtMetro.cpp $(IntermediateDirectory)/ArtMetro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ArtMetro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ArtMetro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ArtMetro.cpp$(DependSuffix): ArtMetro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ArtMetro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ArtMetro.cpp$(DependSuffix) -MM ArtMetro.cpp

$(IntermediateDirectory)/ArtMetro.cpp$(PreprocessSuffix): ArtMetro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ArtMetro.cpp$(PreprocessSuffix) ArtMetro.cpp

$(IntermediateDirectory)/wxGLString.cpp$(ObjectSuffix): wxGLString.cpp $(IntermediateDirectory)/wxGLString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/wxGLString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxGLString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxGLString.cpp$(DependSuffix): wxGLString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxGLString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxGLString.cpp$(DependSuffix) -MM wxGLString.cpp

$(IntermediateDirectory)/wxGLString.cpp$(PreprocessSuffix): wxGLString.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxGLString.cpp$(PreprocessSuffix) wxGLString.cpp

$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix): MainFrame.cpp $(IntermediateDirectory)/MainFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/MainFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainFrame.cpp$(DependSuffix): MainFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainFrame.cpp$(DependSuffix) -MM MainFrame.cpp

$(IntermediateDirectory)/MainFrame.cpp$(PreprocessSuffix): MainFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainFrame.cpp$(PreprocessSuffix) MainFrame.cpp

$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix): Workspace.cpp $(IntermediateDirectory)/Workspace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Workspace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Workspace.cpp$(DependSuffix): Workspace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Workspace.cpp$(DependSuffix) -MM Workspace.cpp

$(IntermediateDirectory)/Workspace.cpp$(PreprocessSuffix): Workspace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Workspace.cpp$(PreprocessSuffix) Workspace.cpp

$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix): FileHanding.cpp $(IntermediateDirectory)/FileHanding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/FileHanding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileHanding.cpp$(DependSuffix): FileHanding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileHanding.cpp$(DependSuffix) -MM FileHanding.cpp

$(IntermediateDirectory)/FileHanding.cpp$(PreprocessSuffix): FileHanding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileHanding.cpp$(PreprocessSuffix) FileHanding.cpp

$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix): ControlEditor.cpp $(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ControlEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix): ControlEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix) -MM ControlEditor.cpp

$(IntermediateDirectory)/ControlEditor.cpp$(PreprocessSuffix): ControlEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlEditor.cpp$(PreprocessSuffix) ControlEditor.cpp

$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix): Camera.cpp $(IntermediateDirectory)/Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Camera.cpp$(DependSuffix): Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Camera.cpp$(DependSuffix) -MM Camera.cpp

$(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix): Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix) Camera.cpp

$(IntermediateDirectory)/MainFrameBitmaps.cpp$(ObjectSuffix): MainFrameBitmaps.cpp $(IntermediateDirectory)/MainFrameBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/MainFrameBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainFrameBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainFrameBitmaps.cpp$(DependSuffix): MainFrameBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainFrameBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainFrameBitmaps.cpp$(DependSuffix) -MM MainFrameBitmaps.cpp

$(IntermediateDirectory)/MainFrameBitmaps.cpp$(PreprocessSuffix): MainFrameBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainFrameBitmaps.cpp$(PreprocessSuffix) MainFrameBitmaps.cpp

$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(ObjectSuffix): WorkspaceBitmaps.cpp $(IntermediateDirectory)/WorkspaceBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/WorkspaceBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(DependSuffix): WorkspaceBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(DependSuffix) -MM WorkspaceBitmaps.cpp

$(IntermediateDirectory)/WorkspaceBitmaps.cpp$(PreprocessSuffix): WorkspaceBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WorkspaceBitmaps.cpp$(PreprocessSuffix) WorkspaceBitmaps.cpp

$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix): BusFormBitmaps.cpp $(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/BusFormBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix): BusFormBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix) -MM BusFormBitmaps.cpp

$(IntermediateDirectory)/BusFormBitmaps.cpp$(PreprocessSuffix): BusFormBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BusFormBitmaps.cpp$(PreprocessSuffix) BusFormBitmaps.cpp

$(IntermediateDirectory)/ElementFormBitmaps.cpp$(ObjectSuffix): ElementFormBitmaps.cpp $(IntermediateDirectory)/ElementFormBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ElementFormBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElementFormBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElementFormBitmaps.cpp$(DependSuffix): ElementFormBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElementFormBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElementFormBitmaps.cpp$(DependSuffix) -MM ElementFormBitmaps.cpp

$(IntermediateDirectory)/ElementFormBitmaps.cpp$(PreprocessSuffix): ElementFormBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElementFormBitmaps.cpp$(PreprocessSuffix) ElementFormBitmaps.cpp

$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(ObjectSuffix): ControlEditorBitmaps.cpp $(IntermediateDirectory)/ControlEditorBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ControlEditorBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(DependSuffix): ControlEditorBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(DependSuffix) -MM ControlEditorBitmaps.cpp

$(IntermediateDirectory)/ControlEditorBitmaps.cpp$(PreprocessSuffix): ControlEditorBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlEditorBitmaps.cpp$(PreprocessSuffix) ControlEditorBitmaps.cpp

$(IntermediateDirectory)/MainFrameBase.cpp$(ObjectSuffix): MainFrameBase.cpp $(IntermediateDirectory)/MainFrameBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/MainFrameBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainFrameBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainFrameBase.cpp$(DependSuffix): MainFrameBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainFrameBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainFrameBase.cpp$(DependSuffix) -MM MainFrameBase.cpp

$(IntermediateDirectory)/MainFrameBase.cpp$(PreprocessSuffix): MainFrameBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainFrameBase.cpp$(PreprocessSuffix) MainFrameBase.cpp

$(IntermediateDirectory)/WorkspaceBase.cpp$(ObjectSuffix): WorkspaceBase.cpp $(IntermediateDirectory)/WorkspaceBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/WorkspaceBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/WorkspaceBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/WorkspaceBase.cpp$(DependSuffix): WorkspaceBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/WorkspaceBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/WorkspaceBase.cpp$(DependSuffix) -MM WorkspaceBase.cpp

$(IntermediateDirectory)/WorkspaceBase.cpp$(PreprocessSuffix): WorkspaceBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/WorkspaceBase.cpp$(PreprocessSuffix) WorkspaceBase.cpp

$(IntermediateDirectory)/ElementForm.cpp$(ObjectSuffix): ElementForm.cpp $(IntermediateDirectory)/ElementForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ElementForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElementForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElementForm.cpp$(DependSuffix): ElementForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElementForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElementForm.cpp$(DependSuffix) -MM ElementForm.cpp

$(IntermediateDirectory)/ElementForm.cpp$(PreprocessSuffix): ElementForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElementForm.cpp$(PreprocessSuffix) ElementForm.cpp

$(IntermediateDirectory)/ControlEditorBase.cpp$(ObjectSuffix): ControlEditorBase.cpp $(IntermediateDirectory)/ControlEditorBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ControlEditorBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlEditorBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlEditorBase.cpp$(DependSuffix): ControlEditorBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlEditorBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlEditorBase.cpp$(DependSuffix) -MM ControlEditorBase.cpp

$(IntermediateDirectory)/ControlEditorBase.cpp$(PreprocessSuffix): ControlEditorBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlEditorBase.cpp$(PreprocessSuffix) ControlEditorBase.cpp

$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix): Bus.cpp $(IntermediateDirectory)/Bus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Bus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Bus.cpp$(DependSuffix): Bus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Bus.cpp$(DependSuffix) -MM Bus.cpp

$(IntermediateDirectory)/Bus.cpp$(PreprocessSuffix): Bus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Bus.cpp$(PreprocessSuffix) Bus.cpp

$(IntermediateDirectory)/Line.cpp$(ObjectSuffix): Line.cpp $(IntermediateDirectory)/Line.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Line.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Line.cpp$(DependSuffix): Line.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Line.cpp$(DependSuffix) -MM Line.cpp

$(IntermediateDirectory)/Line.cpp$(PreprocessSuffix): Line.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Line.cpp$(PreprocessSuffix) Line.cpp

$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix): Transformer.cpp $(IntermediateDirectory)/Transformer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Transformer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Transformer.cpp$(DependSuffix): Transformer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Transformer.cpp$(DependSuffix) -MM Transformer.cpp

$(IntermediateDirectory)/Transformer.cpp$(PreprocessSuffix): Transformer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Transformer.cpp$(PreprocessSuffix) Transformer.cpp

$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix): Machines.cpp $(IntermediateDirectory)/Machines.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Machines.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Machines.cpp$(DependSuffix): Machines.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Machines.cpp$(DependSuffix) -MM Machines.cpp

$(IntermediateDirectory)/Machines.cpp$(PreprocessSuffix): Machines.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Machines.cpp$(PreprocessSuffix) Machines.cpp

$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix): SyncGenerator.cpp $(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/SyncGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix): SyncGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix) -MM SyncGenerator.cpp

$(IntermediateDirectory)/SyncGenerator.cpp$(PreprocessSuffix): SyncGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncGenerator.cpp$(PreprocessSuffix) SyncGenerator.cpp

$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix): IndMotor.cpp $(IntermediateDirectory)/IndMotor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/IndMotor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IndMotor.cpp$(DependSuffix): IndMotor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IndMotor.cpp$(DependSuffix) -MM IndMotor.cpp

$(IntermediateDirectory)/IndMotor.cpp$(PreprocessSuffix): IndMotor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IndMotor.cpp$(PreprocessSuffix) IndMotor.cpp

$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix): Branch.cpp $(IntermediateDirectory)/Branch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Branch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Branch.cpp$(DependSuffix): Branch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Branch.cpp$(DependSuffix) -MM Branch.cpp

$(IntermediateDirectory)/Branch.cpp$(PreprocessSuffix): Branch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Branch.cpp$(PreprocessSuffix) Branch.cpp

$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix): SyncMotor.cpp $(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/SyncMotor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix): SyncMotor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix) -MM SyncMotor.cpp

$(IntermediateDirectory)/SyncMotor.cpp$(PreprocessSuffix): SyncMotor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncMotor.cpp$(PreprocessSuffix) SyncMotor.cpp

$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix): Shunt.cpp $(IntermediateDirectory)/Shunt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Shunt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Shunt.cpp$(DependSuffix): Shunt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Shunt.cpp$(DependSuffix) -MM Shunt.cpp

$(IntermediateDirectory)/Shunt.cpp$(PreprocessSuffix): Shunt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Shunt.cpp$(PreprocessSuffix) Shunt.cpp

$(IntermediateDirectory)/Load.cpp$(ObjectSuffix): Load.cpp $(IntermediateDirectory)/Load.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Load.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Load.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Load.cpp$(DependSuffix): Load.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Load.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Load.cpp$(DependSuffix) -MM Load.cpp

$(IntermediateDirectory)/Load.cpp$(PreprocessSuffix): Load.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Load.cpp$(PreprocessSuffix) Load.cpp

$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix): Inductor.cpp $(IntermediateDirectory)/Inductor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Inductor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Inductor.cpp$(DependSuffix): Inductor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Inductor.cpp$(DependSuffix) -MM Inductor.cpp

$(IntermediateDirectory)/Inductor.cpp$(PreprocessSuffix): Inductor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Inductor.cpp$(PreprocessSuffix) Inductor.cpp

$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix): Capacitor.cpp $(IntermediateDirectory)/Capacitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Capacitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Capacitor.cpp$(DependSuffix): Capacitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Capacitor.cpp$(DependSuffix) -MM Capacitor.cpp

$(IntermediateDirectory)/Capacitor.cpp$(PreprocessSuffix): Capacitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Capacitor.cpp$(PreprocessSuffix) Capacitor.cpp

$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix): PowerElement.cpp $(IntermediateDirectory)/PowerElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/PowerElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PowerElement.cpp$(DependSuffix): PowerElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PowerElement.cpp$(DependSuffix) -MM PowerElement.cpp

$(IntermediateDirectory)/PowerElement.cpp$(PreprocessSuffix): PowerElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PowerElement.cpp$(PreprocessSuffix) PowerElement.cpp

$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix): ElectricCalculation.cpp $(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ElectricCalculation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix): ElectricCalculation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix) -MM ElectricCalculation.cpp

$(IntermediateDirectory)/ElectricCalculation.cpp$(PreprocessSuffix): ElectricCalculation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElectricCalculation.cpp$(PreprocessSuffix) ElectricCalculation.cpp

$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix): PowerFlow.cpp $(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/PowerFlow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix): PowerFlow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix) -MM PowerFlow.cpp

$(IntermediateDirectory)/PowerFlow.cpp$(PreprocessSuffix): PowerFlow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PowerFlow.cpp$(PreprocessSuffix) PowerFlow.cpp

$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix): Fault.cpp $(IntermediateDirectory)/Fault.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Fault.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Fault.cpp$(DependSuffix): Fault.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Fault.cpp$(DependSuffix) -MM Fault.cpp

$(IntermediateDirectory)/Fault.cpp$(PreprocessSuffix): Fault.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Fault.cpp$(PreprocessSuffix) Fault.cpp

$(IntermediateDirectory)/Text.cpp$(ObjectSuffix): Text.cpp $(IntermediateDirectory)/Text.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/Text.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Text.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Text.cpp$(DependSuffix): Text.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Text.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Text.cpp$(DependSuffix) -MM Text.cpp

$(IntermediateDirectory)/Text.cpp$(PreprocessSuffix): Text.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Text.cpp$(PreprocessSuffix) Text.cpp

$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix): GraphicalElement.cpp $(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/GraphicalElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix): GraphicalElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix) -MM GraphicalElement.cpp

$(IntermediateDirectory)/GraphicalElement.cpp$(PreprocessSuffix): GraphicalElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GraphicalElement.cpp$(PreprocessSuffix) GraphicalElement.cpp

$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix): ControlElement.cpp $(IntermediateDirectory)/ControlElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ControlElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlElement.cpp$(DependSuffix): ControlElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlElement.cpp$(DependSuffix) -MM ControlElement.cpp

$(IntermediateDirectory)/ControlElement.cpp$(PreprocessSuffix): ControlElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlElement.cpp$(PreprocessSuffix) ControlElement.cpp

$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix): TransferFunction.cpp $(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/TransferFunction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix): TransferFunction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix) -MM TransferFunction.cpp

$(IntermediateDirectory)/TransferFunction.cpp$(PreprocessSuffix): TransferFunction.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransferFunction.cpp$(PreprocessSuffix) TransferFunction.cpp

$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix): ConnectionLine.cpp $(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ConnectionLine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix): ConnectionLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix) -MM ConnectionLine.cpp

$(IntermediateDirectory)/ConnectionLine.cpp$(PreprocessSuffix): ConnectionLine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConnectionLine.cpp$(PreprocessSuffix) ConnectionLine.cpp

$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix): BusForm.cpp $(IntermediateDirectory)/BusForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/BusForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BusForm.cpp$(DependSuffix): BusForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BusForm.cpp$(DependSuffix) -MM BusForm.cpp

$(IntermediateDirectory)/BusForm.cpp$(PreprocessSuffix): BusForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BusForm.cpp$(PreprocessSuffix) BusForm.cpp

$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix): GeneratorStabForm.cpp $(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/GeneratorStabForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix): GeneratorStabForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix) -MM GeneratorStabForm.cpp

$(IntermediateDirectory)/GeneratorStabForm.cpp$(PreprocessSuffix): GeneratorStabForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeneratorStabForm.cpp$(PreprocessSuffix) GeneratorStabForm.cpp

$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix): LineForm.cpp $(IntermediateDirectory)/LineForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/LineForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LineForm.cpp$(DependSuffix): LineForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LineForm.cpp$(DependSuffix) -MM LineForm.cpp

$(IntermediateDirectory)/LineForm.cpp$(PreprocessSuffix): LineForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LineForm.cpp$(PreprocessSuffix) LineForm.cpp

$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix): SwitchingForm.cpp $(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/SwitchingForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix): SwitchingForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix) -MM SwitchingForm.cpp

$(IntermediateDirectory)/SwitchingForm.cpp$(PreprocessSuffix): SwitchingForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwitchingForm.cpp$(PreprocessSuffix) SwitchingForm.cpp

$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix): TransformerForm.cpp $(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/TransformerForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix): TransformerForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix) -MM TransformerForm.cpp

$(IntermediateDirectory)/TransformerForm.cpp$(PreprocessSuffix): TransformerForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransformerForm.cpp$(PreprocessSuffix) TransformerForm.cpp

$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix): LoadForm.cpp $(IntermediateDirectory)/LoadForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/LoadForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LoadForm.cpp$(DependSuffix): LoadForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LoadForm.cpp$(DependSuffix) -MM LoadForm.cpp

$(IntermediateDirectory)/LoadForm.cpp$(PreprocessSuffix): LoadForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LoadForm.cpp$(PreprocessSuffix) LoadForm.cpp

$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix): ReactiveShuntElementForm.cpp $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/ReactiveShuntElementForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix): ReactiveShuntElementForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix) -MM ReactiveShuntElementForm.cpp

$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(PreprocessSuffix): ReactiveShuntElementForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(PreprocessSuffix) ReactiveShuntElementForm.cpp

$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix): IndMotorForm.cpp $(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/IndMotorForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix): IndMotorForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix) -MM IndMotorForm.cpp

$(IntermediateDirectory)/IndMotorForm.cpp$(PreprocessSuffix): IndMotorForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IndMotorForm.cpp$(PreprocessSuffix) IndMotorForm.cpp

$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix): SyncMachineForm.cpp $(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/SyncMachineForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix): SyncMachineForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix) -MM SyncMachineForm.cpp

$(IntermediateDirectory)/SyncMachineForm.cpp$(PreprocessSuffix): SyncMachineForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncMachineForm.cpp$(PreprocessSuffix) SyncMachineForm.cpp

$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix): TextForm.cpp $(IntermediateDirectory)/TextForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/TextForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TextForm.cpp$(DependSuffix): TextForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TextForm.cpp$(DependSuffix) -MM TextForm.cpp

$(IntermediateDirectory)/TextForm.cpp$(PreprocessSuffix): TextForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TextForm.cpp$(PreprocessSuffix) TextForm.cpp

$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix): TransferFunctionForm.cpp $(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Thales/Documents/GitHub/PSP/Project/TransferFunctionForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix): TransferFunctionForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix) -MM TransferFunctionForm.cpp

$(IntermediateDirectory)/TransferFunctionForm.cpp$(PreprocessSuffix): TransferFunctionForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransferFunctionForm.cpp$(PreprocessSuffix) TransferFunctionForm.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


