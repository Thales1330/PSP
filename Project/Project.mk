##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release_Windows_x64
ProjectName            :=Project
ConfigurationName      :=Release_Windows_x64
WorkspacePath          :=C:/Users/NDSE-69/Documents/GitHub/PSP
ProjectPath            :=C:/Users/NDSE-69/Documents/GitHub/PSP/Project
IntermediateDirectory  :=./Release_Windows_x64
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=NDSE-69
Date                   :=27/03/2018
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
LinkOptions            :=  $(shell wx-config --libs std,ribbon,aui,propgrid,richtext,gl) -mwindows
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
CXXFLAGS :=  -O2 -Wall $(shell wx-config --cflags) -std=gnu++11 $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
WXWIN:=C:\wxWidgets-3.1.0
WXCFG:=gcc_dll\mswu
Objects0=$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElementPlotData.cpp$(ObjectSuffix) $(IntermediateDirectory)/ChartView.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) $(IntermediateDirectory)/GainForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/Constant.cpp$(ObjectSuffix) $(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(ObjectSuffix) $(IntermediateDirectory)/XMLParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/fparser_fpoptimizer.cc$(ObjectSuffix) \
	$(IntermediateDirectory)/fparser_fparser.cc$(ObjectSuffix) $(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/Divider.cpp$(ObjectSuffix) $(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_ChartViewBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/SumForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/PropertiesData.cpp$(ObjectSuffix) $(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_ControlEditorBase.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ConstantForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/Gain.cpp$(ObjectSuffix) $(IntermediateDirectory)/DataReport.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_MainFrameBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/SimulationsSettingsForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/MathExprParser.cpp$(ObjectSuffix) $(IntermediateDirectory)/Line.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlSystemTest.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_DataReportBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/win_resources.rc$(ObjectSuffix) $(IntermediateDirectory)/base_WorkspaceBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) $(IntermediateDirectory)/IOControlForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) $(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) $(IntermediateDirectory)/Element.cpp$(ObjectSuffix) $(IntermediateDirectory)/RateLimiterForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/Electromechanical.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_PropertiesFormBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/Multiplier.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) $(IntermediateDirectory)/ControlElementContainer.cpp$(ObjectSuffix) $(IntermediateDirectory)/Exponential.cpp$(ObjectSuffix) $(IntermediateDirectory)/Limiter.cpp$(ObjectSuffix) $(IntermediateDirectory)/RateLimiter.cpp$(ObjectSuffix) $(IntermediateDirectory)/MathOperation.cpp$(ObjectSuffix) $(IntermediateDirectory)/MathExpression.cpp$(ObjectSuffix) $(IntermediateDirectory)/artProvider_ArtMetro.cpp$(ObjectSuffix) $(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) $(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/OpenGLText.cpp$(ObjectSuffix) $(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Text.cpp$(ObjectSuffix) $(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) $(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) $(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) $(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) $(IntermediateDirectory)/Load.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_ElementFormBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) $(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/IOControl.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) $(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/ExponentialForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ImportForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/AboutForm.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/ControlElementSolver.cpp$(ObjectSuffix) $(IntermediateDirectory)/LimiterForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/MathExpressionForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) $(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/GeneralPropertiesForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sum.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Release_Windows_x64"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Release_Windows_x64"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix): TransformerForm.cpp $(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/TransformerForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix): TransformerForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransformerForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransformerForm.cpp$(DependSuffix) -MM TransformerForm.cpp

$(IntermediateDirectory)/TransformerForm.cpp$(PreprocessSuffix): TransformerForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransformerForm.cpp$(PreprocessSuffix) TransformerForm.cpp

$(IntermediateDirectory)/ElementPlotData.cpp$(ObjectSuffix): ElementPlotData.cpp $(IntermediateDirectory)/ElementPlotData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ElementPlotData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElementPlotData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElementPlotData.cpp$(DependSuffix): ElementPlotData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElementPlotData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElementPlotData.cpp$(DependSuffix) -MM ElementPlotData.cpp

$(IntermediateDirectory)/ElementPlotData.cpp$(PreprocessSuffix): ElementPlotData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElementPlotData.cpp$(PreprocessSuffix) ElementPlotData.cpp

$(IntermediateDirectory)/ChartView.cpp$(ObjectSuffix): ChartView.cpp $(IntermediateDirectory)/ChartView.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ChartView.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ChartView.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ChartView.cpp$(DependSuffix): ChartView.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ChartView.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ChartView.cpp$(DependSuffix) -MM ChartView.cpp

$(IntermediateDirectory)/ChartView.cpp$(PreprocessSuffix): ChartView.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ChartView.cpp$(PreprocessSuffix) ChartView.cpp

$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix): ElectricCalculation.cpp $(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ElectricCalculation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix): ElectricCalculation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElectricCalculation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElectricCalculation.cpp$(DependSuffix) -MM ElectricCalculation.cpp

$(IntermediateDirectory)/ElectricCalculation.cpp$(PreprocessSuffix): ElectricCalculation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElectricCalculation.cpp$(PreprocessSuffix) ElectricCalculation.cpp

$(IntermediateDirectory)/GainForm.cpp$(ObjectSuffix): GainForm.cpp $(IntermediateDirectory)/GainForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/GainForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GainForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GainForm.cpp$(DependSuffix): GainForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GainForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GainForm.cpp$(DependSuffix) -MM GainForm.cpp

$(IntermediateDirectory)/GainForm.cpp$(PreprocessSuffix): GainForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GainForm.cpp$(PreprocessSuffix) GainForm.cpp

$(IntermediateDirectory)/Constant.cpp$(ObjectSuffix): Constant.cpp $(IntermediateDirectory)/Constant.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Constant.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Constant.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Constant.cpp$(DependSuffix): Constant.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Constant.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Constant.cpp$(DependSuffix) -MM Constant.cpp

$(IntermediateDirectory)/Constant.cpp$(PreprocessSuffix): Constant.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Constant.cpp$(PreprocessSuffix) Constant.cpp

$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix): Camera.cpp $(IntermediateDirectory)/Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Camera.cpp$(DependSuffix): Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Camera.cpp$(DependSuffix) -MM Camera.cpp

