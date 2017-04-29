#ifndef _CGPUPERFAPI_HPP_
#define _CGPUPERFAPI_HPP_

#include<GPUPerfAPITypes.h>
#include<GPUPerfAPI.h>

#include<iostream>

namespace NDormon{
  class CGpuPerfApi
  {
    private:
      void*Context;
      gpa_uint32 CurrentWaitSessionID;
      gpa_uint32 SessionID;
    public:
      CGpuPerfApi(void*Context);
      ~CGpuPerfApi();
      void EnableAll();
      void DisableAll();
      void EnableTiming();
      void EnableVertexShader();
      void EnableControlShader();
      void EnableEvaluationShader();
      void EnableGeometryShader();
      void EnablePrimitiveAssembly();
      void EnableFragmentShader();
      void EnableTextureUnit();
      void EnableTextureFormat();
      void EnableGeneral();
      void EnableComputeShader();
      void EnableDepthAndStencil();
      void EnableColorBuffer();
      void EnableGlobalMemory();
      void EnableLocalMemory();
      void EnableCounters(const char*Counters[],unsigned NumCounters);
      void Enable(const char*Counter);
      void Disable(const char*Counter);
      gpa_uint32 GetNumPasses();
      void WriteSession(std::string File);
      void WriteHead(std::string File);
      std::string GetResults();
      void BeginSession();
      void EndSession();
      void BeginPass();
      void EndPass();
      void BeginSample(gpa_uint32 Id);
      void EndSample();

  };
}

#endif//_CGPUPERFAPI_HPP_
