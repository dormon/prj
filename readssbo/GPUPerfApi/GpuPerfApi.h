#pragma once

#include<GPUPerfAPITypes.h>
#include<GPUPerfAPI.h>

#include<iostream>

namespace NDormon{
  class GpuPerfApi{
    private:
      void*      _context = nullptr;
      gpa_uint32 _currentWaitSessionID;
      gpa_uint32 _sessionID;
      void _enableByName(const char*group);
    public:
      GpuPerfApi(void*context);
      ~GpuPerfApi();
      void enableAll();
      void disableAll();
      void enableTiming();
      void enableVertexShader();
      void enableControlShader();
      void enableEvaluationShader();
      void enableGeometryShader();
      void enablePrimitiveAssembly();
      void enableFragmentShader();
      void enableTextureUnit();
      void enableTextureFormat();
      void enableGeneral();
      void enableComputeShader();
      void enableDepthAndStencil();
      void enableColorBuffer();
      void enableGlobalMemory();
      void enableLocalMemory();
      void enableCounters(const char*counters[],unsigned nofCounters);
      void enable(const char*counter);
      void disable(const char*counter);
      gpa_uint32 getNumPasses();
      void writeSession(std::string file);
      void writeHead(std::string file);
      std::string getResults();
      void beginSession();
      void endSession();
      void beginPass();
      void endPass();
      void beginSample(gpa_uint32 id);
      void endSample();
  };
}