$(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix): Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix) Camera.cpp

$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(ObjectSuffix): wxMathPlot/mathplot.cpp $(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/wxMathPlot/mathplot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(DependSuffix): wxMathPlot/mathplot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(DependSuffix) -MM wxMathPlot/mathplot.cpp

$(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(PreprocessSuffix): wxMathPlot/mathplot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/wxMathPlot_mathplot.cpp$(PreprocessSuffix) wxMathPlot/mathplot.cpp

$(IntermediateDirectory)/XMLParser.cpp$(ObjectSuffix): XMLParser.cpp $(IntermediateDirectory)/XMLParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/XMLParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/XMLParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/XMLParser.cpp$(DependSuffix): XMLParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/XMLParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/XMLParser.cpp$(DependSuffix) -MM XMLParser.cpp

$(IntermediateDirectory)/XMLParser.cpp$(PreprocessSuffix): XMLParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/XMLParser.cpp$(PreprocessSuffix) XMLParser.cpp

$(IntermediateDirectory)/fparser_fpoptimizer.cc$(ObjectSuffix): fparser/fpoptimizer.cc $(IntermediateDirectory)/fparser_fpoptimizer.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/fparser/fpoptimizer.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fparser_fpoptimizer.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fparser_fpoptimizer.cc$(DependSuffix): fparser/fpoptimizer.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fparser_fpoptimizer.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/fparser_fpoptimizer.cc$(DependSuffix) -MM fparser/fpoptimizer.cc

$(IntermediateDirectory)/fparser_fpoptimizer.cc$(PreprocessSuffix): fparser/fpoptimizer.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fparser_fpoptimizer.cc$(PreprocessSuffix) fparser/fpoptimizer.cc

$(IntermediateDirectory)/fparser_fparser.cc$(ObjectSuffix): fparser/fparser.cc $(IntermediateDirectory)/fparser_fparser.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/fparser/fparser.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/fparser_fparser.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/fparser_fparser.cc$(DependSuffix): fparser/fparser.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/fparser_fparser.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/fparser_fparser.cc$(DependSuffix) -MM fparser/fparser.cc

$(IntermediateDirectory)/fparser_fparser.cc$(PreprocessSuffix): fparser/fparser.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/fparser_fparser.cc$(PreprocessSuffix) fparser/fparser.cc

$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix): MainFrame.cpp $(IntermediateDirectory)/MainFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/MainFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainFrame.cpp$(DependSuffix): MainFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MainFrame.cpp$(DependSuffix) -MM MainFrame.cpp

$(IntermediateDirectory)/MainFrame.cpp$(PreprocessSuffix): MainFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainFrame.cpp$(PreprocessSuffix) MainFrame.cpp

$(IntermediateDirectory)/Divider.cpp$(ObjectSuffix): Divider.cpp $(IntermediateDirectory)/Divider.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Divider.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Divider.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Divider.cpp$(DependSuffix): Divider.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Divider.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Divider.cpp$(DependSuffix) -MM Divider.cpp

$(IntermediateDirectory)/Divider.cpp$(PreprocessSuffix): Divider.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Divider.cpp$(PreprocessSuffix) Divider.cpp

$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix): ElementDataObject.cpp $(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ElementDataObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix): ElementDataObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ElementDataObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ElementDataObject.cpp$(DependSuffix) -MM ElementDataObject.cpp

$(IntermediateDirectory)/ElementDataObject.cpp$(PreprocessSuffix): ElementDataObject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ElementDataObject.cpp$(PreprocessSuffix) ElementDataObject.cpp

$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix): Fault.cpp $(IntermediateDirectory)/Fault.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Fault.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Fault.cpp$(DependSuffix): Fault.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Fault.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Fault.cpp$(DependSuffix) -MM Fault.cpp

$(IntermediateDirectory)/Fault.cpp$(PreprocessSuffix): Fault.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Fault.cpp$(PreprocessSuffix) Fault.cpp

$(IntermediateDirectory)/base_ChartViewBase.cpp$(ObjectSuffix): base/ChartViewBase.cpp $(IntermediateDirectory)/base_ChartViewBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ChartViewBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ChartViewBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ChartViewBase.cpp$(DependSuffix): base/ChartViewBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ChartViewBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ChartViewBase.cpp$(DependSuffix) -MM base/ChartViewBase.cpp

$(IntermediateDirectory)/base_ChartViewBase.cpp$(PreprocessSuffix): base/ChartViewBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ChartViewBase.cpp$(PreprocessSuffix) base/ChartViewBase.cpp

$(IntermediateDirectory)/SumForm.cpp$(ObjectSuffix): SumForm.cpp $(IntermediateDirectory)/SumForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SumForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SumForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SumForm.cpp$(DependSuffix): SumForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SumForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SumForm.cpp$(DependSuffix) -MM SumForm.cpp

$(IntermediateDirectory)/SumForm.cpp$(PreprocessSuffix): SumForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SumForm.cpp$(PreprocessSuffix) SumForm.cpp

$(IntermediateDirectory)/PropertiesData.cpp$(ObjectSuffix): PropertiesData.cpp $(IntermediateDirectory)/PropertiesData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/PropertiesData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PropertiesData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PropertiesData.cpp$(DependSuffix): PropertiesData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PropertiesData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PropertiesData.cpp$(DependSuffix) -MM PropertiesData.cpp

