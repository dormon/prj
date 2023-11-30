#include<spirv/1.0/spirv.h>
#include<stdio.h>
#include<cstdint>

int main(int argc,char*argv[]){
  uint32_t const code[]={
    SpvMagicNumber,
    SpvVersion,
    0xDE9E,
    27,
    0,

    SpvOpCapability | (2u<<16u), // 0x00020011,
    SpvCapabilityShader,

    SpvOpExtInstImport | (6u<<16u), //0x06000B,
    1u, //%1
    'G'<<0u|'L'<<8u|'S'<<16u|'L'<<24u,
    '.'<<0u|'s'<<8u|'t'<<16u|'d'<<24u,
    '.'<<0u|'4'<<8u|'5'<<16u|'0'<<24u,
    0u,

    SpvOpMemoryModel | (3u<<16u), //0x0003000E
    SpvAddressingModelLogical,
    SpvMemoryModelGLSL450,

    SpvOpEntryPoint | (6u<<16u), //0x0006000F,
    SpvExecutionModelGLCompute,
    4u, // %main
    'm'<<0u|'a'<<8u|'i'<<16u|'n'<<24u,
    0u, 
    15u, // %gl_GlobalInvocationID

    SpvOpExecutionMode | (6u<<16u),
    4u, // %main
    SpvExecutionModeLocalSize,
    64u,
    1u,
    1u,

    SpvOpSource | (3u<<16u),
    SpvSourceLanguageGLSL,
    460u,

    SpvOpName | (4u<<16u),
    4u, // %main
    'm'<<0u|'a'<<8u|'i'<<16u|'n'<<24u,
    0u,

    SpvOpName | (4u<<16u),
    8u, // %Data
    'D'<<0u|'a'<<8u|'t'<<16u|'a'<<24u,
    0u,

    SpvOpMemberName | (5u<<16u),
    8u, // %Data
    0u,
    'd'<<0u|'a'<<8u|'t'<<16u|'a'<<24u,
    0u,

    SpvOpName | (3u<<16u),
    10u, // %_
    0u,

    SpvOpName | (8u<<16u),
    15u, // %gl_GlobalInvocationID
    'g'<<0u|'l' <<8u|'_' <<16u|'G' <<24u,
    'l'<<0u|'o' <<8u|'b' <<16u|'a' <<24u,
    'l'<<0u|'I' <<8u|'n' <<16u|'v' <<24u,
    'o'<<0u|'c' <<8u|'a' <<16u|'t' <<24u,
    'i'<<0u|'o' <<8u|'n' <<16u|'I' <<24u,
    'D'<<0u|'\0'<<8u|'\0'<<16u|'\0'<<24u,

    SpvOpDecorate | (4u<<16u),
    7u, // % _runtimearr_uint
    SpvDecorationArrayStride,
    4u,

    SpvOpMemberDecorate | (5u<<16u),
    8u, // %Data
    0u,
    SpvDecorationOffset,
    0u,

    SpvOpDecorate | (3u<<16u),
    8u, // %Data
    SpvDecorationBufferBlock,

    SpvOpDecorate | (4u<<16u),
    10u, // %_
    SpvDecorationDescriptorSet,
    0u,

    SpvOpDecorate | (4u<<16u),
    10u, // %_
    SpvDecorationBinding,
    0u,

    SpvOpDecorate | (4u<<16u),
    15u, // %gl_GlobalInvocationID
    SpvDecorationBuiltIn,
    SpvBuiltInGlobalInvocationId,

    SpvOpDecorate | (4u<<16u),
    26u, // %gl_WorkGroupSize
    SpvDecorationBuiltIn,
    SpvBuiltInWorkgroupSize,

    SpvOpTypeVoid | (2u<<16u),
    2u, // %void

    SpvOpTypeFunction | (3u<<16u),
    3u, // %3
    2u, // %void

    SpvOpTypeInt | (4u<<16u),
    6u, // %uint
    32u,
    0u,

    SpvOpTypeRuntimeArray | (3u<<16u),
    7u, // %_runtimearr_uint
    6u, // %uint

    SpvOpTypeStruct | (3u<<16u),
    8u, // %Data
    7u, // %_runtimearr_uint

    SpvOpTypePointer | (4u<<16u),
    9u, // %_ptr_Uniform_Data
    SpvStorageClassUniform,
    8u,

    SpvOpVariable | (4u<<16u),
    9u,  //%_ptr_Uniform_Data
    10u, //%_
    SpvStorageClassUniform,

    SpvOpTypeInt | (4u<<16u),
    11u, //%int
    32u,
    1u,

    SpvOpConstant | (4u<<16u),
    11u, //%int
    12u, //%int_0
    0u,

    SpvOpTypeVector | (4u<<16u),
    13u, //%v3uint
    6u,  //%uint
    3u,

    SpvOpTypePointer | (4u<<16u),
    14u, //%_ptr_Input_v3uint_Input
    SpvStorageClassInput,
    13u, //%v3uint

    SpvOpVariable | (4u<<16u),
    14u, //%_ptr_Input_v3uint_Input
    15u, //%gl_GlobalInvocationID
    SpvStorageClassInput,

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    16u, //%uint_0
    0u,

    SpvOpTypePointer | (4u<<16u),
    17u, //%_ptr_Input_uint
    SpvStorageClassInput,
    6u,  //%uint

    SpvOpTypePointer | (4u<<16u),
    22u, //%_ptr_Uniform_uint
    SpvStorageClassUniform,
    6u,  //%uint

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    24u, //%uint_64
    64u,

    SpvOpConstant | (4u<<16u),
    6u,  //%uint
    25u, //%uint_1
    1u,

    SpvOpConstantComposite | (6u<<16u),
    13u,  //%gl_WorkGroupSize
    26u,  //%v3uint
    24u,  //
    25u,
    25u,

    SpvOpFunction | (5u<<16u),
    2u, //%void
    4u, //%main
    SpvFunctionControlMaskNone,
    3u, //%3

    SpvOpLabel | (2u<<16u),
    29u, //%5

    SpvOpAccessChain | (5u<<16u),
    17u, //%_ptr_Input_uint
    30u, //%30
    15u, //%gl_GlobalInvocationID
    16u, //%uint_0

    SpvOpLoad | (4u<<16u),
    6u,  //%uint
    31u, //%31
    30u, //%30

    SpvOpAccessChain | (5u<<16u),
    17u, //%_ptr_Input_uint
    32u, //%32
    15u, //%gl_GlobalInvocationID
    16u, //%uint_0

    SpvOpLoad | (4u<<16u),
    6u,  //%uint
    33u, //%33
    32u, //%32

    SpvOpAccessChain | (6u<<16u),
    22u, //%_ptr_Uniform_uint
    34u, //%34
    10u, //%_
    16u, //%uint_0
    31u, //%31u

    SpvOpStore | (3u<<16u),
    34u, //%34
    33u, //%33

    SpvOpReturn | (1u<<16u),

    SpvOpFunctionEnd | (1u<<16u),
  };

  auto n = sizeof(code)/sizeof(uint32_t);
  auto file = fopen("cpp_comp.spv","wb");
  fwrite(code,sizeof(uint32_t),n,file);
  fclose(file);
  return 0;
}
