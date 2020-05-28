#include "DCSHMPController.h"
DCSHMPController::DCSHMPController(UA_Server *server, std::string name,
                                   int size)
    : DCSDeviceController(server, name), size(size) {
  auto &m = addVariable("measurements",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPM]);
  addVariableUpdate(m, 1000, &DCSHMPController::getMeasurements, this);
  auto &c = addVariable("configuration",
                        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC]);
  addVariableUpdate(c, 1000, &DCSHMPController::getConfiguration, this);
  addControllerMethod("setchannel", "Sets channel ON/OFF",
                      {{"Channel", "Channels number", UA_TYPES[UA_TYPES_INT16]},
                       {"State", "ON/OFF", UA_TYPES[UA_TYPES_BOOLEAN]}},
                      {}, &DCSHMPController::setChannel, this);
  addControllerMethod("setoutput", "Sets output ON/OFF",
                      {{"State", "ON/OFF", UA_TYPES[UA_TYPES_BOOLEAN]}}, {},
                      &DCSHMPController::setOutput, this);
  addControllerMethod("setvoltage", "Sets voltage",
                      {{"Channel", "Channels number", UA_TYPES[UA_TYPES_INT16]},
                       {"Voltage", "Voltage in V", UA_TYPES[UA_TYPES_DOUBLE]}},
                      {}, &DCSHMPController::setVoltage, this);
  addControllerMethod("setcurrent", "Sets current",
                      {{"Channel", "Channels number", UA_TYPES[UA_TYPES_INT16]},
                       {"Current", "Current in A", UA_TYPES[UA_TYPES_DOUBLE]}},
                      {}, &DCSHMPController::setCurrent, this);
  addControllerMethod("dump","dump",{},{},&DCSHMPController::dumpConfig,this);
}

UA_HMPm DCSHMPController::getMeasurements() {
  std::string response;
  UA_HMPm hmp;
  UA_HMPm_init(&hmp);
  response = device.getOutputGen();
  hmp.output = (response == "1") ? true : false;
  hmp.chSize = size;
  hmp.currentSize = size;
  hmp.voltageSize = size;
  hmp.ch = static_cast<UA_Boolean *>(
      UA_Array_new(size, &UA_TYPES[UA_TYPES_BOOLEAN]));
  hmp.current =
      static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
  hmp.voltage =
      static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
  for (int i = 0; i != size; ++i) {
    device.setActiveChannel(i + 1);
    response = device.getOutputSel();
    hmp.ch[i] = (response == "1") ? true : false;
    hmp.current[i] = std::stod(device.getCurrent());
    hmp.voltage[i] = std::stod(device.getVoltage());
  }
  return hmp;
}

UA_HMPc DCSHMPController::getConfiguration() {
  UA_HMPc hmp;
  UA_HMPc_init(&hmp);
  hmp.currentSetSize = size;
  hmp.voltageSetSize = size;
  hmp.currentSet =
      static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
  hmp.voltageSet =
      static_cast<UA_Double *>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
  for (int i = 0; i != size; i++) {
    device.setActiveChannel(i + 1);
    hmp.currentSet[i] = std::stod(device.getCurrentSet());
    hmp.voltageSet[i] = std::stod(device.getVoltageSet());
  }
  return hmp;
}

void DCSHMPController::setChannel(std::vector<UA_Variant> input, UA_Variant *) {
  int channel = *static_cast<UA_Int16 *>(input.at(0).data);
  bool state = *static_cast<UA_Boolean *>(input.at(1).data);
  device.setOutputSel(channel, state);
}

void DCSHMPController::setOutput(std::vector<UA_Variant> input, UA_Variant *) {
  bool state = *static_cast<UA_Boolean *>(input.at(0).data);
  device.setOutputGen(state);
}

void DCSHMPController::setVoltage(std::vector<UA_Variant> input, UA_Variant *) {
  int channel = *static_cast<UA_Int16 *>(input.at(0).data);
  double v = *static_cast<UA_Double *>(input.at(1).data);
  device.setVoltage(channel, v);
}

void DCSHMPController::setCurrent(std::vector<UA_Variant> input, UA_Variant *) {
  int channel = *static_cast<UA_Int16 *>(input.at(0).data);
  double i = *static_cast<UA_Double *>(input.at(1).data);
  device.setCurrent(channel, i);
}