$(IntermediateDirectory)/PropertiesData.cpp$(PreprocessSuffix): PropertiesData.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PropertiesData.cpp$(PreprocessSuffix) PropertiesData.cpp

$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix): BusFormBitmaps.cpp $(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/BusFormBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix): BusFormBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BusFormBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BusFormBitmaps.cpp$(DependSuffix) -MM BusFormBitmaps.cpp

$(IntermediateDirectory)/BusFormBitmaps.cpp$(PreprocessSuffix): BusFormBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BusFormBitmaps.cpp$(PreprocessSuffix) BusFormBitmaps.cpp

$(IntermediateDirectory)/base_ControlEditorBase.cpp$(ObjectSuffix): base/ControlEditorBase.cpp $(IntermediateDirectory)/base_ControlEditorBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ControlEditorBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ControlEditorBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ControlEditorBase.cpp$(DependSuffix): base/ControlEditorBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ControlEditorBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ControlEditorBase.cpp$(DependSuffix) -MM base/ControlEditorBase.cpp

$(IntermediateDirectory)/base_ControlEditorBase.cpp$(PreprocessSuffix): base/ControlEditorBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ControlEditorBase.cpp$(PreprocessSuffix) base/ControlEditorBase.cpp

$(IntermediateDirectory)/ConstantForm.cpp$(ObjectSuffix): ConstantForm.cpp $(IntermediateDirectory)/ConstantForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ConstantForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConstantForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConstantForm.cpp$(DependSuffix): ConstantForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConstantForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConstantForm.cpp$(DependSuffix) -MM ConstantForm.cpp

$(IntermediateDirectory)/ConstantForm.cpp$(PreprocessSuffix): ConstantForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConstantForm.cpp$(PreprocessSuffix) ConstantForm.cpp

$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix): IndMotorForm.cpp $(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/IndMotorForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix): IndMotorForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IndMotorForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IndMotorForm.cpp$(DependSuffix) -MM IndMotorForm.cpp

$(IntermediateDirectory)/IndMotorForm.cpp$(PreprocessSuffix): IndMotorForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IndMotorForm.cpp$(PreprocessSuffix) IndMotorForm.cpp

$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix): ControlElement.cpp $(IntermediateDirectory)/ControlElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ControlElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlElement.cpp$(DependSuffix): ControlElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlElement.cpp$(DependSuffix) -MM ControlElement.cpp

$(IntermediateDirectory)/ControlElement.cpp$(PreprocessSuffix): ControlElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlElement.cpp$(PreprocessSuffix) ControlElement.cpp

$(IntermediateDirectory)/Gain.cpp$(ObjectSuffix): Gain.cpp $(IntermediateDirectory)/Gain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Gain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Gain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Gain.cpp$(DependSuffix): Gain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Gain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Gain.cpp$(DependSuffix) -MM Gain.cpp

$(IntermediateDirectory)/Gain.cpp$(PreprocessSuffix): Gain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Gain.cpp$(PreprocessSuffix) Gain.cpp

$(IntermediateDirectory)/DataReport.cpp$(ObjectSuffix): DataReport.cpp $(IntermediateDirectory)/DataReport.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/DataReport.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DataReport.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DataReport.cpp$(DependSuffix): DataReport.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DataReport.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DataReport.cpp$(DependSuffix) -MM DataReport.cpp

$(IntermediateDirectory)/DataReport.cpp$(PreprocessSuffix): DataReport.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DataReport.cpp$(PreprocessSuffix) DataReport.cpp

$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(ObjectSuffix): base/PropertiesFormBitmaps.cpp $(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/PropertiesFormBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(DependSuffix): base/PropertiesFormBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(DependSuffix) -MM base/PropertiesFormBitmaps.cpp

$(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(PreprocessSuffix): base/PropertiesFormBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_PropertiesFormBitmaps.cpp$(PreprocessSuffix) base/PropertiesFormBitmaps.cpp

$(IntermediateDirectory)/base_MainFrameBase.cpp$(ObjectSuffix): base/MainFrameBase.cpp $(IntermediateDirectory)/base_MainFrameBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/MainFrameBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_MainFrameBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_MainFrameBase.cpp$(DependSuffix): base/MainFrameBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_MainFrameBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_MainFrameBase.cpp$(DependSuffix) -MM base/MainFrameBase.cpp

$(IntermediateDirectory)/base_MainFrameBase.cpp$(PreprocessSuffix): base/MainFrameBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_MainFrameBase.cpp$(PreprocessSuffix) base/MainFrameBase.cpp

$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(ObjectSuffix): SimulationsSettingsForm.cpp $(IntermediateDirectory)/SimulationsSettingsForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SimulationsSettingsForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(DependSuffix): SimulationsSettingsForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(DependSuffix) -MM SimulationsSettingsForm.cpp

$(IntermediateDirectory)/SimulationsSettingsForm.cpp$(PreprocessSuffix): SimulationsSettingsForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SimulationsSettingsForm.cpp$(PreprocessSuffix) SimulationsSettingsForm.cpp

$(IntermediateDirectory)/MathExprParser.cpp$(ObjectSuffix): MathExprParser.cpp $(IntermediateDirectory)/MathExprParser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/MathExprParser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MathExprParser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MathExprParser.cpp$(DependSuffix): MathExprParser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MathExprParser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MathExprParser.cpp$(DependSuffix) -MM MathExprParser.cpp

$(IntermediateDirectory)/MathExprParser.cpp$(PreprocessSuffix): MathExprParser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MathExprParser.cpp$(PreprocessSuffix) MathExprParser.cpp

$(IntermediateDirectory)/Line.cpp$(ObjectSuffix): Line.cpp $(IntermediateDirectory)/Line.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Line.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Line.cpp$(DependSuffix): Line.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Line.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Line.cpp$(DependSuffix) -MM Line.cpp

$(IntermediateDirectory)/Line.cpp$(PreprocessSuffix): Line.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Line.cpp$(PreprocessSuffix) Line.cpp

$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix): Transformer.cpp $(IntermediateDirectory)/Transformer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Transformer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Transformer.cpp$(DependSuffix): Transformer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Transformer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Transformer.cpp$(DependSuffix) -MM Transformer.cpp

$(IntermediateDirectory)/Transformer.cpp$(PreprocessSuffix): Transformer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Transformer.cpp$(PreprocessSuffix) Transformer.cpp

$(IntermediateDirectory)/ControlSystemTest.cpp$(ObjectSuffix): ControlSystemTest.cpp $(IntermediateDirectory)/ControlSystemTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ControlSystemTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlSystemTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlSystemTest.cpp$(DependSuffix): ControlSystemTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlSystemTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlSystemTest.cpp$(DependSuffix) -MM ControlSystemTest.cpp

$(IntermediateDirectory)/ControlSystemTest.cpp$(PreprocessSuffix): ControlSystemTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlSystemTest.cpp$(PreprocessSuffix) ControlSystemTest.cpp

$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix): ControlEditor.cpp $(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ControlEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix): ControlEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlEditor.cpp$(DependSuffix) -MM ControlEditor.cpp

