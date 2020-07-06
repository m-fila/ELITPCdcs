#ifndef DCS_DUMMY_CONTROLLER_H
#define DCS_DUMMY_CONTROLLER_H

#include "DCSDeviceController.h"
#include "GenericDevice.h"
#include "json.hpp"
#include <stdexcept>
class DCSDummyController : public DCSDeviceController<GenericDevice>,
                           DCSObjectFactory::Register<DCSDummyController> {
  friend DCSServer;
  friend DCSObjectFactory;

protected:
  void addProfiles();
  void addChildren(Options options) override;
  DCSDummyController(){};
  virtual void parseProfile(const nlohmann::json &profile) {}

private:
  static std::string GetType() { return "Dummy"; }

  void dumpProfile(DCSVariable &profiles, const UA_Variant *input,
                   UA_Variant *output);
  void setProfile(DCSVariable &profile, const UA_Variant *input,
                  UA_Variant *output);
  void applyProfile(const DCSVariable &profile, const DCSVariable &profiles,
                    const UA_Variant *input, UA_Variant *output);
  void updateProfiles(DCSVariable &profiles);
};
// REGISTER_OBJECT(DCSDummyController)
#endif