#ifndef ABSBMSSTRUCT_H
#define ABSBMSSTRUCT_H
#include <QtGlobal>
#include <QSet>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>

#include <QObject>

constexpr int ABS_BMS_SOC_LOW_LIMIT_SMALLEST = 0; // soc不提供服务的阈值可配置的下限
constexpr int ABS_BMS_SOC_WARN_LIMIT = 30; // soc低于此阈值表示电量低，作为可配置的上限
constexpr qint64 ABS_BMS_FW_MAX_SIZE_BYTE = 20 * 1024 * 1024;   // 20M
constexpr int BATTERY_RACKID_BASE = 0; // 簇ID从0开始
constexpr int BATTERY_MODULEID_BASE = 0; // 模块ID从0开始

enum ABSBmsType {    // 定义顺序不能修改，与capabilities中保持一致
    ABSBT_Unknown = 0,
    ABSBT_Rentec,       // 锐能
    ABSBT_Jsqx,         // 江苏汽协0012
    ABSBT_EllBms,       // 电拉拉bms
    // 新类型在此扩充
    ABSBT_END   // 无效值
};

// HV控制模式
enum ABSBmsHVMode_E {
    HVMode_Off = 0,
    HVMode_On = 1,
    HVMode_Unknown = 2
};

// 电池厂家信息
enum ABS_Manufacturer {
    ABSBMF_UNKNOWN = 0,
    ABSBMF_CATL    = 1,
    ABSBMF_LiShen  = 2,
    ABSBMF_MGL     = 3,
    ABSBMF_SAMSUNG = 4,
    ABSBMF_LG      = 5,
    ABSBMF_EVE     = 6,
    ABSBMF_BYD     = 7
};

// 充电桩状态
enum ABSBmsMode_E {
    ABSBM_Standby = 0,              // 待机
    ABSBM_Discharge = 1,            // 储能包放电
    ABSBM_DC_Charging = 2,          // 储能包直流充电状态
    ABSBM_AC_Charging = 3,          // 储能包交流充电状态
    ABSBM_Invalid = 4,              // 无效
    ABSBM_AC_Discharge = 5          // 交流放电模式
};

// 自检状态
enum ABSSelfCheckStatus_E {
    ABSSelfCheck_Checking = 0,  // 自检中
    ABSSelfCheck_Completed = 1, // 自检完成
    ABSSelfCheck_Error = 2      // 自检失败
};

// 电池被充电状态
enum ABSChargeStatus_E {
    ABSCS_NoCharge = 0,      // 未充电
    ABSCS_Charging = 1,      // 充电中
    ABSCS_ChargeCompleted = 2, // 充电完成
    ABSCS_ChargeError = 3    // 故障停止
};

/// 继电器开关状态
enum ABSRelayState {
    Relay_Reserved = 0,
    Relay_Open     = 1, // 开启（断开）
    Relay_Closed   = 2, // 闭合（接通）
    Relay_Invalid  = 3  // 无效
};

/// 报警等级
enum ABSFaultLevel {
    Fault_None  = 0,
    Fault_L1    = 1,
    Fault_L2    = 2,
    Fault_L3    = 3
};

// 充电枪连接状态
enum ABSGunConnection_E {
    ABSGunDisconnected = 0, // 充电枪未连接
    ABSGunConnected = 1     // 充电枪连接
};


// bms完整信息数据结构
// 电池状态
struct ABSBmsStatus {
    // 状态信息
    bool isSupportBmsMode = false; // 是否支持BMS模式
    ABSBmsMode_E bmsMode = ABSBM_Standby; ///< 当前BMS工作模式
    ABSChargeStatus_E chargeStatus = ABSCS_NoCharge; ///< 充电状态
    ABSSelfCheckStatus_E selfCheckStatus = ABSSelfCheck_Completed; ///< 自检状态
    ABSFaultLevel faultLevel = Fault_None; ///< 当前最高故障等级
    QSet<int> existErrCodes; ///< 当前存在的故障码集合
    bool isSupportHVCommand = false; // 是否支持HV命令
    ABSBmsHVMode_E hvCommandMode = HVMode_Off; ///< HV命令模式
    ABSGunConnection_E dcGunConnection = ABSGunDisconnected; ///< 直流充电枪连接状态
    ABSGunConnection_E acGunConnection = ABSGunDisconnected; ///< 交流充电枪

    // 实时数据
    float volV = 0; // 实时电压，单位v
    float curA = 0; // 实时电流，单位A
    float soc = 0;  // soc 单位%
    float soh = 0;  // soh 单位%
    bool isSupportChargeRequire = false;
    float chargeRequireCurA = 0; // 充电需求电流，部分bms不支持
    float chargeRequireVolV = 0; // 充电需求电压，部分bms不支持
    float maxAllowChargeCurA = 0.0f;   ///< 最大允许充电电流，单位：A
    float maxAllowDischargeCurA = 0.0f;///< 最大允许放电电流，单位：A

