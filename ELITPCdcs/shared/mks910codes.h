#ifndef MKS910CODES_H
#define MKS910CODES_H
#include <map>
#include <string>

namespace MKS910codes {
enum class Units { PASCAL, MBAR, TORR };

static std::map<Units, std::string> unitsToString = {
    {Units::PASCAL, "PASCAL"}, {Units::MBAR, "MBAR"}, {Units::TORR, "TORR"}};

static std::map<std::string, Units> unitsFromString = {
    {"PASCAL", Units::PASCAL}, {"MBAR", Units::MBAR}, {"TORR", Units::TORR}};

enum class Status { O, M, Z };
static std::map<std::string, Status> statusFromString = {
    {"O", Status::O}, {"M", Status::M}, {"Z", Status::Z}};

static std::map<Status, std::string> statusToString = {
    {Status::O, "O"}, {Status::M, "M"}, {Status::Z, "Z"}};

enum class RelayDirection { Below, Above };

static std::map<std::string, RelayDirection> relayDirectionFromString = {
    {"BELOW", RelayDirection::Below}, {"ABOVE", RelayDirection::Above}};

static std::map<RelayDirection, std::string> relayDirectionToString = {
    {RelayDirection::Below, "BELOW"}, {RelayDirection::Above, "ABOVE"}};

enum class RelayEnabled { Off, Diff, Abs };

static std::map<std::string, RelayEnabled> relayEnabledFromString = {
    {"OFF", RelayEnabled::Off},
    {"DIFF", RelayEnabled::Diff},
    {"ABS", RelayEnabled::Abs}};

static std::map<RelayEnabled, std::string> relayEnabledToString = {
    {RelayEnabled::Off, "OFF"},
    {RelayEnabled::Diff, "DIFF"},
    {RelayEnabled::Abs, "ABS"}};

enum class RelaySet { Clear, Set };

static std::map<std::string, RelaySet> relaySetFromString = {
    {"CLEAR", RelaySet::Clear}, {"SET", RelaySet::Set}};

static std::map<RelaySet, std::string> relaySetToString = {
    {RelaySet::Clear, "CLEAR"}, {RelaySet::Set, "SET"}};

enum class RelayDelay { Off, On };

static std::map<std::string, RelayDelay> relayDelayFromString = {
    {"ON", RelayDelay::On}, {"OFF", RelayDelay::Off}};

static std::map<RelayDelay, std::string> relayDelayToString = {
    {RelayDelay::On, "ON"}, {RelayDelay::Off, "OFF"}};

}
#endif // MKS910CODES_H
