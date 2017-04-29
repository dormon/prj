#include"CGpuPerfApi.hpp"

#include<iostream>
#include<fstream>
#include<sstream>

#include<stdlib.h>

#include<GPUPerfAPITypes.h>
#include<GPUPerfAPI.h>

namespace NDormon{
  std::string GPUPerfApiTranslateUsage(GPA_Usage_Type Type){
  	std::stringstream Result;
    switch(Type){
      case GPA_USAGE_TYPE_RATIO:
        return"ratio";
      case GPA_USAGE_TYPE_PERCENTAGE:
        return"%";
      case GPA_USAGE_TYPE_CYCLES:
        return"cycles";
      case GPA_USAGE_TYPE_MILLISECONDS:
        return"ms";
      case GPA_USAGE_TYPE_BYTES:
        return"byte";
      case GPA_USAGE_TYPE_ITEMS:
        return"items";
      case GPA_USAGE_TYPE_KILOBYTES:
        return"kb";
      case GPA_USAGE_TYPE__LAST:
        return"last";
      default:
      Result<<"unknown error: "<<Type;
        return Result.str();
    }
  }
  std::string GPUPerfApiTranslateStatus(GPA_Status Status){
  	std::stringstream Result;
    switch(Status){
      case GPA_STATUS_OK:
        return"GPA_STATUS_OK";
      case GPA_STATUS_ERROR_NULL_POINTER:
        return"GPA_STATUS_ERROR_NULL_POINTER";
      case GPA_STATUS_ERROR_COUNTERS_NOT_OPEN:
        return"GPA_STATUS_ERROR_COUNTERS_NOT_OPEN";
      case GPA_STATUS_ERROR_COUNTERS_ALREADY_OPEN:
        return"GPA_STATUS_ERROR_COUNTERS_ALREADY_OPEN";
      case GPA_STATUS_ERROR_INDEX_OUT_OF_RANGE:
        return"GPA_STATUS_ERROR_INDEX_OUT_OF_RANGE";
      case GPA_STATUS_ERROR_NOT_FOUND:
        return"GPA_STATUS_ERROR_NOT_FOUND";
      case GPA_STATUS_ERROR_ALREADY_ENABLED:
        return"GPA_STATUS_ERROR_ALREADY_ENABLED";
      case GPA_STATUS_ERROR_NO_COUNTERS_ENABLED:
        return"GPA_STATUS_ERROR_NO_COUNTERS_ENABLED";
      case GPA_STATUS_ERROR_NOT_ENABLED:
        return"GPA_STATUS_ERROR_NOT_ENABLED";
      case GPA_STATUS_ERROR_SAMPLING_NOT_STARTED:
        return"GPA_STATUS_ERROR_SAMPLING_NOT_STARTED";
      case GPA_STATUS_ERROR_SAMPLING_ALREADY_STARTED:
        return"GPA_STATUS_ERROR_SAMPLING_ALREADY_STARTED";
      case GPA_STATUS_ERROR_SAMPLING_NOT_ENDED:
        return"GPA_STATUS_ERROR_SAMPLING_NOT_ENDED";
      case GPA_STATUS_ERROR_NOT_ENOUGH_PASSES:
        return"GPA_STATUS_ERROR_NOT_ENOUGH_PASSES";
      case GPA_STATUS_ERROR_PASS_NOT_ENDED:
        return"GPA_STATUS_ERROR_PASS_NOT_ENDED";
      case GPA_STATUS_ERROR_PASS_NOT_STARTED:
        return"GPA_STATUS_ERROR_PASS_NOT_STARTED";
      case GPA_STATUS_ERROR_PASS_ALREADY_STARTED:
        return"GPA_STATUS_ERROR_PASS_ALREADY_STARTED";
      case GPA_STATUS_ERROR_SAMPLE_NOT_STARTED:
        return"GPA_STATUS_ERROR_SAMPLE_NOT_STARTED";
      case GPA_STATUS_ERROR_SAMPLE_ALREADY_STARTED:
        return"GPA_STATUS_ERROR_SAMPLE_ALREADY_STARTED";
      case GPA_STATUS_ERROR_SAMPLE_NOT_ENDED:
        return"GPA_STATUS_ERROR_SAMPLE_NOT_ENDED";
      case GPA_STATUS_ERROR_CANNOT_CHANGE_COUNTERS_WHEN_SAMPLING:
        return"GPA_STATUS_ERROR_CANNOT_CHANGE_COUNTERS_WHEN_SAMPLING";
      case GPA_STATUS_ERROR_SESSION_NOT_FOUND:
        return"GPA_STATUS_ERROR_SESSION_NOT_FOUND";
      case GPA_STATUS_ERROR_SAMPLE_NOT_FOUND:
        return"GPA_STATUS_ERROR_SAMPLE_NOT_FOUND";
      case GPA_STATUS_ERROR_SAMPLE_NOT_FOUND_IN_ALL_PASSES:
        return"GPA_STATUS_ERROR_SAMPLE_NOT_FOUND_IN_ALL_PASSES";
      case GPA_STATUS_ERROR_COUNTER_NOT_OF_SPECIFIED_TYPE:
        return"GPA_STATUS_ERROR_COUNTER_NOT_OF_SPECIFIED_TYPE";
      case GPA_STATUS_ERROR_READING_COUNTER_RESULT:
        return"GPA_STATUS_ERROR_READING_COUNTER_RESULT";
      case GPA_STATUS_ERROR_VARIABLE_NUMBER_OF_SAMPLES_IN_PASSES:
        return"GPA_STATUS_ERROR_VARIABLE_NUMBER_OF_SAMPLES_IN_PASSES";
      case GPA_STATUS_ERROR_FAILED:
        return"GPA_STATUS_ERROR_FAILED";
      case GPA_STATUS_ERROR_HARDWARE_NOT_SUPPORTED:
        return"GPA_STATUS_ERROR_HARDWARE_NOT_SUPPORTED";
      case GPA_STATUS_ERROR_CATALYST_VER_UNSUPPORTED_SI:
        return"GPA_STATUS_ERROR_CATALYST_VER_UNSUPPORTED_SI";
      default:
        Result<<"unknown error: "<<Status;
        return Result.str();
    }
  }
  void MyLoggingFunction( GPA_Logging_Type messageType, const char*
      message ){
    std::cerr<<message<<std::endl;
  }

