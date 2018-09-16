cmake_minimum_required(VERSION 3.10)

project(CmakePrograming)

set(alist a b c)
set(blist d e f)
foreach(pp ${alist} ${blist})
  message("${pp}")
endforeach()

#list of libraries to find
set(ExternLibraries 
  NIndex\\ 1.0\\ CONFIG
  TextUtils\\ 1.0\\ CONFIG
  DefineList\\ CONFIG
  )

#list of targets we need to link to
set(TargetsToLink
  NIndex::NIndex
  TextUtils::TextUtils
  )

foreach(lib ${ExternLibraries})
  list(GET lib 0 libName)
  if(NOT TARGET ${libName}) 
    find_package(${lib})
  endif()
endforeach()

#create content of *Config.cmake script
string(CONCAT ConfigContent
  "include(CMakeFindDependencyMacro)\n"
  )

foreach(lib ${ExternLibraries})
  list(GET lib 0 libName)
  list(LENGTH lib len)
  if(${len} LESS 2)
    string(CONCAT ConfigContent
      ${ConfigContent}
      "find_dependency(" ${libName} ")\n"
      )
  else()
    list(GET lib 1 libVersion)
    #try to match the version
    string(REGEX MATCH "^[0-9]+(\\.[0-9]+)$" matchedVersion ${libVersion})
    if("${matchedVersion}" STREQUAL "")
      string(CONCAT ConfigContent
        ${ConfigContent}
        "find_dependency(" ${libName} ")\n"
        )
    else()
      string(CONCAT ConfigContent
        ${ConfigContent}
        "find_dependency(" ${libName} " " ${libVersion} ")\n"
        )
    endif()
  endif()
endforeach()

string(CONCAT ConfigContent
  ${ConfigContent}
  "include($" "{CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)\n"
  )

message(${ConfigContent})

