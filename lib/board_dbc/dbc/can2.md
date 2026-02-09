# DBC Documentation

## Index

| Message Name | ID (Dec) | ID (Hex) | Notes |
|--------------|---------|----------|-------|
| [Arduino1](#arduino1) | 8 | 0x8 |  |
| [LVError](#lverror) | 20 | 0x14 |  |
| [ResGO](#resgo) | 32 | 0x20 |  |
| [EbsStatus](#ebsstatus) | 60 | 0x3C |  |
| [Asms](#asms) | 65 | 0x41 |  |
| [CarMission](#carmission) | 71 | 0x47 |  |
| [PcuFault](#pcufault) | 81 | 0x51 |  |
| [Paddle](#paddle) | 82 | 0x52 |  |
| [Driver](#driver) | 83 | 0x53 |  |
| [BmsLv1](#bmslv1) | 84 | 0x54 |  |
| [BmsLv2](#bmslv2) | 85 | 0x55 |  |
| [BmsHv1](#bmshv1) | 87 | 0x57 |  |
| [BmsHv2](#bmshv2) | 88 | 0x58 |  |
| [Imu1](#imu1) | 96 | 0x60 |  |
| [Imu2](#imu2) | 97 | 0x61 |  |
| [Imu3](#imu3) | 98 | 0x62 |  |
| [IMUCalib](#imucalib) | 99 | 0x63 | RESERVER FOR IMU mask - DO NOT USE |
| [Map](#map) | 100 | 0x64 |  |
| [CarStatus](#carstatus) | 101 | 0x65 |  |
| [CarSettings](#carsettings) | 102 | 0x66 |  |
| [CheckASBReq](#checkasbreq) | 104 | 0x68 |  |
| [EbsBrakeReq](#ebsbrakereq) | 105 | 0x69 |  |
| [ResStatus](#resstatus) | 106 | 0x6A |  |
| [LapStart](#lapstart) | 112 | 0x70 |  |
| [CarMissionStatus](#carmissionstatus) | 113 | 0x71 |  |
| [Imu4](#imu4) | 128 | 0x80 |  |
| [Imu5](#imu5) | 129 | 0x81 |  |
| [Temp1](#temp1) | 256 | 0x100 |  |
| [Temp2](#temp2) | 257 | 0x101 |  |
| [SuspRear](#susprear) | 258 | 0x102 |  |
| [RESERVED2](#reserved2) | 259 | 0x103 | RESERVER FOR SMU mask - DO NOT USE |
| [SuspFront](#suspfront) | 260 | 0x104 |  |
| [TempFrontR](#tempfrontr) | 261 | 0x105 |  |
| [HydraulicPressure](#hydraulicpressure) | 264 | 0x108 | Hydraulic Brakes Pressures |
| [TempImu](#tempimu) | 277 | 0x115 |  |
| [InvVolt](#invvolt) | 288 | 0x120 |  |
| [Pcu](#pcu) | 304 | 0x130 |  |
| [Calib](#calib) | 305 | 0x131 |  |
| [CalibAck](#caliback) | 306 | 0x132 |  |
| [PcuSwControl](#pcuswcontrol) | 307 | 0x133 |  |
| [PcuRfAck](#pcurfack) | 308 | 0x134 |  |
| [EmbeddedAliveCheck](#embeddedalivecheck) | 310 | 0x136 |  |
| [CalibRegen](#calibregen) | 328 | 0x148 |  |
| [CalibRegenAck](#calibregenack) | 329 | 0x149 |  |
| [PcuAdc1](#pcuadc1) | 331 | 0x14B |  |
| [PcuAdc2](#pcuadc2) | 332 | 0x14C |  |
| [PcuAdc3](#pcuadc3) | 333 | 0x14D |  |
| [coolingControl](#coolingcontrol) | 334 | 0x14E |  |
| [Balancing](#balancing) | 420 | 0x1A4 |  |
| [DisplayACK](#displayack) | 600 | 0x258 |  |
| [MapAck](#mapack) | 700 | 0x2BC |  |
| [VcuErrTrace](#vcuerrtrace) | 800 | 0x320 |  |
| [CsLog_1](#cslog_1) | 820 | 0x334 |  |
| [CsLog_2](#cslog_2) | 821 | 0x335 |  |
| [CsLog_3](#cslog_3) | 822 | 0x336 |  |
| [BmsLvCellControl](#bmslvcellcontrol) | 869 | 0x365 |  |
| [BmsLvCell_1](#bmslvcell_1) | 870 | 0x366 |  |
| [BmsLvCell_2](#bmslvcell_2) | 871 | 0x367 |  |
| [BmsLvCell_3](#bmslvcell_3) | 872 | 0x368 |  |
| [BmsLvTemps](#bmslvtemps) | 873 | 0x369 |  |
| [Lem](#lem) | 962 | 0x3C2 |  |


#### Arduino1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 8 | 0x8 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| temp_1 | 0 | 32 | little_endian | True | 1 | 0 | None | None | C | VCU |
| temp_2 | 32 | 32 | little_endian | True | 1 | 0 | None | None | C | VCU |


#### LVError

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 20 | 0x14 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| error_lv | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | None |  |

**Enumerations:**

- **error_lv**:
  - 1: Error
  - 0: No_Error


#### ResGO

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 32 | 0x20 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| go_signal | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |


#### EbsStatus

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 60 | 0x3C | 5 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| system_check | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| sanity_left_sensor | 1 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| sanity_right_sensor | 2 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| ASB_check | 3 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| brakes_engaged | 4 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| brake_consistency | 5 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| tank_brake_coherence | 6 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| _NOT_IN_USE | 7 | 1 | little_endian | False | 1 | 0 | None | None | None |  |
| press_left_tank | 8 | 16 | little_endian | False | 0.001 | 0 | 0 | 10 | Bar |  |
| press_right_tank | 24 | 16 | little_endian | False | 0.001 | 0 | 0 | 10 | Bar |  |


#### Asms

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 65 | 0x41 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| Asms_sens | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | High |  |


#### CarMission

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 71 | 0x47 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| Mission | 0 | 3 | little_endian | False | 1 | 0 | 0 | 7 | None | VCU |

**Enumerations:**

- **Mission**:
  - 7: dv_inspection
  - 6: dv_ebs_test
  - 5: dv_trackdrive
  - 4: dv_autocross
  - 3: dv_skidpad
  - 2: dv_acceleration
  - 1: manualy
  - 0: none


#### PcuFault

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 81 | 0x51 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| fault_12v | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_dv | 1 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_24v | 2 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_pumpl | 3 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_pumpr | 4 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_fanbattr | 5 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| fault_fanbattl | 6 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |


#### Paddle

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 82 | 0x52 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| regen | 0 | 8 | little_endian | False | 1 | 0 | 0 | 100 | % | VCU |


#### Driver

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 83 | 0x53 | 4 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| throttle | 0 | 8 | little_endian | False | 1 | 0 | 0 | 100 | % | VCU |
| brake | 8 | 8 | little_endian | False | 1 | 0 | 0 | 100 | % | VCU |
| steering | 16 | 12 | little_endian | True | 1 | 0 | -120 | 120 | deg | VCU |
| no_implausibility | 28 | 1 | little_endian | False | 1 | 0 | None | None | None |  |
| bre_implausibility | 29 | 1 | little_endian | False | 1 | 0 | None | None | None |  |
| pad_implausibility | 30 | 1 | little_endian | False | 1 | 0 | None | None | None |  |
| pot_implausibility | 31 | 1 | little_endian | False | 1 | 0 | None | None | None |  |


#### BmsLv1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 84 | 0x54 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| max_volt | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| min_volt | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| avg_volt | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| tot_volt | 48 | 16 | little_endian | False | 0.01 | 0 | 0 | 100 | V |  |


#### BmsLv2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 85 | 0x55 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| max_temp | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | C | VCU |
| min_temp | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | C | VCU |
| current | 32 | 32 | little_endian | False | 0.1 | 0 | None | None | mA | VCU |


#### BmsHv1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 87 | 0x57 | 7 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| max_volt | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| min_volt | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| avg_volt | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | VCU |
| soc | 48 | 8 | little_endian | False | 1 | 0 | 0 | 100 | % | VCU |


#### BmsHv2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 88 | 0x58 | 7 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| max_temp | 0 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| min_temp | 16 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| avg_temp | 32 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| fan_speed | 48 | 8 | little_endian | False | 1 | 0 | 0 | 100 | % | VCU |


#### Imu1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 96 | 0x60 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| acc_x | 0 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |
| acc_y | 32 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |


#### Imu2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 97 | 0x61 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| acc_z | 0 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |
| omega_x | 32 | 32 | little_endian | True | 1 | 0 | None | None | rad/s | VCU |


#### Imu3

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 98 | 0x62 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| omega_y | 0 | 32 | little_endian | True | 1 | 0 | None | None | rad/s | VCU |
| omega_z | 32 | 32 | little_endian | True | 1 | 0 | None | None | rad/s | VCU |


#### IMUCalib

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 99 | 0x63 | 1 |

**Comment:** RESERVER FOR IMU mask - DO NOT USE

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| start_imu_calibration | 0 | 1 | little_endian | False | 1 | 0 | None | None | None |  |


#### Map

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 100 | 0x64 | 2 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| power | 0 | 4 | little_endian | False | 1 | 0 | 1 | 12 | map | VCU |
| regen | 4 | 4 | little_endian | False | 1 | 0 | 1 | 12 | map | VCU |
| torque_rep | 8 | 4 | little_endian | False | 1 | 0 | 0 | 12 | map |  |


#### CarStatus

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 101 | 0x65 | 5 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| HV | 0 | 1 | little_endian | False | 1 | 0 | None | None |  Off/On |  |
| AIR1 | 1 | 1 | little_endian | False | 1 | 0 | None | None |  Closed/Open |  |
| precharge | 2 | 1 | little_endian | False | 1 | 0 | None | None |  Closed/Open |  |
| AS_NODE | 3 | 1 | little_endian | False | 1 | 0 | None | None |  Open/Closed |  |
| SCS | 4 | 1 | little_endian | False | 1 | 0 | None | None |  Open/Closed |  |
| rtd_req | 5 | 1 | little_endian | False | 1 | 0 | None | None |  Open/Closed |  |
| RunningStatus | 6 | 2 | little_endian | False | 1 | 0 | 0 | 3 |  Phase |  |
| speed | 8 | 8 | little_endian | False | 1 | 0 | None | None |  km/h |  |
| brake_front_press | 16 | 8 | little_endian | False | 0.25 | 0 | 0 | 60 | Bar |  |
| brake_rear_press | 24 | 8 | little_endian | False | 0.25 | 0 | 0 | 60 | Bar |  |
| bre_impl | 32 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |

**Enumerations:**

- **RunningStatus**:
  - 3: Running
  - 2: Ts Ready
  - 1: Precharge started
  - 0: System off
- **bre_impl**:
  - 1: On
  - 0: Off


#### CarSettings

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 102 | 0x66 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| max_regen_current | 0 | 8 | little_endian | False | 1 | 0 | 0 | 150 | A |  |
| pwr_limit | 8 | 8 | little_endian | False | 1 | 0 | 0 | 80 | kW |  |
| speed_lim | 16 | 8 | little_endian | False | 1 | 0 | None | None | krpm |  |
| max_pos_trq | 24 | 8 | little_endian | False | 1 | 0 | None | None | Nm |  |
| max_neg_trq | 32 | 8 | little_endian | True | 1 | 0 | None | None | Nm |  |
| front_motor_repartition | 40 | 8 | little_endian | False | 1 | 0 | None | None | % |  |
| rear_motor_repartition | 48 | 8 | little_endian | False | 1 | 0 | None | None | % |  |
| torque_vectoring | 56 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |


#### CheckASBReq

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 104 | 0x68 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| req | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |
| reqAck | 1 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None |  |


#### EbsBrakeReq

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 105 | 0x69 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| req | 0 | 1 | little_endian | False | 1 | 0 | 0 | 2 | None |  |


#### ResStatus

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 106 | 0x6A | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| data | 0 | 1 | little_endian | False | 1 | 0 | 0 | 2 | None |  |


#### LapStart

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 112 | 0x70 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| start | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | start |  |


#### CarMissionStatus

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 113 | 0x71 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| Mission | 0 | 3 | little_endian | False | 1 | 0 | 0 | 7 | None | SW |
| MissionStatus | 3 | 2 | little_endian | False | 1 | 0 | 0 | 2 | None | SW |
| AsStatus | 5 | 3 | little_endian | False | 1 | 0 | 0 | 7 | None | SW |

**Enumerations:**

- **Mission**:
  - 7: dv_inspection
  - 6: dv_ebs_test
  - 5: dv_trackdrive
  - 4: dv_autocross
  - 3: dv_skidpad
  - 2: dv_acceleration
  - 1: manualy
  - 0: none
- **MissionStatus**:
  - 2: Mission_finished
  - 1: Mission_running
  - 0: Mission_not_running
- **AsStatus**:
  - 5: finish
  - 4: emergency_brake
  - 3: driving
  - 2: ready
  - 1: off


#### Imu4

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 128 | 0x80 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| mag_x | 0 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |
| mag_y | 32 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |


#### Imu5

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 129 | 0x81 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| mag_z | 0 | 32 | little_endian | True | 1 | 0 | None | None | m/s^2 | VCU |
| null | 32 | 32 | little_endian | True | 1 | 0 | None | None | rad/s | VCU |


#### Temp1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 256 | 0x100 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| TEMP_INVERTER_POST_R_PIN | 0 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| TEMP_RAD_POST_R_PIN | 16 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| TEMP_MOTOR_POST_RR_PIN | 32 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| TEMP_RAD_POST_L_PIN | 48 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |


#### Temp2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 257 | 0x101 | 7 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| TEMP_MOTOR_POST_RL_PIN | 0 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| TEMP_INVERTER_POST_L_PIN | 16 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| TEMP_EMBEDDED_POST_PIN | 32 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |


#### SuspRear

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 258 | 0x102 | 3 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| susp_rr | 0 | 12 | little_endian | False | 0.1 | 0 | None | None | mm | VCU |
| susp_rl | 12 | 12 | little_endian | False | 0.1 | 0 | None | None | mm | VCU |


#### RESERVED2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 259 | 0x103 | 0 |

**Comment:** RESERVER FOR SMU mask - DO NOT USE



#### SuspFront

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 260 | 0x104 | 3 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| susp_fr | 0 | 12 | little_endian | False | 0.1 | 0 | None | None | mm | VCU |
| susp_fl | 12 | 12 | little_endian | False | 0.1 | 0 | None | None | mm | VCU |


#### TempFrontR

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 261 | 0x105 | 3 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| temp_mot_pot_FR | 0 | 10 | little_endian | False | 1 | 0 | None | None | C |  |
| temp_mot_pre_FR | 10 | 10 | little_endian | False | 1 | 0 | None | None | C |  |


#### HydraulicPressure

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 264 | 0x108 | 8 |

**Comment:** Hydraulic Brakes Pressures

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| PressFront | 0 | 32 | little_endian | False | 1 | 0 | None | None | Bar | EBS |
| PressRear | 32 | 32 | little_endian | False | 1 | 0 | None | None | Bar | EBS |


#### TempImu

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 277 | 0x115 | 4 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| temp_pump_right | 0 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |
| temp_pump_left | 16 | 16 | little_endian | False | 1 | 0 | None | None | C | VCU |


#### InvVolt

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 288 | 0x120 | 2 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| car_voltage | 0 | 16 | little_endian | False | 1 | 0 | 0 | 600 | V | VCU |


#### Pcu

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 304 | 0x130 | 7 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| mode | 0 | 2 | little_endian | False | 1 | 0 | 0 | 2 | None |  |
| rf | 2 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| enable_dv | 2 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |
| enable_embedded | 3 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on |  |


#### Calib

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 305 | 0x131 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| position | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | high | SMUF |


#### CalibAck

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 306 | 0x132 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| position | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | high | SW |


#### PcuSwControl

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 307 | 0x133 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| pump | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None | VCU |
| fan | 1 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None | VCU |

**Enumerations:**

- **pump**:
  - 1: On
  - 0: Off
- **fan**:
  - 1: On
  - 0: Off


#### PcuRfAck

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 308 | 0x134 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| rf_signalAck | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | on | VCU |


#### EmbeddedAliveCheck

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 310 | 0x136 | 0 |



#### CalibRegen

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 328 | 0x148 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| arrive | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | arrive | SMUF |


#### CalibRegenAck

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 329 | 0x149 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| arrive | 0 | 8 | little_endian | False | 1 | 0 | 0 | 1 | arrive | SW |


#### PcuAdc1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 331 | 0x14B | 6 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| adc_24v | 0 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_pumpl | 16 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_pumpr | 32 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |


#### PcuAdc2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 332 | 0x14C | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| adc_fanbattl | 0 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_fanbattr | 16 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_fanradl | 32 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_fanradr | 48 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |


#### PcuAdc3

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 333 | 0x14D | 6 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| adc_dv | 0 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_emb | 16 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |
| adc_steeract | 32 | 16 | little_endian | False | 0.001 | 0 | 0 | 30 | A |  |


#### coolingControl

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 334 | 0x14E | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| pwm_fanbatt | 0 | 16 | little_endian | False | 1 | 0 | 0 | 65535 | None |  |
| pwm_fanrad | 16 | 16 | little_endian | False | 1 | 0 | 0 | 65535 | None |  |
| pwm_pumpl | 32 | 16 | little_endian | False | 1 | 0 | 0 | 65535 | None |  |
| pwm_pumpr | 48 | 16 | little_endian | False | 1 | 0 | 0 | 65535 | None |  |


#### Balancing

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 420 | 0x1A4 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| balancing_lv | 0 | 8 | little_endian | True | 1 | 0 | 0 | 1 | None | BMSLV |

**Enumerations:**

- **balancing_lv**:
  - 1: On
  - 0: Off


#### DisplayACK

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 600 | 0x258 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| UART_fault | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None | SW |


#### MapAck

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 700 | 0x2BC | 2 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| power | 0 | 4 | little_endian | False | 1 | 0 | 1 | 12 | map | VCU |
| regen | 4 | 4 | little_endian | False | 1 | 0 | 1 | 12 | map | VCU |
| torque_rep | 8 | 4 | little_endian | False | 1 | 0 | 0 | 12 | map |  |


#### VcuErrTrace

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 800 | 0x320 | 3 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| core_0_err | 0 | 8 | little_endian | False | 1 | 0 | 0 | 255 | None | VCU |
| core_1_err | 8 | 8 | little_endian | False | 1 | 0 | 0 | 255 | None | VCU |
| core_2_err | 16 | 8 | little_endian | False | 1 | 0 | 0 | 255 | None | VCU |


#### CsLog_1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 820 | 0x334 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| yr_ref | 0 | 32 | little_endian | False | 1 | 0 | -5 | 5 |  rad/s |  |
| yaw_moment | 32 | 32 | little_endian | False | 1 | 0 | -4000 | 4000 |  Nm |  |


#### CsLog_2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 821 | 0x335 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| delta_torque_fl | 0 | 16 | little_endian | True | 1 | 0 | None | None |  Nmm |  |
| delta_torque_fr | 16 | 16 | little_endian | True | 1 | 0 | None | None |  Nmm |  |
| delta_torque_rl | 32 | 16 | little_endian | True | 1 | 0 | None | None |  Nmm |  |
| delta_torque_rr | 48 | 16 | little_endian | True | 1 | 0 | None | None |  Nmm |  |


#### CsLog_3

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 822 | 0x336 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| vx | 0 | 32 | little_endian | False | 0.001 | 0 | 0 | 50 |  m/s |  |


#### BmsLvCellControl

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 869 | 0x365 | 1 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| enable | 0 | 1 | little_endian | False | 1 | 0 | 0 | 1 | None | BMSLV |


#### BmsLvCell_1

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 870 | 0x366 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| cell_0 | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_1 | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_2 | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_3 | 48 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |


#### BmsLvCell_2

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 871 | 0x367 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| cell_4 | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_5 | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_6 | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_7 | 48 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |


#### BmsLvCell_3

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 872 | 0x368 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| cell_8 | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_9 | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_10 | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |
| cell_12 | 48 | 16 | little_endian | False | 0.1 | 0 | None | None | mV | BMSLV |


#### BmsLvTemps

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 873 | 0x369 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| temp_0 | 0 | 16 | little_endian | False | 0.1 | 0 | None | None | C | BMSLV |
| temp_1 | 16 | 16 | little_endian | False | 0.1 | 0 | None | None | C | BMSLV |
| temp_2 | 32 | 16 | little_endian | False | 0.1 | 0 | None | None | C | BMSLV |
| temp_3 | 48 | 16 | little_endian | False | 0.1 | 0 | None | None | C | BMSLV |


#### Lem

| ID (Dec) | ID (Hex) | DLC |
|----------|----------|-----|
| 962 | 0x3C2 | 8 |

| Signal Name | Start Bit | Length | Byte Order | Value Type | Factor | Offset | Min | Max | Unit | Receiver |
|-------------|-----------|--------|------------|------------|--------|--------|-----|-----|------|----------|
| current | 7 | 32 | big_endian | False | 1 | -2147483648 | None | None | mA | VCU |