  CGpuPerfApi::CGpuPerfApi(void*Context){
    this->Context=Context;//set context
    this->CurrentWaitSessionID=1;//set current sesstion
    GPA_Status Status=GPA_Initialize();//initialize GPA
    if(Status!=GPA_STATUS_OK)throw std::string("GPA_Initialize has failed");
    Status=GPA_OpenContext(this->Context);//open context
    if(Status!=GPA_STATUS_OK)
      throw"GPA_OpenContext: "+GPUPerfApiTranslateStatus(Status);
    /*Status=GPA_RegisterLoggingCallback( GPA_LOGGING_ALL,
    MyLoggingFunction);*/

  }
  CGpuPerfApi::~CGpuPerfApi(){
    this->DisableAll();
    GPA_Status Status=GPA_STATUS_OK;
    if((Status=GPA_CloseContext())!=GPA_STATUS_OK)
      throw "GPA_CloseContext: "+GPUPerfApiTranslateStatus(Status);
    if((Status=GPA_Destroy())!=GPA_STATUS_OK)
      throw "GPA_Destroy: "+GPUPerfApiTranslateStatus(Status);
  }
  void CGpuPerfApi::EnableAll(){
    GPA_EnableAllCounters();
  }
  void CGpuPerfApi::DisableAll(){
    GPA_DisableAllCounters();
  }

  void CGpuPerfApi::EnableCounters(const char*Counters[],unsigned NumCounters){
    for(unsigned c=0;c<NumCounters;++c){
      GPA_Status Status=
        GPA_EnableCounterStr(Counters[c]);
      if(Status!=GPA_STATUS_OK)std::cerr<<Counters[c]<<std::endl;
    }
  }

  void CGpuPerfApi::EnableTiming(){
    const char*TimingCounters[]={
      /*"DepthStencilTestBusy",
      "GPUTime",
      "GPUBusy",
      "InterpBusy",*/
      "VSBusy",
      "HSBusy",
      "DSBusy",
      "GSBusy",
      "PrimitiveAssemblyBusy",
      "PSBusy",
      "CSBusy"/*,
      "ShaderBusy",
      "ShaderBusyCS",
      "ShaderBusyDS",
      "ShaderBusyGS",
      "ShaderBusyHS",
      "ShaderBusyPS",
      "ShaderBusyVS",
      "TessellatorBusy",
      "TexUnitBusy"*/
    };
    this->EnableCounters(TimingCounters,sizeof(TimingCounters)/sizeof(const char*));
  }

