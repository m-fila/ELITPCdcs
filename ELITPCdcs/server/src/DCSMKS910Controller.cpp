#include "DCSMKS910Controller.h"
DCSMKS910Controller::DCSMKS910Controller(UA_Server *server, std::string name)
    : DCSDeviceController(server, name) {
  auto &m = addVariable("measurements",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910M]);
  addVariableUpdate(m, 1000, &DCSMKS910Controller::getMeasurements, this);
  auto &c = addVariable("configuration",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_MKS910C]);
  addVariableUpdate(c, 1000, &DCSMKS910Controller::getConfiguration, this);
  addControllerMethod("setunits", "Sets pressure units",
                      {{"Unit", "PASCAL/BAR/TORR", UA_TYPES[UA_TYPES_INT16]}},
                      {}, &DCSMKS910Controller::setUnits, this);
  addVariable("relay", UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_RELAY]);
}

UA_MKS910m DCSMKS910Controller::getMeasurements() {
  UA_MKS910m mks;
  UA_MKS910m_init(&mks);
  mks.vacuum = std::stod(device.getPCombinedLong());
  mks.temperature = std::stod(device.getTemp());
  mks.status =
      static_cast<int>(MKS910codes::statusFromString.at(device.getStatus()));
  mks.units =
      static_cast<int>(MKS910codes::unitsFromString.at(device.getUnits()));
  return mks;
}

UA_MKS910c DCSMKS910Controller::getConfiguration() {
  UA_MKS910c mks;
  UA_MKS910c_init(&mks);
  return mks;
}

void DCSMKS910Controller::setUnits(std::vector<UA_Variant> input,
                                   UA_Variant *) {
  int unit = *static_cast<UA_Int16 *>(input.at(0).data);
  device.setUnits(static_cast<MKS910codes::Units>(unit));
}