$(IntermediateDirectory)/ControlEditor.cpp$(PreprocessSuffix): ControlEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlEditor.cpp$(PreprocessSuffix) ControlEditor.cpp

$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix): SyncGenerator.cpp $(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SyncGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix): SyncGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncGenerator.cpp$(DependSuffix) -MM SyncGenerator.cpp

$(IntermediateDirectory)/SyncGenerator.cpp$(PreprocessSuffix): SyncGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncGenerator.cpp$(PreprocessSuffix) SyncGenerator.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(ObjectSuffix): base/MainFrameBitmaps.cpp $(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/MainFrameBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(DependSuffix): base/MainFrameBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(DependSuffix) -MM base/MainFrameBitmaps.cpp

$(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(PreprocessSuffix): base/MainFrameBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_MainFrameBitmaps.cpp$(PreprocessSuffix) base/MainFrameBitmaps.cpp

$(IntermediateDirectory)/base_DataReportBase.cpp$(ObjectSuffix): base/DataReportBase.cpp $(IntermediateDirectory)/base_DataReportBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/DataReportBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_DataReportBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_DataReportBase.cpp$(DependSuffix): base/DataReportBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_DataReportBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_DataReportBase.cpp$(DependSuffix) -MM base/DataReportBase.cpp

$(IntermediateDirectory)/base_DataReportBase.cpp$(PreprocessSuffix): base/DataReportBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_DataReportBase.cpp$(PreprocessSuffix) base/DataReportBase.cpp

$(IntermediateDirectory)/win_resources.rc$(ObjectSuffix): win_resources.rc
	$(RcCompilerName) -i "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/win_resources.rc" $(RcCmpOptions)   $(ObjectSwitch)$(IntermediateDirectory)/win_resources.rc$(ObjectSuffix) $(RcIncludePath)
$(IntermediateDirectory)/base_WorkspaceBase.cpp$(ObjectSuffix): base/WorkspaceBase.cpp $(IntermediateDirectory)/base_WorkspaceBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/WorkspaceBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_WorkspaceBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_WorkspaceBase.cpp$(DependSuffix): base/WorkspaceBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_WorkspaceBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_WorkspaceBase.cpp$(DependSuffix) -MM base/WorkspaceBase.cpp

$(IntermediateDirectory)/base_WorkspaceBase.cpp$(PreprocessSuffix): base/WorkspaceBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_WorkspaceBase.cpp$(PreprocessSuffix) base/WorkspaceBase.cpp

$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(ObjectSuffix): base/WorkspaceBitmaps.cpp $(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/WorkspaceBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(DependSuffix): base/WorkspaceBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(DependSuffix) -MM base/WorkspaceBitmaps.cpp

$(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(PreprocessSuffix): base/WorkspaceBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_WorkspaceBitmaps.cpp$(PreprocessSuffix) base/WorkspaceBitmaps.cpp

$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix): ConnectionLine.cpp $(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ConnectionLine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix): ConnectionLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConnectionLine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConnectionLine.cpp$(DependSuffix) -MM ConnectionLine.cpp

$(IntermediateDirectory)/ConnectionLine.cpp$(PreprocessSuffix): ConnectionLine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConnectionLine.cpp$(PreprocessSuffix) ConnectionLine.cpp

$(IntermediateDirectory)/IOControlForm.cpp$(ObjectSuffix): IOControlForm.cpp $(IntermediateDirectory)/IOControlForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/IOControlForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IOControlForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IOControlForm.cpp$(DependSuffix): IOControlForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IOControlForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IOControlForm.cpp$(DependSuffix) -MM IOControlForm.cpp

$(IntermediateDirectory)/IOControlForm.cpp$(PreprocessSuffix): IOControlForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IOControlForm.cpp$(PreprocessSuffix) IOControlForm.cpp

$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(ObjectSuffix): base/ControlEditorBitmaps.cpp $(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ControlEditorBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(DependSuffix): base/ControlEditorBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(DependSuffix) -MM base/ControlEditorBitmaps.cpp

$(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(PreprocessSuffix): base/ControlEditorBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ControlEditorBitmaps.cpp$(PreprocessSuffix) base/ControlEditorBitmaps.cpp

$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix): FileHanding.cpp $(IntermediateDirectory)/FileHanding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/FileHanding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/FileHanding.cpp$(DependSuffix): FileHanding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/FileHanding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/FileHanding.cpp$(DependSuffix) -MM FileHanding.cpp

$(IntermediateDirectory)/FileHanding.cpp$(PreprocessSuffix): FileHanding.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/FileHanding.cpp$(PreprocessSuffix) FileHanding.cpp

$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix): PowerFlow.cpp $(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/PowerFlow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix): PowerFlow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PowerFlow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PowerFlow.cpp$(DependSuffix) -MM PowerFlow.cpp

$(IntermediateDirectory)/PowerFlow.cpp$(PreprocessSuffix): PowerFlow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PowerFlow.cpp$(PreprocessSuffix) PowerFlow.cpp

$(IntermediateDirectory)/Element.cpp$(ObjectSuffix): Element.cpp $(IntermediateDirectory)/Element.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Element.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Element.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Element.cpp$(DependSuffix): Element.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Element.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Element.cpp$(DependSuffix) -MM Element.cpp

$(IntermediateDirectory)/Element.cpp$(PreprocessSuffix): Element.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Element.cpp$(PreprocessSuffix) Element.cpp

$(IntermediateDirectory)/RateLimiterForm.cpp$(ObjectSuffix): RateLimiterForm.cpp $(IntermediateDirectory)/RateLimiterForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/RateLimiterForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RateLimiterForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RateLimiterForm.cpp$(DependSuffix): RateLimiterForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RateLimiterForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RateLimiterForm.cpp$(DependSuffix) -MM RateLimiterForm.cpp

$(IntermediateDirectory)/RateLimiterForm.cpp$(PreprocessSuffix): RateLimiterForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RateLimiterForm.cpp$(PreprocessSuffix) RateLimiterForm.cpp

$(IntermediateDirectory)/Electromechanical.cpp$(ObjectSuffix): Electromechanical.cpp $(IntermediateDirectory)/Electromechanical.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Electromechanical.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Electromechanical.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Electromechanical.cpp$(DependSuffix): Electromechanical.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Electromechanical.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Electromechanical.cpp$(DependSuffix) -MM Electromechanical.cpp

$(IntermediateDirectory)/Electromechanical.cpp$(PreprocessSuffix): Electromechanical.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Electromechanical.cpp$(PreprocessSuffix) Electromechanical.cpp

$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(ObjectSuffix): base/PropertiesFormBase.cpp $(IntermediateDirectory)/base_PropertiesFormBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/PropertiesFormBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(DependSuffix): base/PropertiesFormBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(DependSuffix) -MM base/PropertiesFormBase.cpp

$(IntermediateDirectory)/base_PropertiesFormBase.cpp$(PreprocessSuffix): base/PropertiesFormBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_PropertiesFormBase.cpp$(PreprocessSuffix) base/PropertiesFormBase.cpp

$(IntermediateDirectory)/Multiplier.cpp$(ObjectSuffix): Multiplier.cpp $(IntermediateDirectory)/Multiplier.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Multiplier.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Multiplier.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Multiplier.cpp$(DependSuffix): Multiplier.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Multiplier.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Multiplier.cpp$(DependSuffix) -MM Multiplier.cpp

$(IntermediateDirectory)/Multiplier.cpp$(PreprocessSuffix): Multiplier.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Multiplier.cpp$(PreprocessSuffix) Multiplier.cpp

$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix): Shunt.cpp $(IntermediateDirectory)/Shunt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Shunt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Shunt.cpp$(DependSuffix): Shunt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Shunt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Shunt.cpp$(DependSuffix) -MM Shunt.cpp

$(IntermediateDirectory)/Shunt.cpp$(PreprocessSuffix): Shunt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Shunt.cpp$(PreprocessSuffix) Shunt.cpp

$(IntermediateDirectory)/ControlElementContainer.cpp$(ObjectSuffix): ControlElementContainer.cpp $(IntermediateDirectory)/ControlElementContainer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ControlElementContainer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlElementContainer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlElementContainer.cpp$(DependSuffix): ControlElementContainer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlElementContainer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlElementContainer.cpp$(DependSuffix) -MM ControlElementContainer.cpp

$(IntermediateDirectory)/ControlElementContainer.cpp$(PreprocessSuffix): ControlElementContainer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlElementContainer.cpp$(PreprocessSuffix) ControlElementContainer.cpp

$(IntermediateDirectory)/Exponential.cpp$(ObjectSuffix): Exponential.cpp $(IntermediateDirectory)/Exponential.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Exponential.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Exponential.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Exponential.cpp$(DependSuffix): Exponential.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Exponential.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Exponential.cpp$(DependSuffix) -MM Exponential.cpp

$(IntermediateDirectory)/Exponential.cpp$(PreprocessSuffix): Exponential.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Exponential.cpp$(PreprocessSuffix) Exponential.cpp

$(IntermediateDirectory)/Limiter.cpp$(ObjectSuffix): Limiter.cpp $(IntermediateDirectory)/Limiter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Limiter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Limiter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Limiter.cpp$(DependSuffix): Limiter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Limiter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Limiter.cpp$(DependSuffix) -MM Limiter.cpp

$(IntermediateDirectory)/Limiter.cpp$(PreprocessSuffix): Limiter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Limiter.cpp$(PreprocessSuffix) Limiter.cpp

$(IntermediateDirectory)/RateLimiter.cpp$(ObjectSuffix): RateLimiter.cpp $(IntermediateDirectory)/RateLimiter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/RateLimiter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RateLimiter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RateLimiter.cpp$(DependSuffix): RateLimiter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RateLimiter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RateLimiter.cpp$(DependSuffix) -MM RateLimiter.cpp

$(IntermediateDirectory)/RateLimiter.cpp$(PreprocessSuffix): RateLimiter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RateLimiter.cpp$(PreprocessSuffix) RateLimiter.cpp

$(IntermediateDirectory)/MathOperation.cpp$(ObjectSuffix): MathOperation.cpp $(IntermediateDirectory)/MathOperation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/MathOperation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MathOperation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MathOperation.cpp$(DependSuffix): MathOperation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MathOperation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MathOperation.cpp$(DependSuffix) -MM MathOperation.cpp

$(IntermediateDirectory)/MathOperation.cpp$(PreprocessSuffix): MathOperation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MathOperation.cpp$(PreprocessSuffix) MathOperation.cpp

$(IntermediateDirectory)/MathExpression.cpp$(ObjectSuffix): MathExpression.cpp $(IntermediateDirectory)/MathExpression.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/MathExpression.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MathExpression.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MathExpression.cpp$(DependSuffix): MathExpression.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MathExpression.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MathExpression.cpp$(DependSuffix) -MM MathExpression.cpp

$(IntermediateDirectory)/MathExpression.cpp$(PreprocessSuffix): MathExpression.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MathExpression.cpp$(PreprocessSuffix) MathExpression.cpp

$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(ObjectSuffix): artProvider/ArtMetro.cpp $(IntermediateDirectory)/artProvider_ArtMetro.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/artProvider/ArtMetro.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(DependSuffix): artProvider/ArtMetro.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(DependSuffix) -MM artProvider/ArtMetro.cpp

$(IntermediateDirectory)/artProvider_ArtMetro.cpp$(PreprocessSuffix): artProvider/ArtMetro.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/artProvider_ArtMetro.cpp$(PreprocessSuffix) artProvider/ArtMetro.cpp

$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix): Workspace.cpp $(IntermediateDirectory)/Workspace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Workspace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Workspace.cpp$(DependSuffix): Workspace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Workspace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Workspace.cpp$(DependSuffix) -MM Workspace.cpp

$(IntermediateDirectory)/Workspace.cpp$(PreprocessSuffix): Workspace.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Workspace.cpp$(PreprocessSuffix) Workspace.cpp

$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix): GraphicalElement.cpp $(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/GraphicalElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix): GraphicalElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GraphicalElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GraphicalElement.cpp$(DependSuffix) -MM GraphicalElement.cpp