    // 电压统计
    float cellLowestVolV = 0;         // 最小单体电压，单位v
    qint32 cellLowestCellNum = 0;    // 最小单体编号
    qint32 cellLowestCellBmuNum = 0; // 最小单体bmu编号

    float cellHighestVolV = 0.0f;         ///< 最大单体电压，单位：V
    qint32 cellHighestCellNum = 0;       ///< 最大电压电芯编号
    qint32 cellHighestCellBmuNum = 0;    ///< 最大电压BMU编号

    float cellAvgVolV = 0.0f;             ///< 单体电压平均值，单位：V
    float voltageDiff = 0.0f;             ///< 单体电压差，单位：V

    // 温度统计
    qint32 avgTemp = 0;               // 平均温度，单位℃
    qint32 maxTemp = 0;               // 最高温度，单位℃
    qint32 minTemp = 0;               // 最低温度，单位℃
    qint32 maxTempProbe = 0; ///< 最大温度探针编号
    qint32 minTempProbe = 0; ///< 最小温度探针编号
    qint32 maxTempBmu = 0;   ///< 最大温度所在BMU编号
    qint32 minTempBmu = 0;   ///< 最小温度所在BMU编号
    qint32 tempDiff = -1;      ///< 温差，单位：℃

    // 电量统计
    bool isSupportBatteryStats = false;  // 是否支持电量统计
    float totalChargeKWh = 0.0f;     ///< 累计充电电量，单位：kWh
    float totalDischargeKWh = 0.0f;  ///< 累计放电电量，单位：kWh
    float singleChargeKWh = 0.0f;    ///< 单次充电电量，单位：kWh

    // 电池包正极绝缘值（单位：kΩ）
    quint16 insulationPosKohm = 0;
    // 电池包负极绝缘值（单位：kΩ）
    quint16 insulationNegKohm = 0;
    
    // 时间戳
    QDateTime lastUpdateTime; ///< 最后更新时间
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["bmsModeSup"] = isSupportBmsMode;
        obj["mode"] = int(bmsMode);
        obj["chgStat"] = int(chargeStatus);
        obj["selfChk"] = int(selfCheckStatus);
        obj["faultLvl"] = int(faultLevel);

        QJsonArray errArray;
        for (int code : existErrCodes) {
            errArray.append(code);
        }
        obj["errs"] = errArray;

        obj["hvCmdSup"] = isSupportHVCommand;
        obj["hvMode"] = int(hvCommandMode);
        obj["dcGun"] = int(dcGunConnection);
        obj["acGun"] = int(acGunConnection);

        obj["vol"] = volV;
        obj["cur"] = curA;
        obj["soc"] = soc;
        obj["soh"] = soh;

        obj["chgReqSup"] = isSupportChargeRequire;
        obj["chgReqCur"] = chargeRequireCurA;
        obj["chgReqVol"] = chargeRequireVolV;
        obj["maxChgCur"] = maxAllowChargeCurA;
        obj["maxDchgCur"] = maxAllowDischargeCurA;

        obj["cellMinV"] = cellLowestVolV;
        obj["cellMinIdx"] = cellLowestCellNum;
        obj["cellMinBmu"] = cellLowestCellBmuNum;

        obj["cellMaxV"] = cellHighestVolV;
        obj["cellMaxIdx"] = cellHighestCellNum;
        obj["cellMaxBmu"] = cellHighestCellBmuNum;

        obj["cellAvgV"] = cellAvgVolV;
        obj["cellDiff"] = voltageDiff;

        obj["tmpAvg"] = avgTemp;
        obj["tmpMax"] = maxTemp;
        obj["tmpMin"] = minTemp;
        obj["tmpMaxIdx"] = maxTempProbe;
        obj["tmpMinIdx"] = minTempProbe;
        obj["tmpMaxBmu"] = maxTempBmu;
        obj["tmpMinBmu"] = minTempBmu;
        obj["tmpDiff"] = tempDiff;

        obj["batStatSup"] = isSupportBatteryStats;
        obj["chgTotal"] = totalChargeKWh;
        obj["dchgTotal"] = totalDischargeKWh;
        obj["chgOnce"] = singleChargeKWh;

        obj["insuPos"] = insulationPosKohm;
        obj["insuNeg"] = insulationNegKohm;

        obj["ts"] = lastUpdateTime.toString(Qt::ISODate);

