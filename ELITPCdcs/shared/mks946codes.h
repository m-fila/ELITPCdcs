#ifndef MKS946CODES_H
#define MKS946CODES_H
#include <map>
#include <string>

namespace MKS946codes {
enum class Units { PASCAL, MBAR, TORR, MICRON };

static std::map<Units, std::string> unitsToString = {
    {Units::PASCAL, "PASCAL"},
    {Units::MBAR, "MBAR"},
    {Units::TORR, "TORR"},
    {Units::MICRON, "MICRON"},
};

static std::map<std::string, Units> unitsFromString = {{"PASCAL", Units::PASCAL},
                                                       {"MBAR", Units::MBAR},
                                                       {"TORR", Units::TORR},
                                                       {"MICRON", Units::MICRON}};

enum class Mode { OPEN, CLOSE, SETPOINT, PCTRL, RATIO, RESET };

static std::map<Mode, std::string> modeToString = {
    {Mode::CLOSE, "CLOSE"}, {Mode::OPEN, "OPEN"},   {Mode::SETPOINT, "SETPOINT"},
    {Mode::PCTRL, "PCTRL"}, {Mode::RATIO, "RATIO"}, {Mode::RESET, "RESET"},
};

static std::map<std::string, Mode> modeFromString = {
    {"CLOSE", Mode::CLOSE}, {"OPEN", Mode::OPEN},   {"SETPOINT", Mode::SETPOINT},
    {"PCTRL", Mode::PCTRL}, {"RATIO", Mode::RATIO}, {"RESET", Mode::RESET}};

enum class Error {
    WRONG_GAUGE = 150,
    NO_GAUGE = 151,
    NOT_IONGAUGE = 152,
    NOT_HOTCATHODE = 153,
    NOT_COLDCATHODE = 154,
    NOT_CAPACITANCE_MANOMETER = 155,
    NOT_PIRANI_OR_CTP = 156,
    NOT_PR_OR_CM = 157,
    NOT_MFC = 158,
    NOT_VLV = 159,
    UNRECOGNIZED_MSG = 160,
    SET_CMD_LOCK = 161,
    RLY_DIR_FIX_FOR_ION = 162,
    INVALID_CHANNEL = 163,
    DIFF_CM = 164,
    INVALID_PID_PARAM = 165,
    PID_IN_PROGRESS = 166,
    INVALID_RATIO_PARAM = 167,
    NOT_IN_DEGAS = 168,
    INVALID_ARGUMENT = 169,
    VALUE_OUT_OF_RANGE = 172,
    INVALID_CTRL_CHAN = 173,
    CMD_QUERY_BYTE_INVALID = 175,
    NO_GAS_TYPE = 176,
    NOT_485 = 177,
    CAL_DISABLED = 178,
    SET_POINT_NOT_ENABLED = 179,
    COMBINATION_DISABLED = 181,
    INTERNATIONAL_UNIT_ONLY = 182,
    GAS_TYPE_DEFINED = 183,
    NOT_RATIO_MODE = 191,
    CONTROL_SET_POINT_ENABLED = 195,
    PRESSURE_TOO_HIGH_FOR_DEGAS = 199
};

static std::map<Error, std::string> errorToString = {
    {Error::WRONG_GAUGE, "WRONG_GAUGE"},
    {Error::NO_GAUGE, "NO_GAUGE"},
    {Error::NOT_IONGAUGE, "NOT_IONGAUGE"},
    {Error::NOT_HOTCATHODE, "NOT_HOTCATHODE"},
    {Error::NOT_COLDCATHODE, "NOT_COLDCATHODE"},
    {Error::NOT_CAPACITANCE_MANOMETER, "NOT_CAPACITANCE_MANOMETER"},
    {Error::NOT_PIRANI_OR_CTP, "NOT_PIRANI_OR_CTP"},
    {Error::NOT_PR_OR_CM, "NOT_PR_OR_CM"},
    {Error::NOT_MFC, "NOT_MFC"},
    {Error::NOT_VLV, ":NOT_VLV"},
    {Error::UNRECOGNIZED_MSG, "UNRECOGNIZED_MSG"},
    {Error::SET_CMD_LOCK, "SET_CMD_LOC"},
    {Error::RLY_DIR_FIX_FOR_ION, ""},
    {Error::INVALID_CHANNEL, "INVALID_CHANNEL"},
    {Error::DIFF_CM, "DIFF_CM"},
    {Error::INVALID_PID_PARAM, "NVALID_PID_PARAM"},
    {Error::PID_IN_PROGRESS, "PID_IN_PROGRESS"},
    {Error::INVALID_RATIO_PARAM, "INVALID_RATIO_PARAM"},
    {Error::NOT_IN_DEGAS, "NOT_IN_DEGAS"},
    {Error::INVALID_ARGUMENT, "INVALID_ARGUMENT"},
    {Error::VALUE_OUT_OF_RANGE, "ALUE_OUT_OF_RANGE"},
    {Error::INVALID_CTRL_CHAN, "INVALID_CTRL_CHAN"},
    {Error::CMD_QUERY_BYTE_INVALID, "CMD_QUERY_BYTE_INVALID"},
    {Error::NO_GAS_TYPE, "NO_GAS_TYPE"},
    {Error::NOT_485, "NOT_485"},
    {Error::CAL_DISABLED, "CAL_DISABLED"},
    {Error::SET_POINT_NOT_ENABLED, "SET_POINT_NOT_ENABLED"},
    {Error::COMBINATION_DISABLED, "COMBINATION_DISABLED"},
    {Error::INTERNATIONAL_UNIT_ONLY, "INTERNATIONAL_UNIT_ONLY"},
    {Error::GAS_TYPE_DEFINED, "GAS_TYPE_DEFINED"},
    {Error::NOT_RATIO_MODE, "NOT_RATIO_MODE"},
    {Error::CONTROL_SET_POINT_ENABLED, "CONTROL_SET_POINT_ENABLED "},
    {Error::PRESSURE_TOO_HIGH_FOR_DEGAS, "PRESSURE_TOO_HIGH_FOR_DEGAS"}};

enum class RelayDirection { Below, Above };

static std::map<std::string, RelayDirection> relayDirectionFromString = {
    {"BELOW", RelayDirection::Below}, {"ABOVE", RelayDirection::Above}};

static std::map<RelayDirection, std::string> relayDirectionToString = {
    {RelayDirection::Below, "BELOW"}, {RelayDirection::Above, "ABOVE"}};

enum class RelayEnabled { Off, On, Enabled };

static std::map<std::string, RelayEnabled> relayEnabledFromString = {
    {"CLEAR", RelayEnabled::Off},
    {"SET", RelayEnabled::On},
    {"ENABLE", RelayEnabled::Enabled}};

static std::map<RelayEnabled, std::string> relayEnabledToString = {
    {RelayEnabled::Off, "CLEAR"},
    {RelayEnabled::On, "SET"},
    {RelayEnabled::Enabled, "ENABLE"}};

enum class RelaySet { Clear, Set };

static std::map<std::string, RelaySet> relaySetFromString = {{"CLEAR", RelaySet::Clear},
                                                             {"SET", RelaySet::Set}};

static std::map<RelaySet, std::string> relaySetToString = {{RelaySet::Clear, "CLEAR"},
                                                           {RelaySet::Set, "SET"}};

}  // namespace MKS946codes
#endif  // MKS946CODES_H