$(IntermediateDirectory)/GraphicalElement.cpp$(PreprocessSuffix): GraphicalElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GraphicalElement.cpp$(PreprocessSuffix) GraphicalElement.cpp

$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(ObjectSuffix): base/DataReportBitmaps.cpp $(IntermediateDirectory)/base_DataReportBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/DataReportBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(DependSuffix): base/DataReportBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(DependSuffix) -MM base/DataReportBitmaps.cpp

$(IntermediateDirectory)/base_DataReportBitmaps.cpp$(PreprocessSuffix): base/DataReportBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_DataReportBitmaps.cpp$(PreprocessSuffix) base/DataReportBitmaps.cpp

$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix): TextForm.cpp $(IntermediateDirectory)/TextForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/TextForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TextForm.cpp$(DependSuffix): TextForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TextForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TextForm.cpp$(DependSuffix) -MM TextForm.cpp

$(IntermediateDirectory)/TextForm.cpp$(PreprocessSuffix): TextForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TextForm.cpp$(PreprocessSuffix) TextForm.cpp

$(IntermediateDirectory)/OpenGLText.cpp$(ObjectSuffix): OpenGLText.cpp $(IntermediateDirectory)/OpenGLText.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/OpenGLText.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/OpenGLText.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OpenGLText.cpp$(DependSuffix): OpenGLText.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OpenGLText.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/OpenGLText.cpp$(DependSuffix) -MM OpenGLText.cpp

