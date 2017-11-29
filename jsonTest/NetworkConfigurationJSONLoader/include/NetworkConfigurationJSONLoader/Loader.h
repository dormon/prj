#pragma once

#include<NetworkConfiguration/NetworkConfiguration.h>

#include<NetworkConfigurationJSONLoader/networkconfigurationjsonloader_export.h>

namespace cnnConf{
  NETWORKCONFIGURATIONJSONLOADER_EXPORT std::shared_ptr<cnnConf::Network>loadJSON(std::string const&fileName);
}