        return obj;
    }

};
// 电池包信息
struct ABSBmsInfo {
    ABSBmsType type = ABSBT_Unknown; ///< BMS类型
    QString sn;                ///< 电池包编号（SN）
    ABS_Manufacturer manufacturer = ABSBMF_UNKNOWN; ///< 厂商
    quint16 ratedCapacityAh = 0; ///< 额定容量，单位：Ah
    float ratedVoltage = 0.0f;   ///< 额定电压，单位：V
    float ratedEnergyKWh = 0.0f; ///< 额定能量，单位：kWh
    quint8 cellCount = 0;        ///< 单体电芯总数
    quint8 tempSensorCount = 0;  ///< 温度探针总数
    QString version;             ///< 版本
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["type"] = int(type);
        obj["sn"] = sn;
        obj["manu"] = int(manufacturer);
        obj["cap"] = ratedCapacityAh;
        obj["vol"] = ratedVoltage;
        obj["ene"] = ratedEnergyKWh;
        obj["cells"] = cellCount;
        obj["temps"] = tempSensorCount;
        obj["ver"] = version;
        return obj;
    }

};
// 继电器状态
struct ABSBmsRelayStatus {
    qint32 rcv;
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["rcv"] = int(rcv);
        return obj;
    }

};

/// 单体电芯电压信息（单位：V）
struct ABSCellVoltage {
    float voltage = -1.0f; ///< 电压值，单位：V，-1表示无效
    ABSCellVoltage() = default;
    ABSCellVoltage(float vol){voltage = vol;}
    operator float() const {return voltage;}
};

/// 温度探针信息（单位：℃）
struct ABSTempSensor {
    int temperature = -127; ///< 温度值，单位：℃，-127表示无效
    ABSTempSensor() = default;
    ABSTempSensor(int temp){temperature = temp;}
    operator int() const {return temperature;}
};

/// 电池模块（Module）信息
struct ABSBatteryModule {
    const static int MaxCellCount = 2048; ///< 模块内最大电芯数量
    QVector<ABSCellVoltage> cells; ///< 模块内所有电芯电压
    QVector<ABSTempSensor> temps;  ///< 模块内所有温感探针
    void clear() {
        cells.clear();
        temps.clear();
    }
    void addCellVoltage(int cellNum, float voltage) {
        if (cellNum > MaxCellCount) return;
        int index = cellNum;
        if (index >= cells.size()) {
            cells.resize(index + 1);
        }
        cells[index].voltage = voltage;
    }
    void addTempSensor(int sensorNum, int temperature) {
        if (sensorNum > MaxCellCount) return;
        int index = sensorNum;
        if (index >= temps.size()) {
            temps.resize(index + 1);
        }
        temps[index].temperature = temperature;
    }
    QJsonObject toJson() const {
        QJsonObject obj;

        QJsonArray cellArray;
        for (const auto& cell : cells) {
            cellArray.append(cell.voltage);
        }
        obj["c"] = cellArray;  // "c" = cells

        QJsonArray tempArray;
        for (const auto& temp : temps) {
            tempArray.append(temp.temperature);
        }
        obj["t"] = tempArray;  // "t" = temps

        return obj;
    }


};

/// 电池簇（Rack）信息
struct ABSBatteryRack {
    QMap<int, ABSBatteryModule> modules; ///< 多个模块的信息，key为模块编号
    void clear() {
        modules.clear();
    }
    void addCellVoltage(int moduleNum, int cellNum, float voltage) {
        if (!modules.contains(moduleNum)) {
            modules.insert(moduleNum, ABSBatteryModule());
        }
        modules[moduleNum].addCellVoltage(cellNum, voltage);
    }
    void addTempSensor(int moduleNum, int sensorNum, int temperature) {
        if (!modules.contains(moduleNum)) {
            modules.insert(moduleNum, ABSBatteryModule());
        }
        modules[moduleNum].addTempSensor(sensorNum, temperature);
    }
    QJsonObject toJson() const {
        QJsonObject obj;
        for (auto it = modules.begin(); it != modules.end(); ++it) {
            obj[QString::number(it.key())] = it.value().toJson();
        }
        return obj;
    }

};

/// 电池集群（Cluster）信息
struct ABSBatteryClusterInfo {
    QMap<int, ABSBatteryRack> racks; ///< 多个簇的信息，key为簇编号
    void clear() {
        racks.clear();
    }
    void addCellVoltage(int rackNum, int moduleNum, int cellNum, float voltage) {
        if (!racks.contains(rackNum)) {
            racks.insert(rackNum, ABSBatteryRack());
        }
        racks[rackNum].addCellVoltage(moduleNum, cellNum, voltage);
    }
    void addTempSensor(int rackNum, int moduleNum, int sensorNum, int temperature) {
        if (!racks.contains(rackNum)) {
            racks.insert(rackNum, ABSBatteryRack());
        }
        racks[rackNum].addTempSensor(moduleNum, sensorNum, temperature);
    }
    QJsonObject toJson() const {
        QJsonObject obj;
        for (auto it = racks.begin(); it != racks.end(); ++it) {
            obj[QString::number(it.key())] = it.value().toJson();
        }
        return obj;
    }

};

#endif   // ABSBMSSTRUCT_H