$(IntermediateDirectory)/OpenGLText.cpp$(PreprocessSuffix): OpenGLText.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OpenGLText.cpp$(PreprocessSuffix) OpenGLText.cpp

$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix): IndMotor.cpp $(IntermediateDirectory)/IndMotor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/IndMotor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IndMotor.cpp$(DependSuffix): IndMotor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IndMotor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IndMotor.cpp$(DependSuffix) -MM IndMotor.cpp

$(IntermediateDirectory)/IndMotor.cpp$(PreprocessSuffix): IndMotor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IndMotor.cpp$(PreprocessSuffix) IndMotor.cpp

$(IntermediateDirectory)/Text.cpp$(ObjectSuffix): Text.cpp $(IntermediateDirectory)/Text.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Text.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Text.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Text.cpp$(DependSuffix): Text.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Text.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Text.cpp$(DependSuffix) -MM Text.cpp

$(IntermediateDirectory)/Text.cpp$(PreprocessSuffix): Text.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Text.cpp$(PreprocessSuffix) Text.cpp

$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix): Branch.cpp $(IntermediateDirectory)/Branch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Branch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Branch.cpp$(DependSuffix): Branch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Branch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Branch.cpp$(DependSuffix) -MM Branch.cpp

$(IntermediateDirectory)/Branch.cpp$(PreprocessSuffix): Branch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Branch.cpp$(PreprocessSuffix) Branch.cpp

$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix): TransferFunction.cpp $(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/TransferFunction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix): TransferFunction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransferFunction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransferFunction.cpp$(DependSuffix) -MM TransferFunction.cpp

$(IntermediateDirectory)/TransferFunction.cpp$(PreprocessSuffix): TransferFunction.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransferFunction.cpp$(PreprocessSuffix) TransferFunction.cpp

$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix): Bus.cpp $(IntermediateDirectory)/Bus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Bus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Bus.cpp$(DependSuffix): Bus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Bus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Bus.cpp$(DependSuffix) -MM Bus.cpp

$(IntermediateDirectory)/Bus.cpp$(PreprocessSuffix): Bus.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Bus.cpp$(PreprocessSuffix) Bus.cpp

$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix): Inductor.cpp $(IntermediateDirectory)/Inductor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Inductor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Inductor.cpp$(DependSuffix): Inductor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Inductor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Inductor.cpp$(DependSuffix) -MM Inductor.cpp

$(IntermediateDirectory)/Inductor.cpp$(PreprocessSuffix): Inductor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Inductor.cpp$(PreprocessSuffix) Inductor.cpp