  void CGpuPerfApi::EnableVertexShader(){
    const char*VertexShaderCounters[]={
      /*"VertexMemFetched",
      "VertexMemFetchedCost",
      "VSALUBusy",
      "VSALUEfficiency",
      "VSALUInstCount",
      "VSALUTexRatio",
      "VSSALUBusy",
      "VSSALUInstCount",
      "VSTexBusy",
      "VSTexInstCount",
      "VSVALUBusy",*/
      "VSVALUInstCount",
      "VSVerticesIn"
    };
    this->EnableCounters(VertexShaderCounters,sizeof(VertexShaderCounters)/sizeof(const char*));
  }

  void CGpuPerfApi::EnableControlShader(){
    const char*ControlShaderCounters[]={
      "HSALUBusy",
      "HSALUEfficiency",
      "HSALUInstCount",
      "HSALUTexRatio",
      "HSTexBusy",
      "HSTexInstCount",
      "HSPatches",
      "HSSALUBusy",
      "HSSALUInstCount",
      "HSVALUBusy",
      "HSVALUInstCount"
    };
    this->EnableCounters(ControlShaderCounters,sizeof(ControlShaderCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableEvaluationShader(){
    const char*EvaluationShaderCounters[]={
      "DSALUBusy",
      "DSALUEfficiency",
      "DSALUInstCount",
      "DSALUTexRatio",
      "DSTexBusy",
      "DSTexInstCount",
      "DSVerticesIn"
    };
    this->EnableCounters(EvaluationShaderCounters,sizeof(EvaluationShaderCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableGeometryShader(){
    const char*GeometryShaderCounters[]={
      "GSALUBusy",
      "GSALUEfficiency",
      "GSALUInstCount",
      "GSALUTexRatio",
      "GSExportPct",
      "GSPrimsIn",
      "GSSALUBusy",
      "GSSALUInstCount",
      "GSTexBusy",
      "GSTexInstCount",
      "GSVALUBusy",
      "GSVALUInstCount",
      "GSVerticesOut"
    };
    this->EnableCounters(GeometryShaderCounters,sizeof(GeometryShaderCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnablePrimitiveAssembly(){
    const char*PrimitiveAssemblyCounters[]={
      "ClippedPrims",
      "CulledPrims",
      "PAPixelsPerTriangle",
      "PAStalledOnRasterizer",
      "PrimitivesIn"
    };
    this->EnableCounters(PrimitiveAssemblyCounters,sizeof(PrimitiveAssemblyCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableFragmentShader(){
    const char*FragmentShaderCounters[]={
      /*"PSALUBusy",
      "PSALUEfficiency",
      "PSALUInstCount",
      "PSALUTexRatio",
      "PSExportStalls",*/
      "PSPixelsIn",
      "PSPixelsOut"/*,
      "PSSALUBusy",
      "PSSALUInstCount",
      "PSTexBusy",
      "PSTexInstCount",
      "PSVALUBusy",
      "PSVALUInstCount"*/
    };
    this->EnableCounters(FragmentShaderCounters,sizeof(FragmentShaderCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableTextureUnit(){
    const char*TextureUnitCounters[]={
      "TexAveAnisotropy",
      "TexCacheStalled",
      "TexCostOfFiltering",
      "TexelFetchCount",
      "TexMemBytesRead",
      "TexMissRate",
      "TexTriFilteringPct",
      "TexVolFilteringPct"
    };
    this->EnableCounters(TextureUnitCounters,sizeof(TextureUnitCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableTextureFormat(){
    const char*TextureFormatCounters[]={
      "Pct64SlowTexels",
      "Pct128SlowTexels",
      "PctCompressedTexels",
      "PctDepthTexels",
      "PctInterlacedTexels",
      "PctTex1D",
      "PctTex1Darray",
      "PctTex2D",
      "PctTex2Darray",
      "PctTex2DMSAA",
      "PctTex2DMSAAArray",
      "PctTex3D",
      "PctTexCube",
      "PctTexCubeArray",
      "PctUncompressedTexels",
      "PctVertex64SlowTexels",
      "PctVertex128SlowTexels",
      "PctVertexTexels"
    };
    this->EnableCounters(TextureFormatCounters,sizeof(TextureFormatCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableGeneral(){
    const char*GeneralCounters[]={
      "ALUBusy",
      "ALUFetchRatio",
      "ALUInsts",
      "ALUPacking",
      "FetchInsts",
      "GDSInsts",
      "SALUBusy",
      "SALUInsts",
      "SFetchInsts",
      "VALUBusy",
      "VALUInsts",
      "VALUUtilization",
      "VFetchInsts",
      "VWriteInsts",
      "Wavefronts",
      "WriteInsts"
    };
    this->EnableCounters(GeneralCounters,sizeof(GeneralCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableComputeShader(){
    const char*ComputeShaderCounters[]={
      "CSALUBusy",
      "CSALUFetchRatio",
      "CSALUInsts",
      "CSALUPacking",
      "CSALUStalledByLDS",
      "CSCacheHit",
      "CSCompletePath",
      "CSFastPath",
      "CSFetchInsts",
      "CSFetchSize",
      "CSGDSInsts",
      "CSLDSBankConflict",
      "CSLDSFetchInsts",
      "CSLDSWriteInsts",
      "CSMemUnitBusy",
      "CSMemUnitStalled",
      "CSPathUtilization",
      "CSSALUBusy",
      "CSSALUInsts",
      "CSSFetchInsts",
      "CSFetchSize",
      "CSGDSInsts",
      "CSTexBusy",
      "CSThreadGroups",
      "CSThreads",
      "CSVALUBusy",
      "CSVALUInsts",
      "CSVALUUtilization",
      "CSVFetchInsts",
      "CSVWriteInsts",
      "CSWavefronts",
      "CSWriteInsts",
      "CSWriteSize",
      "CSWriteUnitStalled"
    };
    this->EnableCounters(ComputeShaderCounters,sizeof(ComputeShaderCounters)/sizeof(const char*));
  }

  void CGpuPerfApi::EnableDepthAndStencil(){
    const char*DepthAndStencilCounters[]={
      "HiZQuadsCulled",
      "HiZTilesAccepted",
      "PostZQuads",
      "PostZSamplesFailingS",
      "PostZSamplesFailingZ",
      "PostZSamplesPassing",
      "PreZQuadsCulled",
      "PreZSamplesFailingS",
      "PreZSamplesFailingZ",
      "PreZSamplesPassing",
      "PreZTilesDetailCulled",
      "ZUnitStalled"
    };
    this->EnableCounters(DepthAndStencilCounters,sizeof(DepthAndStencilCounters)/sizeof(const char*));
  }

  void CGpuPerfApi::EnableColorBuffer(){
    const char*ColorBufferCounters[]={
      "CBMemRead",
      "CBMemWritten",
      "CBSlowPixelPct"
    };
    this->EnableCounters(ColorBufferCounters,sizeof(ColorBufferCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::EnableGlobalMemory(){
    const char*GlobalMemoryCounters[]={
      "CompletePath",
      "FastPath",
      "FetchSize",
      "FetchUnitBusy",
      "FetchUnitStalled",
      "CacheHit",
      "MemUnitBusy",
      "MemUnitStalled",
      "PathUtilization",
      "WriteSize",
      "WriteUnitStalled"
    };
    this->EnableCounters(GlobalMemoryCounters,sizeof(GlobalMemoryCounters)/sizeof(const char*));
  }

  void CGpuPerfApi::EnableLocalMemory(){
    const char*LocalMemoryCounters[]={
      "LDSBankConflict",
      "LDSFetchInsts",
      "LDSInsts",
      "LDSWriteInsts"
    };
    this->EnableCounters(LocalMemoryCounters,sizeof(LocalMemoryCounters)/sizeof(const char*));
  }
  void CGpuPerfApi::Enable(const char*Counter){
    GPA_EnableCounterStr(Counter);
  }
  void CGpuPerfApi::Disable(const char*Counter){
    GPA_DisableCounterStr(Counter);
  }
  unsigned CGpuPerfApi::GetNumPasses(){
    gpa_uint32 NumPasses;
    GPA_GetPassCount(&NumPasses);
    return NumPasses;
  }
  void CGpuPerfApi::WriteHead(std::string File){
    gpa_uint32 count;//number of enable counters
    GPA_GetEnabledCount(&count);//obtain number of enabled counters
    std::ofstream f;//output file handle
    const char*name;
    f.open(File.c_str());//open file
    f<<"Identifier, ";
    //std::cerr<<"Identifier, ";
    for(gpa_uint32 counter=0;counter<count;counter++){//loop over counters
      gpa_uint32 enabledCounterIndex;//enabled counter
      GPA_GetEnabledIndex(counter,&enabledCounterIndex);//get counter id
      GPA_GetCounterName(enabledCounterIndex,&name);//get counter name
      f<<name<<", ";//write counter's name
      std::cerr<<name<<", ";
    }
    f<<std::endl;
    f.close();
  }
  void CGpuPerfApi::WriteSession(std::string File){
    bool readyResult=false;
    //std::cerr<<"SessionID: "<<this->SessionID<<"CUrrentWaitSessionID: "<<this->CurrentWaitSessionID<<std::endl;
    if(this->SessionID!=this->CurrentWaitSessionID){
      //std::cerr<<"this->SessionID!=this->CurrentWaitSessionID"<<std::endl;
      //std::cerr<<"SessionID: "<<this->SessionID<<"CUrrentWaitSessionID: "<<this->CurrentWaitSessionID<<std::endl;
      GPA_Status sessionStatus;
      sessionStatus=GPA_IsSessionReady(&readyResult,
          this->CurrentWaitSessionID);
      while(sessionStatus==GPA_STATUS_ERROR_SESSION_NOT_FOUND){//skip
        //std::cerr<<"while(sessionStatus==GPA_STATUS_ERROR_SESSION_NOT_FOUND)"<<std::endl;
        //std::cerr<<"SessionID: "<<this->SessionID<<"CUrrentWaitSessionID: "<<this->CurrentWaitSessionID<<std::endl;
        this->CurrentWaitSessionID++;
          sessionStatus=GPA_IsSessionReady(&readyResult,
            this->CurrentWaitSessionID);
        //std::cerr<<GPUPerfApiTranslateStatus(sessionStatus)<<std::endl;
        //std::cerr<<"GPA_IsSessionReady: "<<readyResult<<std::endl;
      }


    }
    //std::cerr<<"Docyklil";
    if(!readyResult)return;
    //std::cerr<<"if(!readyResult)"<<std::endl;
    gpa_uint32 count;//number of enable counters
    GPA_GetEnabledCount(&count);//obtain number of enabled counters
    //std::cerr<<"GPA_GetEnabledCount: "<<count<<std::endl;
    std::ofstream f;//output file handle
    f.open(File.c_str(),std::ios_base::app);
    gpa_uint32 sampleCount;//number of samples
    GPA_GetSampleCount(this->CurrentWaitSessionID,&sampleCount);//number of samples
    //std::cerr<<"sampleCount: "<<sampleCount<<std::endl;
    for(gpa_uint32 sample=0;sample<sampleCount;sample++){//loop over samples
      for(gpa_uint32 counter=0;counter<count;counter++){//loop over counters
        gpa_uint32 enabledCounterIndex;//enable counter id
        GPA_GetEnabledIndex(counter,&enabledCounterIndex);//get enabled id

        const char*name;
        GPA_GetCounterName(enabledCounterIndex,&name);//get counter name
        std::cerr<<name<<": ";

        GPA_Type type;//type of counter
        GPA_GetCounterDataType(enabledCounterIndex,&type);//get data type
        if(type==GPA_TYPE_UINT32){//32bit uint
          gpa_uint32 value;//32bit uint value
          GPA_GetSampleUInt32(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          std::cerr<<value;
          f<<value<<",";
        }else if(type==GPA_TYPE_UINT64){
          gpa_uint64 value;//64bit uint
          GPA_GetSampleUInt64(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          std::cerr<<value;
          f<<value<<",";
        }else if(type==GPA_TYPE_FLOAT32){
          gpa_float32 value;//float
          GPA_GetSampleFloat32(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          std::cerr<<value;
          f<<value<<",";
        }else if(type==GPA_TYPE_FLOAT64){
          gpa_float64 value;//double
          GPA_GetSampleFloat64(this->CurrentWaitSessionID,
              sample,enabledCounterIndex,&value);
          std::cerr<<value;
          f<<value<<",";
        }else{
          std::cerr<<"nic\n";
        }
        GPA_Usage_Type UsageType;
        GPA_GetCounterUsageType( enabledCounterIndex,&UsageType);
        std::cerr<<GPUPerfApiTranslateUsage(UsageType)<<std::endl;
      }
      f<<std::endl;
      std::cerr<<std::endl;
    }
    f.close();
  }
  std::string CGpuPerfApi::GetResults(){
    std::stringstream Result;
    bool readyResult=false;
    if(this->SessionID!=this->CurrentWaitSessionID){
      GPA_Status sessionStatus;
      sessionStatus=GPA_IsSessionReady(&readyResult,
          this->CurrentWaitSessionID);
      while(sessionStatus==GPA_STATUS_ERROR_SESSION_NOT_FOUND){//skip
        this->CurrentWaitSessionID++;
          sessionStatus=GPA_IsSessionReady(&readyResult,
            this->CurrentWaitSessionID);
      }
    }
    //std::cerr<<"Docyklil";
    if(!readyResult)return Result.str();
    gpa_uint32 count;//number of enable counters
    GPA_GetEnabledCount(&count);//obtain number of enabled counters
    gpa_uint32 sampleCount;//number of samples
    GPA_GetSampleCount(this->CurrentWaitSessionID,&sampleCount);//number of samples
    for(gpa_uint32 sample=0;sample<sampleCount;sample++){//loop over samples
      for(gpa_uint32 counter=0;counter<count;counter++){//loop over counters
        gpa_uint32 enabledCounterIndex;//enable counter id
        GPA_GetEnabledIndex(counter,&enabledCounterIndex);//get enabled id

        const char*name;
        GPA_GetCounterName(enabledCounterIndex,&name);//get counter name
        Result<<name<<": ";

        GPA_Type type;//type of counter
        GPA_GetCounterDataType(enabledCounterIndex,&type);//get data type
        if(type==GPA_TYPE_UINT32){//32bit uint
          gpa_uint32 value;//32bit uint value
          GPA_GetSampleUInt32(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          Result<<value;
        }else if(type==GPA_TYPE_UINT64){
          gpa_uint64 value;//64bit uint
          GPA_GetSampleUInt64(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          Result<<value;
        }else if(type==GPA_TYPE_FLOAT32){
          gpa_float32 value;//float
          GPA_GetSampleFloat32(this->CurrentWaitSessionID,//get data
              sample,enabledCounterIndex,&value);
          Result<<value;
        }else if(type==GPA_TYPE_FLOAT64){
          gpa_float64 value;//double
          GPA_GetSampleFloat64(this->CurrentWaitSessionID,
              sample,enabledCounterIndex,&value);
          Result<<value;
        }else{
        }
        GPA_Usage_Type UsageType;
        GPA_GetCounterUsageType( enabledCounterIndex,&UsageType);
        Result<<" "<<GPUPerfApiTranslateUsage(UsageType)<<std::endl;
      }
    }
    return Result.str();
  }

  void CGpuPerfApi::BeginSession(){
    //std::cerr<<"GPA_BeginSession: "<<GPUPerfApiTranslateStatus(GPA_BeginSession(&this->SessionID))<<std::endl;
    GPA_BeginSession(&this->SessionID);
  }
  void CGpuPerfApi::EndSession(){
    //std::cerr<<"GPA_EndSession: "<<GPUPerfApiTranslateStatus(GPA_EndSession())<<std::endl;
    GPA_EndSession();
  }
  void CGpuPerfApi::BeginPass(){
    //std::cerr<<"GPA_BeginPass: "<<GPUPerfApiTranslateStatus(GPA_BeginPass())<<std::endl;
    GPA_BeginPass();
  }
  void CGpuPerfApi::EndPass(){
    //std::cerr<<"GPA_EndPass: "<<GPUPerfApiTranslateStatus(GPA_EndPass())<<std::endl;
    GPA_EndPass();
  }
  void CGpuPerfApi::BeginSample(unsigned Id){
    //std::cerr<<"GPA_BeginSample: "<<GPUPerfApiTranslateStatus(GPA_BeginSample(Id))<<std::endl;
    GPA_BeginSample(Id);
  }
  void CGpuPerfApi::EndSample(){
    //std::cerr<<"GPA_EndSample: "<<GPUPerfApiTranslateStatus(GPA_EndSample())<<std::endl;
    GPA_EndSample();
  }
}



