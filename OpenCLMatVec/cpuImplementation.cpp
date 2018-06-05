#include <CPUTaskData.h>
#include <cpuImplementation.h>

void cpuImplementation0(std::unique_ptr<CPUTaskData> const& data,
                       TaskSize const&                     size) {
  for (std::size_t o = 0; o < size.output; ++o)
    for (std::size_t b = 0; b < size.batch; ++b) {
      float acc = 0;
      for (std::size_t i = 0; i < size.input; ++i)
        acc +=
            data->matrix[o * size.input + i] * data->input[b * size.input + i];
      data->output[o * size.batch + b] = acc;
    }
}

void cpuImplementation1(std::unique_ptr<CPUTaskData> const& data,
                       TaskSize const&                     size) {
  for (std::size_t o = 0; o < size.output; ++o)
    for (std::size_t b = 0; b < size.batch; ++b) {
      float acc = 0;
      for (std::size_t i = 0; i < size.input; ++i)
        acc +=
            data->matrix[o * size.input + i] * data->input[b * size.input + i];
      data->output[b * size.output + o] = acc;
    }
}

void cpuImplementation2(std::unique_ptr<CPUTaskData> const& data,
                       TaskSize const&                     size) {
  for (std::size_t o = 0; o < size.output; ++o)
    for (std::size_t b = 0; b < size.batch; ++b) {
      float acc = 0;
      for (std::size_t i = 0; i < size.input; ++i)
        acc +=
            data->matrix[i * size.output + o] * data->input[b * size.input + i];
      data->output[o * size.batch + b] = acc;
    }
}

void cpuImplementation3(std::unique_ptr<CPUTaskData> const& data,
                       TaskSize const&                     size) {
  for (std::size_t o = 0; o < size.output; ++o)
    for (std::size_t b = 0; b < size.batch; ++b) {
      float acc = 0;
      for (std::size_t i = 0; i < size.input; ++i)
        acc +=
            data->matrix[o * size.input + i] * data->input[i * size.batch + b];
      data->output[o * size.batch + b] = acc;
    }
}

void cpuImplementation4(std::unique_ptr<CPUTaskData> const& data,
                       TaskSize const&                     size) {
  for (std::size_t o = 0; o < size.output; ++o)
    for (std::size_t b = 0; b < size.batch; ++b) {
      float acc = 0;
      for (std::size_t i = 0; i < size.input; ++i)
        acc +=
            data->matrix[o * size.input + i] * data->input[i * size.batch + b];
      data->output[b * size.output + o] = acc;
    }
}