$(IntermediateDirectory)/Load.cpp$(ObjectSuffix): Load.cpp $(IntermediateDirectory)/Load.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Load.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Load.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Load.cpp$(DependSuffix): Load.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Load.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Load.cpp$(DependSuffix) -MM Load.cpp

$(IntermediateDirectory)/Load.cpp$(PreprocessSuffix): Load.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Load.cpp$(PreprocessSuffix) Load.cpp

$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix): TransferFunctionForm.cpp $(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/TransferFunctionForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix): TransferFunctionForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TransferFunctionForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TransferFunctionForm.cpp$(DependSuffix) -MM TransferFunctionForm.cpp

$(IntermediateDirectory)/TransferFunctionForm.cpp$(PreprocessSuffix): TransferFunctionForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TransferFunctionForm.cpp$(PreprocessSuffix) TransferFunctionForm.cpp

$(IntermediateDirectory)/base_ElementFormBase.cpp$(ObjectSuffix): base/ElementFormBase.cpp $(IntermediateDirectory)/base_ElementFormBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ElementFormBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ElementFormBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ElementFormBase.cpp$(DependSuffix): base/ElementFormBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ElementFormBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ElementFormBase.cpp$(DependSuffix) -MM base/ElementFormBase.cpp

$(IntermediateDirectory)/base_ElementFormBase.cpp$(PreprocessSuffix): base/ElementFormBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ElementFormBase.cpp$(PreprocessSuffix) base/ElementFormBase.cpp

$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix): Machines.cpp $(IntermediateDirectory)/Machines.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Machines.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Machines.cpp$(DependSuffix): Machines.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Machines.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Machines.cpp$(DependSuffix) -MM Machines.cpp

$(IntermediateDirectory)/Machines.cpp$(PreprocessSuffix): Machines.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Machines.cpp$(PreprocessSuffix) Machines.cpp

$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix): PowerElement.cpp $(IntermediateDirectory)/PowerElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/PowerElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/PowerElement.cpp$(DependSuffix): PowerElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/PowerElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/PowerElement.cpp$(DependSuffix) -MM PowerElement.cpp

$(IntermediateDirectory)/PowerElement.cpp$(PreprocessSuffix): PowerElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/PowerElement.cpp$(PreprocessSuffix) PowerElement.cpp

$(IntermediateDirectory)/IOControl.cpp$(ObjectSuffix): IOControl.cpp $(IntermediateDirectory)/IOControl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/IOControl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IOControl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IOControl.cpp$(DependSuffix): IOControl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IOControl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IOControl.cpp$(DependSuffix) -MM IOControl.cpp

$(IntermediateDirectory)/IOControl.cpp$(PreprocessSuffix): IOControl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IOControl.cpp$(PreprocessSuffix) IOControl.cpp

$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix): SyncMotor.cpp $(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SyncMotor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix): SyncMotor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncMotor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncMotor.cpp$(DependSuffix) -MM SyncMotor.cpp

$(IntermediateDirectory)/SyncMotor.cpp$(PreprocessSuffix): SyncMotor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncMotor.cpp$(PreprocessSuffix) SyncMotor.cpp

$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(ObjectSuffix): base/ChartViewBitmaps.cpp $(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ChartViewBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(DependSuffix): base/ChartViewBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(DependSuffix) -MM base/ChartViewBitmaps.cpp

$(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(PreprocessSuffix): base/ChartViewBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ChartViewBitmaps.cpp$(PreprocessSuffix) base/ChartViewBitmaps.cpp

$(IntermediateDirectory)/ExponentialForm.cpp$(ObjectSuffix): ExponentialForm.cpp $(IntermediateDirectory)/ExponentialForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ExponentialForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ExponentialForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ExponentialForm.cpp$(DependSuffix): ExponentialForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ExponentialForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ExponentialForm.cpp$(DependSuffix) -MM ExponentialForm.cpp

$(IntermediateDirectory)/ExponentialForm.cpp$(PreprocessSuffix): ExponentialForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ExponentialForm.cpp$(PreprocessSuffix) ExponentialForm.cpp

$(IntermediateDirectory)/ImportForm.cpp$(ObjectSuffix): ImportForm.cpp $(IntermediateDirectory)/ImportForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ImportForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ImportForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ImportForm.cpp$(DependSuffix): ImportForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ImportForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ImportForm.cpp$(DependSuffix) -MM ImportForm.cpp

$(IntermediateDirectory)/ImportForm.cpp$(PreprocessSuffix): ImportForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ImportForm.cpp$(PreprocessSuffix) ImportForm.cpp

$(IntermediateDirectory)/AboutForm.cpp$(ObjectSuffix): AboutForm.cpp $(IntermediateDirectory)/AboutForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/AboutForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AboutForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AboutForm.cpp$(DependSuffix): AboutForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AboutForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AboutForm.cpp$(DependSuffix) -MM AboutForm.cpp

$(IntermediateDirectory)/AboutForm.cpp$(PreprocessSuffix): AboutForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AboutForm.cpp$(PreprocessSuffix) AboutForm.cpp

$(IntermediateDirectory)/ControlElementSolver.cpp$(ObjectSuffix): ControlElementSolver.cpp $(IntermediateDirectory)/ControlElementSolver.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ControlElementSolver.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ControlElementSolver.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ControlElementSolver.cpp$(DependSuffix): ControlElementSolver.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ControlElementSolver.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ControlElementSolver.cpp$(DependSuffix) -MM ControlElementSolver.cpp

$(IntermediateDirectory)/ControlElementSolver.cpp$(PreprocessSuffix): ControlElementSolver.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ControlElementSolver.cpp$(PreprocessSuffix) ControlElementSolver.cpp

$(IntermediateDirectory)/LimiterForm.cpp$(ObjectSuffix): LimiterForm.cpp $(IntermediateDirectory)/LimiterForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/LimiterForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LimiterForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LimiterForm.cpp$(DependSuffix): LimiterForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LimiterForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LimiterForm.cpp$(DependSuffix) -MM LimiterForm.cpp

$(IntermediateDirectory)/LimiterForm.cpp$(PreprocessSuffix): LimiterForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LimiterForm.cpp$(PreprocessSuffix) LimiterForm.cpp

$(IntermediateDirectory)/MathExpressionForm.cpp$(ObjectSuffix): MathExpressionForm.cpp $(IntermediateDirectory)/MathExpressionForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/MathExpressionForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MathExpressionForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MathExpressionForm.cpp$(DependSuffix): MathExpressionForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MathExpressionForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MathExpressionForm.cpp$(DependSuffix) -MM MathExpressionForm.cpp

$(IntermediateDirectory)/MathExpressionForm.cpp$(PreprocessSuffix): MathExpressionForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MathExpressionForm.cpp$(PreprocessSuffix) MathExpressionForm.cpp

$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix): BusForm.cpp $(IntermediateDirectory)/BusForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/BusForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BusForm.cpp$(DependSuffix): BusForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BusForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BusForm.cpp$(DependSuffix) -MM BusForm.cpp

$(IntermediateDirectory)/BusForm.cpp$(PreprocessSuffix): BusForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BusForm.cpp$(PreprocessSuffix) BusForm.cpp

$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix): GeneratorStabForm.cpp $(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/GeneratorStabForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix): GeneratorStabForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeneratorStabForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeneratorStabForm.cpp$(DependSuffix) -MM GeneratorStabForm.cpp

