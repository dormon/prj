#pragma once

#include<memory>
#include<TaskSize.h>

class CPUTaskData;

void cpuImplementation0(
    std::unique_ptr<CPUTaskData>const&Data,
    TaskSize                    const&size);

void cpuImplementation1(
    std::unique_ptr<CPUTaskData>const&Data,
    TaskSize                    const&size);

void cpuImplementation2(
    std::unique_ptr<CPUTaskData>const&Data,
    TaskSize                    const&size);

void cpuImplementation3(
    std::unique_ptr<CPUTaskData>const&Data,
    TaskSize                    const&size);

void cpuImplementation4(
    std::unique_ptr<CPUTaskData>const&Data,
    TaskSize                    const&size);