$(IntermediateDirectory)/GeneratorStabForm.cpp$(PreprocessSuffix): GeneratorStabForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeneratorStabForm.cpp$(PreprocessSuffix) GeneratorStabForm.cpp

$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix): LineForm.cpp $(IntermediateDirectory)/LineForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/LineForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LineForm.cpp$(DependSuffix): LineForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LineForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LineForm.cpp$(DependSuffix) -MM LineForm.cpp

$(IntermediateDirectory)/LineForm.cpp$(PreprocessSuffix): LineForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LineForm.cpp$(PreprocessSuffix) LineForm.cpp

$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix): Capacitor.cpp $(IntermediateDirectory)/Capacitor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Capacitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Capacitor.cpp$(DependSuffix): Capacitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Capacitor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Capacitor.cpp$(DependSuffix) -MM Capacitor.cpp

$(IntermediateDirectory)/Capacitor.cpp$(PreprocessSuffix): Capacitor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Capacitor.cpp$(PreprocessSuffix) Capacitor.cpp

$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix): LoadForm.cpp $(IntermediateDirectory)/LoadForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/LoadForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LoadForm.cpp$(DependSuffix): LoadForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LoadForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LoadForm.cpp$(DependSuffix) -MM LoadForm.cpp

$(IntermediateDirectory)/LoadForm.cpp$(PreprocessSuffix): LoadForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LoadForm.cpp$(PreprocessSuffix) LoadForm.cpp

$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix): ReactiveShuntElementForm.cpp $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/ReactiveShuntElementForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix): ReactiveShuntElementForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(DependSuffix) -MM ReactiveShuntElementForm.cpp

$(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(PreprocessSuffix): ReactiveShuntElementForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ReactiveShuntElementForm.cpp$(PreprocessSuffix) ReactiveShuntElementForm.cpp

$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix): SyncMachineForm.cpp $(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SyncMachineForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix): SyncMachineForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SyncMachineForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SyncMachineForm.cpp$(DependSuffix) -MM SyncMachineForm.cpp

$(IntermediateDirectory)/SyncMachineForm.cpp$(PreprocessSuffix): SyncMachineForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SyncMachineForm.cpp$(PreprocessSuffix) SyncMachineForm.cpp

$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(ObjectSuffix): base/ElementFormBitmaps.cpp $(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/base/ElementFormBitmaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(DependSuffix): base/ElementFormBitmaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(DependSuffix) -MM base/ElementFormBitmaps.cpp

$(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(PreprocessSuffix): base/ElementFormBitmaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/base_ElementFormBitmaps.cpp$(PreprocessSuffix) base/ElementFormBitmaps.cpp

$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(ObjectSuffix): GeneralPropertiesForm.cpp $(IntermediateDirectory)/GeneralPropertiesForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/GeneralPropertiesForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(DependSuffix): GeneralPropertiesForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(DependSuffix) -MM GeneralPropertiesForm.cpp

$(IntermediateDirectory)/GeneralPropertiesForm.cpp$(PreprocessSuffix): GeneralPropertiesForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GeneralPropertiesForm.cpp$(PreprocessSuffix) GeneralPropertiesForm.cpp

$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix): SwitchingForm.cpp $(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/SwitchingForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix): SwitchingForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SwitchingForm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SwitchingForm.cpp$(DependSuffix) -MM SwitchingForm.cpp

$(IntermediateDirectory)/SwitchingForm.cpp$(PreprocessSuffix): SwitchingForm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SwitchingForm.cpp$(PreprocessSuffix) SwitchingForm.cpp

$(IntermediateDirectory)/Sum.cpp$(ObjectSuffix): Sum.cpp $(IntermediateDirectory)/Sum.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/NDSE-69/Documents/GitHub/PSP/Project/Sum.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sum.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sum.cpp$(DependSuffix): Sum.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sum.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sum.cpp$(DependSuffix) -MM Sum.cpp

$(IntermediateDirectory)/Sum.cpp$(PreprocessSuffix): Sum.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sum.cpp$(PreprocessSuffix) Sum.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release_Windows_x64/


