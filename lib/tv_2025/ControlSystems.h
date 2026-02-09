/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ControlSystems_ONLYFRONT.h
 *
 * Code generated for Simulink model 'ControlSystems_ONLYFRONT'.
 *
 * Model version                  : 3.1213
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Sun Sep 14 13:11:02 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef ControlSystems_ONLYFRONT_h_
#define ControlSystems_ONLYFRONT_h_
#ifndef ControlSystems_ONLYFRONT_COMMON_INCLUDES_
#define ControlSystems_ONLYFRONT_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "complex_types.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                           /* ControlSystems_ONLYFRONT_COMMON_INCLUDES_ */

#include "ControlSystems_types.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  double rpm_fl;                       /* '<Root>/Saturation4' */
  double rpm_fr;                       /* '<Root>/Saturation5' */
  double rpm_rl;                       /* '<Root>/Saturation6' */
  double rpm_rr;                       /* '<Root>/Saturation7' */
  double regen_input;                  /* '<Root>/Saturation21' */
  double throttle;                     /* '<Root>/Saturation20' */
  double ZeroOrderHold;                /* '<Root>/Zero-Order Hold' */
  double RateLimiter1;                 /* '<S17>/Rate Limiter1' */
  double Saturation26;                 /* '<Root>/Saturation26' */
  double Saturation27;                 /* '<Root>/Saturation27' */
  double Product2;                     /* '<S532>/Product2' */
  double yr_e;                         /* '<S17>/Sum' */
  double YMC;                          /* '<S579>/Switch2' */
  double Product1[4];                  /* '<S17>/Product1' */
  double Switch2[4];                   /* '<S529>/Switch2' */
  double Torquetocut[4];               /* '<S14>/Product5' */
  double Product6;                     /* '<S14>/Product6' */
  double Product7;                     /* '<S14>/Product7' */
  double Constant2;                    /* '<S14>/Constant2' */
  double steering_rad;                 /* '<Root>/Discrete lowpass filter3' */
  double y;                            /* '<Root>/Discrete lowpass filter2' */
} B_ControlSystems_ONLYFRONT_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  dsp_simulink_MovingAverage_ControlSystems_ONLYFRONT_T obj;/* '<Root>/Moving Average' */
  double Delay4_DSTATE;                /* '<Root>/Delay4' */
  double Delay5_DSTATE;                /* '<Root>/Delay5' */
  double Delay_DSTATE;                 /* '<Root>/Delay' */
  double Delay3_DSTATE;                /* '<Root>/Delay3' */
  double Delay8_DSTATE;                /* '<Root>/Delay8' */
  double Delay9_DSTATE;                /* '<Root>/Delay9' */
  double Delay6_DSTATE;                /* '<Root>/Delay6' */
  double Delay7_DSTATE;                /* '<Root>/Delay7' */
  double Integrator_DSTATE;            /* '<S571>/Integrator' */
  double u;                            /* '<S183>/Integrator' */
  double u_o;                          /* '<S291>/Integrator' */
  double u_l;                          /* '<S129>/Integrator' */
  double u_c;                          /* '<S237>/Integrator' */
  double u_h;                          /* '<S403>/Integrator' */
  double u_k;                          /* '<S511>/Integrator' */
  double u_a;                          /* '<S349>/Integrator' */
  double u_e;                          /* '<S457>/Integrator' */
  double Delay10_DSTATE;               /* '<Root>/Delay10' */
  double Delay11_DSTATE;               /* '<Root>/Delay11' */
  double UnitDelay_DSTATE;             /* '<S85>/Unit Delay' */
  double Integrator_DSTATE_o;          /* '<S67>/Integrator' */
  double PrevY;                        /* '<S17>/Rate Limiter1' */
  int8_t Integrator_PrevResetState;    /* '<S571>/Integrator' */
  int8_t Integrator_PrevResetState_p;  /* '<S183>/Integrator' */
  int8_t Integrator_PrevResetState_ph; /* '<S291>/Integrator' */
  int8_t Integrator_PrevResetState_e;  /* '<S129>/Integrator' */
  int8_t Integrator_PrevResetState_n;  /* '<S237>/Integrator' */
  int8_t Integrator_PrevResetState_k;  /* '<S403>/Integrator' */
  int8_t Integrator_PrevResetState_m;  /* '<S511>/Integrator' */
  int8_t Integrator_PrevResetState_eh; /* '<S349>/Integrator' */
  int8_t Integrator_PrevResetState_en; /* '<S457>/Integrator' */
  int8_t Integrator_PrevResetState_k1; /* '<S67>/Integrator' */
} DW_ControlSystems_ONLYFRONT_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [21;21;21;21;21;21;21;21;21;21;21;21;21;20.99;20.567;19.098;17.83;16.711;15.728;14.854;14.072;13.369]
   * Referenced by: '<S21>/1-D Lookup Table'
   */
  double uDLookupTable_tableData[22];

  /* Expression: [0;1000;2000;3000;4000;5000;6000;7000;8000;9000;10000;11000;12000;12733;13000;14000;15000;16000;17000;18000;19000;20000]
   * Referenced by: '<S21>/1-D Lookup Table'
   */
  double uDLookupTable_bp01Data[22];

  /* Expression: [-24.0692973871389;-23.2664737895932;-22.474659773904;-21.6932187364136;-20.9215678059757;-20.159171683579;-19.4055373440555;-18.6602094695828;-17.9227664951065;-17.1928171683859;-16.4699975451084;-15.7539683536455;-15.0444126753349;-14.3410338952759;-13.6435538860417;-12.9517113927104;-12.2652605925586;-11.5839698067979;-10.9076203452181;-10.2360054672112;-9.568929445217259;-8.90620671835894;-8.24766112591246;-7.59312521148608;-6.94243959005989;-6.29545348780074;-5.65201971860814;-5.0120009931823;-4.37526491678785;-3.74168473188266;-3.1111389446424;-2.48351098069061;-1.8586888673538;-1.23656493984511;-0.617035569187983;-9.098497259516129e-07;0.614635334646622;1.22696612981894;1.83708131338014;2.44506778480954;3.05100968247918;3.65498854938862;4.25708348848345;4.85737130838379;5.45592666036902;6.05282216724504;6.6481285448369;7.24191471662515;7.83424792213388;8.42519381948693;9.01481742401911;9.603179837610099;10.1903433781389;10.7763683034049;11.361313730638;11.9452377127152;12.5281973111539;13.1102486660894;13.6914470634713;14.2718469997608;14.8515022442805;15.4304658994583;16.0087904591228;16.5865278650347;17.1637295618388;17.7404465505718;18.3167294408677;18.8926285020396;19.4681937131354;20.0434748121219;20.6185213443265]
   * Referenced by: '<S13>/FL wheel angle'
   */
  double FLwheelangle_tableData[71];

  /* Pooled Parameter (Expression: [-95;-92.285714285714306;-89.571428571428598;-86.857142857142904;-84.142857142857096;-81.428571428571402;-78.714285714285694;-76;-73.285714285714306;-70.571428571428598;-67.857142857142904;-65.142857142857096;-62.428571428571402;-59.714285714285701;-57;-54.285714285714299;-51.571428571428598;-48.857142857142897;-46.142857142857103;-43.428571428571402;-40.714285714285701;-38;-35.285714285714299;-32.571428571428598;-29.8571428571429;-27.1428571428571;-24.428571428571399;-21.714285714285701;-19;-16.285714285714299;-13.5714285714286;-10.857142857142801;-8.1428571428571406;-5.4285714285714297;-2.71428571428571;0;2.7142857142857202;5.4285714285714297;8.1428571428571495;10.8571428571429;13.5714285714286;16.285714285714299;19;21.714285714285701;24.428571428571399;27.142857142857199;29.8571428571429;32.571428571428598;35.285714285714299;38;40.714285714285701;43.428571428571402;46.142857142857103;48.857142857142897;51.571428571428598;54.285714285714299;57;59.714285714285701;62.428571428571402;65.142857142857096;67.857142857142904;70.571428571428598;73.285714285714306;76;78.714285714285694;81.428571428571402;84.142857142857096;86.857142857142904;89.571428571428598;92.285714285714306;95])
   * Referenced by:
   *   '<S13>/FL wheel angle'
   *   '<S13>/FR wheel angle'
   */
  double pooled23[71];

  /* Expression: [-20.6185213443621;-20.0434748121321;-19.4681937131465;-18.8926296886617;-18.3167306165596;-17.7404477161585;-17.1637307181743;-16.5865290129818;-16.0087915995554;-15.4304670332612;-14.851503372341;-14.2718481229884;-13.6914481827961;-13.1102497824618;-12.5281984255462;-11.9452388261265;-11.3613148441028;-10.7763694179971;-10.1903444949637;-9.60318095781877;-9.014818548811579;-8.425195789828519;-7.83424989875805;-7.24191670170052;-6.64813054062209;-6.05282417610405;-5.45592868477815;-4.85737335094636;-4.25708555193598;-3.65499063662774;-3.05101179656363;-2.44506992898729;-1.83708349109914;-1.22696834475615;-0.614637590728464;-1.39157083845216e-06;0.617033217928222;1.23656253391246;1.85868640154183;2.48351000918866;3.11113795378242;3.74168371989158;4.37526388176715;5.01199993311668;5.65201863133516;6.29545237103554;6.94243959004232;7.59312521146703;8.24766112589665;8.90620671834273;9.56892944519851;10.236005467192;10.907620345196;11.583969806779;12.2652605925357;12.9517113926858;13.6435538860117;14.3410338952451;15.0444126753022;15.7539683536138;16.4699975450722;17.1928171683479;17.9227664950655;18.6602094695371;19.4055373440032;20.1591716835239;20.9215678059124;21.693218736343;22.4746597738273;23.2664737895071;24.0692974052611]
   * Referenced by: '<S13>/FR wheel angle'
   */
  double FRwheelangle_tableData[71];
} ConstP_ControlSystems_ONLYFRONT_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  double steering_g;                   /* '<Root>/steering_g' */
  double throttle;                     /* '<Root>/throttle' */
  double brake;                        /* '<Root>/brake' */
  double rpm_fl;                       /* '<Root>/rpm_fl' */
  double rpm_fr;                       /* '<Root>/rpm_fr' */
  double rpm_rl;                       /* '<Root>/rpm_rl' */
  double rpm_rr;                       /* '<Root>/rpm_rr' */
  double acc_x;                        /* '<Root>/acc_x' */
  double acc_y;                        /* '<Root>/acc_y' */
  double yaw_rate;                     /* '<Root>/yaw_rate' */
  double battery_voltage;              /* '<Root>/battery_voltage' */
  double Vcell_min;                    /* '<Root>/Vcell_min' */
  double battery_current;              /* '<Root>/battery_current' */
  double TV_active;                    /* '<Root>/TV_active' */
  double Regen_active;                 /* '<Root>/Regen_active' */
  double LC_button;                    /* '<Root>/LC_button' */
} ExtU_ControlSystems_ONLYFRONT_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  double limitTorque[4];               /* '<Root>/limitTorque' */
  double torques[4];                   /* '<Root>/torques' */
  double csi_opt;                      /* '<Root>/csi_opt' */
  float yr_ref;                        /* '<Root>/yr_ref' */
  double Frontwheelsanglerad[2];       /* '<Root>/Front wheels angle [rad]' */
  double Fz[4];                        /* '<Root>/Fz' */
  double Regenpower;                   /* '<Root>/Regen power' */
  double Pemax;                        /* '<Root>/Pemax' */
  double NO_ePL_torques[4];            /* '<Root>/NO_ePL_torques' */
  float yaw_moment;                    /* '<Root>/yaw_moment' */
  float delta_torque[4];               /* '<Root>/delta_torque' */
  float vx;                            /* '<Root>/vx' */
  double negLimitTorque[4];            /* '<Root>/negLimitTorque' */
  double speed_setpoints[4];           /* '<Root>/speed_setpoints' */
  double Power_max_motor;              /* '<Root>/Power_max_motor' */
} ExtY_ControlSystems_ONLYFRONT_T;

/* Parameters (default storage) */
struct P_ControlSystems_ONLYFRONT_T_ {
  double ABS_P_F;                      /* Variable: ABS_P_F
                                        * Referenced by:
                                        *   '<S408>/Proportional Gain'
                                        *   '<S516>/Proportional Gain'
                                        */
  double ABS_P_R;                      /* Variable: ABS_P_R
                                        * Referenced by:
                                        *   '<S354>/Proportional Gain'
                                        *   '<S462>/Proportional Gain'
                                        */
  double ABS_SCALE;                    /* Variable: ABS_SCALE
                                        * Referenced by: '<S87>/Gain'
                                        */
  double ABS_TARGET_SLIP_F;            /* Variable: ABS_TARGET_SLIP_F
                                        * Referenced by: '<S87>/Desired slip front2'
                                        */
  double ABS_TARGET_SLIP_R;            /* Variable: ABS_TARGET_SLIP_R
                                        * Referenced by: '<S87>/Desired slip rear2'
                                        */
  double IN_ACC_X_LP_HZ;               /* Variable: IN_ACC_X_LP_HZ
                                        * Referenced by: '<Root>/Constant12'
                                        */
  double IN_ACC_Y_LP_HZ;               /* Variable: IN_ACC_Y_LP_HZ
                                        * Referenced by: '<Root>/Constant2'
                                        */
  double IN_BRAKE_MAP_EXP;             /* Variable: IN_BRAKE_MAP_EXP
                                        * Referenced by: '<Root>/Constant22'
                                        */
  double IN_STEERING_LP_HZ;            /* Variable: IN_STEERING_LP_HZ
                                        * Referenced by: '<Root>/Constant13'
                                        */
  double IN_THROTTLE_MAP_EXP;          /* Variable: IN_THROTTLE_MAP_EXP
                                        * Referenced by: '<Root>/Constant21'
                                        */
  double IN_YAW_RATE_LP_HZ;            /* Variable: IN_YAW_RATE_LP_HZ
                                        * Referenced by: '<Root>/Constant3'
                                        */
  double Imax_regen;                   /* Variable: Imax_regen
                                        * Referenced by: '<Root>/Constant'
                                        */
  double MAX_RPM;                      /* Variable: MAX_RPM
                                        * Referenced by: '<Root>/Saturation28'
                                        */
  double Pe_lim;                       /* Variable: Pe_lim
                                        * Referenced by:
                                        *   '<Root>/Saturation24'
                                        *   '<S12>/Constant'
                                        *   '<S26>/Switch'
                                        */
  double Pmax;                         /* Variable: Pmax
                                        * Referenced by: '<Root>/Constant24'
                                        */
  double REGEN_TYPE;                   /* Variable: REGEN_TYPE
                                        * Referenced by:
                                        *   '<S19>/Constant6'
                                        *   '<S19>/Constant7'
                                        *   '<S19>/Constant8'
                                        */
  double TC_CL_I_F;                    /* Variable: TC_CL_I_F
                                        * Referenced by:
                                        *   '<S180>/Integral Gain'
                                        *   '<S288>/Integral Gain'
                                        */
  double TC_CL_I_R;                    /* Variable: TC_CL_I_R
                                        * Referenced by:
                                        *   '<S126>/Integral Gain'
                                        *   '<S234>/Integral Gain'
                                        */
  double TC_CL_P_F;                    /* Variable: TC_CL_P_F
                                        * Referenced by:
                                        *   '<S188>/Proportional Gain'
                                        *   '<S296>/Proportional Gain'
                                        */
  double TC_CL_P_R;                    /* Variable: TC_CL_P_R
                                        * Referenced by:
                                        *   '<S134>/Proportional Gain'
                                        *   '<S242>/Proportional Gain'
                                        */
  double TC_CL_TARGET_SLIP_F;          /* Variable: TC_CL_TARGET_SLIP_F
                                        * Referenced by: '<S86>/Desired slip front2'
                                        */
  double TC_CL_TARGET_SLIP_R;          /* Variable: TC_CL_TARGET_SLIP_R
                                        * Referenced by: '<S86>/Desired slip rear2'
                                        */
  double TC_SCALE;                     /* Variable: TC_SCALE
                                        * Referenced by: '<S86>/Gain'
                                        */
  double TC_STRAT;                     /* Variable: TC_STRAT
                                        * Referenced by: '<S14>/Constant8'
                                        */
  double TV_ENABLE_YR_MULT;            /* Variable: TV_ENABLE_YR_MULT
                                        * Referenced by: '<S532>/Constant1'
                                        */
  double TV_HIGH_SPEED_THRESHOLD;      /* Variable: TV_HIGH_SPEED_THRESHOLD
                                        * Referenced by: '<S532>/MATLAB Function'
                                        */
  double TV_YMC_REPARTITION;           /* Variable: TV_YMC_REPARTITION
                                        * Referenced by: '<S533>/MATLAB Function'
                                        */
  double TV_YR_HIGH_SPEED_MULT;        /* Variable: TV_YR_HIGH_SPEED_MULT
                                        * Referenced by: '<S532>/MATLAB Function'
                                        */
  double TV_YR_LOW_SPEED_MULT;         /* Variable: TV_YR_LOW_SPEED_MULT
                                        * Referenced by: '<S532>/MATLAB Function'
                                        */
  double Tmax;                         /* Variable: Tmax
                                        * Referenced by:
                                        *   '<S6>/Constant3'
                                        *   '<S6>/Saturation1'
                                        */
  double Tmax_regen;                   /* Variable: Tmax_regen
                                        * Referenced by: '<Root>/Constant4'
                                        */
  double Vcell_lim;                    /* Variable: Vcell_lim
                                        * Referenced by: '<Root>/Constant23'
                                        */
  double YMC_TRQ_DELTA_LIMIT;          /* Variable: YMC_TRQ_DELTA_LIMIT
                                        * Referenced by:
                                        *   '<Root>/MATLAB Function1'
                                        *   '<Root>/MATLAB Function3'
                                        */
  double YMC_TRQ_DELTA_LIMIT_COASTING; /* Variable: YMC_TRQ_DELTA_LIMIT_COASTING
                                        * Referenced by:
                                        *   '<Root>/MATLAB Function1'
                                        *   '<Root>/MATLAB Function3'
                                        */
  double csi;                          /* Variable: csi
                                        * Referenced by: '<Root>/Constant14'
                                        */
  double ki_pl;                        /* Variable: ki_pl
                                        * Referenced by: '<S64>/Integral Gain'
                                        */
  double variable_csi;                 /* Variable: variable_csi
                                        * Referenced by: '<Root>/Constant15'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_ControlSystems_ONLYFRONT_T {
  const char * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint16_t TID[2];
    } TaskCounters;
  } Timing;
};

/* Block parameters (default storage) */
extern P_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_P;

/* Block signals (default storage) */
extern B_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_B;

/* Block states (default storage) */
extern DW_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_Y;

/* Constant parameters (default storage) */
extern const ConstP_ControlSystems_ONLYFRONT_T ControlSystems_ONLYFRONT_ConstP;

/* Model entry point functions */
extern void ControlSystems_ONLYFRONT_initialize(void);
extern void ControlSystems_ONLYFRONT_step(void);
extern void ControlSystems_ONLYFRONT_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ControlSystems_ONLYFRONT_T *const ControlSystems_ONLYFRONT_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Constant25' : Unused code path elimination
 * Block '<Root>/Divide4' : Unused code path elimination
 * Block '<S6>/Scope' : Unused code path elimination
 * Block '<S19>/Gain4' : Unused code path elimination
 * Block '<S19>/Scope' : Unused code path elimination
 * Block '<S19>/Scope1' : Unused code path elimination
 * Block '<S19>/Scope2' : Unused code path elimination
 * Block '<S21>/Scope1' : Unused code path elimination
 * Block '<Root>/Gain12' : Unused code path elimination
 * Block '<S11>/Abs' : Unused code path elimination
 * Block '<S11>/Abs1' : Unused code path elimination
 * Block '<S11>/Abs2' : Unused code path elimination
 * Block '<S11>/Add' : Unused code path elimination
 * Block '<S11>/Add1' : Unused code path elimination
 * Block '<S11>/Add2' : Unused code path elimination
 * Block '<S11>/Add3' : Unused code path elimination
 * Block '<S11>/Add4' : Unused code path elimination
 * Block '<S11>/Divide' : Unused code path elimination
 * Block '<S11>/Divide1' : Unused code path elimination
 * Block '<S11>/Divide2' : Unused code path elimination
 * Block '<S11>/Divide3' : Unused code path elimination
 * Block '<S11>/Gain' : Unused code path elimination
 * Block '<S11>/Gain1' : Unused code path elimination
 * Block '<S11>/Max' : Unused code path elimination
 * Block '<S11>/Max1' : Unused code path elimination
 * Block '<S11>/Min' : Unused code path elimination
 * Block '<S11>/Min1' : Unused code path elimination
 * Block '<S11>/Product' : Unused code path elimination
 * Block '<S11>/Product1' : Unused code path elimination
 * Block '<S11>/Product2' : Unused code path elimination
 * Block '<S11>/Product3' : Unused code path elimination
 * Block '<S11>/Saturation1' : Unused code path elimination
 * Block '<S11>/Saturation10' : Unused code path elimination
 * Block '<S11>/Saturation2' : Unused code path elimination
 * Block '<S11>/Saturation3' : Unused code path elimination
 * Block '<S11>/Saturation4' : Unused code path elimination
 * Block '<S11>/Saturation5' : Unused code path elimination
 * Block '<S11>/Saturation6' : Unused code path elimination
 * Block '<S11>/Saturation7' : Unused code path elimination
 * Block '<S11>/Saturation8' : Unused code path elimination
 * Block '<S11>/Saturation9' : Unused code path elimination
 * Block '<S11>/Scope' : Unused code path elimination
 * Block '<S11>/Scope1' : Unused code path elimination
 * Block '<S11>/Scope2' : Unused code path elimination
 * Block '<S11>/Scope3' : Unused code path elimination
 * Block '<S11>/Scope4' : Unused code path elimination
 * Block '<S11>/Scope5' : Unused code path elimination
 * Block '<S11>/Scope6' : Unused code path elimination
 * Block '<S11>/Sum of Elements' : Unused code path elimination
 * Block '<S11>/Sum of Elements1' : Unused code path elimination
 * Block '<S11>/Sum of Elements2' : Unused code path elimination
 * Block '<S11>/Sum of Elements3' : Unused code path elimination
 * Block '<S11>/Switch' : Unused code path elimination
 * Block '<Root>/Saturation' : Unused code path elimination
 * Block '<Root>/Scope' : Unused code path elimination
 * Block '<Root>/Scope10' : Unused code path elimination
 * Block '<Root>/Scope11' : Unused code path elimination
 * Block '<Root>/Scope16' : Unused code path elimination
 * Block '<Root>/Scope17' : Unused code path elimination
 * Block '<Root>/Scope2' : Unused code path elimination
 * Block '<Root>/Scope20' : Unused code path elimination
 * Block '<Root>/Scope21' : Unused code path elimination
 * Block '<Root>/Scope22' : Unused code path elimination
 * Block '<Root>/Scope24' : Unused code path elimination
 * Block '<Root>/Scope25' : Unused code path elimination
 * Block '<Root>/Scope3' : Unused code path elimination
 * Block '<Root>/Scope4' : Unused code path elimination
 * Block '<Root>/Scope9' : Unused code path elimination
 * Block '<Root>/Square' : Unused code path elimination
 * Block '<S29>/Data Type Duplicate' : Unused code path elimination
 * Block '<S29>/Data Type Propagation' : Unused code path elimination
 * Block '<Root>/Sum of Elements1' : Unused code path elimination
 * Block '<Root>/Sum of Elements2' : Unused code path elimination
 * Block '<Root>/Sum3' : Unused code path elimination
 * Block '<S86>/Scope3' : Unused code path elimination
 * Block '<S86>/Scope4' : Unused code path elimination
 * Block '<S86>/Sum' : Unused code path elimination
 * Block '<S87>/Scope3' : Unused code path elimination
 * Block '<S87>/Scope4' : Unused code path elimination
 * Block '<S14>/Scope' : Unused code path elimination
 * Block '<S14>/Scope1' : Unused code path elimination
 * Block '<S14>/Scope2' : Unused code path elimination
 * Block '<S14>/To Workspace' : Unused code path elimination
 * Block '<S14>/To Workspace1' : Unused code path elimination
 * Block '<S14>/To Workspace2' : Unused code path elimination
 * Block '<S529>/Data Type Duplicate' : Unused code path elimination
 * Block '<S529>/Data Type Propagation' : Unused code path elimination
 * Block '<S16>/Divide' : Unused code path elimination
 * Block '<S16>/Scope' : Unused code path elimination
 * Block '<S16>/Scope1' : Unused code path elimination
 * Block '<S16>/Scope2' : Unused code path elimination
 * Block '<S16>/Scope4' : Unused code path elimination
 * Block '<S16>/Sum' : Unused code path elimination
 * Block '<S17>/Constant1' : Unused code path elimination
 * Block '<S579>/Data Type Duplicate' : Unused code path elimination
 * Block '<S579>/Data Type Propagation' : Unused code path elimination
 * Block '<S17>/Scope' : Unused code path elimination
 * Block '<S17>/Scope1' : Unused code path elimination
 * Block '<Root>/Driver Demands' : Unused code path elimination
 * Block '<Root>/Motors RPM' : Unused code path elimination
 * Block '<Root>/PI Gains' : Unused code path elimination
 * Block '<Root>/Reference Tracking' : Unused code path elimination
 * Block '<Root>/Requested torques' : Unused code path elimination
 * Block '<Root>/Scope12' : Unused code path elimination
 * Block '<Root>/Scope5' : Unused code path elimination
 * Block '<Root>/Scope7' : Unused code path elimination
 * Block '<Root>/Scope8' : Unused code path elimination
 * Block '<Root>/Speed Estimation' : Unused code path elimination
 * Block '<Root>/Torque to cut' : Unused code path elimination
 * Block '<Root>/Yaw Rate' : Unused code path elimination
 * Block '<Root>/deltaTorque' : Unused code path elimination
 * Block '<S12>/Zero-Order Hold' : Eliminated since input and output rates are identical
 * Block '<S12>/Zero-Order Hold1' : Eliminated since input and output rates are identical
 * Block '<S16>/Gain6' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ControlSystems_ONLYFRONT'
 * '<S1>'   : 'ControlSystems_ONLYFRONT/Csi new'
 * '<S2>'   : 'ControlSystems_ONLYFRONT/Discrete lowpass filter'
 * '<S3>'   : 'ControlSystems_ONLYFRONT/Discrete lowpass filter1'
 * '<S4>'   : 'ControlSystems_ONLYFRONT/Discrete lowpass filter2'
 * '<S5>'   : 'ControlSystems_ONLYFRONT/Discrete lowpass filter3'
 * '<S6>'   : 'ControlSystems_ONLYFRONT/Driver torque request'
 * '<S7>'   : 'ControlSystems_ONLYFRONT/Effective radius estimation'
 * '<S8>'   : 'ControlSystems_ONLYFRONT/MATLAB Function'
 * '<S9>'   : 'ControlSystems_ONLYFRONT/MATLAB Function1'
 * '<S10>'  : 'ControlSystems_ONLYFRONT/MATLAB Function3'
 * '<S11>'  : 'ControlSystems_ONLYFRONT/Power Limitation'
 * '<S12>'  : 'ControlSystems_ONLYFRONT/Subsystem'
 * '<S13>'  : 'ControlSystems_ONLYFRONT/Subsystem1'
 * '<S14>'  : 'ControlSystems_ONLYFRONT/TC + ABS'
 * '<S15>'  : 'ControlSystems_ONLYFRONT/Torque limitation1'
 * '<S16>'  : 'ControlSystems_ONLYFRONT/Vertical loads calculator'
 * '<S17>'  : 'ControlSystems_ONLYFRONT/Yaw Moment Control'
 * '<S18>'  : 'ControlSystems_ONLYFRONT/discharge limit'
 * '<S19>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Subsystem'
 * '<S20>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Throttle cut'
 * '<S21>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Torque limits'
 * '<S22>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Subsystem/MATLAB Function'
 * '<S23>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Subsystem/MATLAB Function1'
 * '<S24>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Torque limits/MATLAB Function'
 * '<S25>'  : 'ControlSystems_ONLYFRONT/Driver torque request/Torque limits/MATLAB Function1'
 * '<S26>'  : 'ControlSystems_ONLYFRONT/Subsystem/ '
 * '<S27>'  : 'ControlSystems_ONLYFRONT/Subsystem/MATLAB Function'
 * '<S28>'  : 'ControlSystems_ONLYFRONT/Subsystem/Subsystem'
 * '<S29>'  : 'ControlSystems_ONLYFRONT/Subsystem/i'
 * '<S30>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller'
 * '<S31>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Anti-windup'
 * '<S32>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/D Gain'
 * '<S33>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/External Derivative'
 * '<S34>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Filter'
 * '<S35>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Filter ICs'
 * '<S36>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/I Gain'
 * '<S37>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Ideal P Gain'
 * '<S38>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S39>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Integrator'
 * '<S40>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Integrator ICs'
 * '<S41>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/N Copy'
 * '<S42>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/N Gain'
 * '<S43>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/P Copy'
 * '<S44>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Parallel P Gain'
 * '<S45>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Reset Signal'
 * '<S46>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Saturation'
 * '<S47>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Saturation Fdbk'
 * '<S48>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Sum'
 * '<S49>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Sum Fdbk'
 * '<S50>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tracking Mode'
 * '<S51>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tracking Mode Sum'
 * '<S52>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tsamp - Integral'
 * '<S53>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tsamp - Ngain'
 * '<S54>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/postSat Signal'
 * '<S55>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/preInt Signal'
 * '<S56>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/preSat Signal'
 * '<S57>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S58>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S59>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S60>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/D Gain/Disabled'
 * '<S61>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/External Derivative/Disabled'
 * '<S62>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Filter/Disabled'
 * '<S63>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Filter ICs/Disabled'
 * '<S64>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/I Gain/Internal Parameters'
 * '<S65>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S66>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S67>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Integrator/Discrete'
 * '<S68>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S69>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/N Copy/Disabled wSignal Specification'
 * '<S70>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/N Gain/Disabled'
 * '<S71>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/P Copy/Disabled'
 * '<S72>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S73>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Reset Signal/External Reset'
 * '<S74>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Saturation/Enabled'
 * '<S75>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S76>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Sum/Sum_PI'
 * '<S77>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S78>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tracking Mode/Disabled'
 * '<S79>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S80>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S81>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S82>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S83>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S84>'  : 'ControlSystems_ONLYFRONT/Subsystem/ /Discrete PID Controller/preSat Signal/Forward_Path'
 * '<S85>'  : 'ControlSystems_ONLYFRONT/Subsystem/Subsystem/Derivative'
 * '<S86>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC '
 * '<S87>'  : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS'
 * '<S88>'  : 'ControlSystems_ONLYFRONT/TC + ABS/Slip Calculation'
 * '<S89>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2'
 * '<S90>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4'
 * '<S91>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5'
 * '<S92>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6'
 * '<S93>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Anti-windup'
 * '<S94>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/D Gain'
 * '<S95>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/External Derivative'
 * '<S96>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Filter'
 * '<S97>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Filter ICs'
 * '<S98>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/I Gain'
 * '<S99>'  : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Ideal P Gain'
 * '<S100>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Ideal P Gain Fdbk'
 * '<S101>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Integrator'
 * '<S102>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Integrator ICs'
 * '<S103>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/N Copy'
 * '<S104>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/N Gain'
 * '<S105>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/P Copy'
 * '<S106>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Parallel P Gain'
 * '<S107>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Reset Signal'
 * '<S108>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Saturation'
 * '<S109>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Saturation Fdbk'
 * '<S110>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Sum'
 * '<S111>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Sum Fdbk'
 * '<S112>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tracking Mode'
 * '<S113>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tracking Mode Sum'
 * '<S114>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tsamp - Integral'
 * '<S115>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tsamp - Ngain'
 * '<S116>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/postSat Signal'
 * '<S117>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/preInt Signal'
 * '<S118>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/preSat Signal'
 * '<S119>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Anti-windup/Disc. Clamping Parallel'
 * '<S120>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S121>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S122>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/D Gain/Disabled'
 * '<S123>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/External Derivative/Disabled'
 * '<S124>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Filter/Disabled'
 * '<S125>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Filter ICs/Disabled'
 * '<S126>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/I Gain/Internal Parameters'
 * '<S127>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Ideal P Gain/Passthrough'
 * '<S128>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Ideal P Gain Fdbk/Disabled'
 * '<S129>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Integrator/Discrete'
 * '<S130>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Integrator ICs/Internal IC'
 * '<S131>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/N Copy/Disabled wSignal Specification'
 * '<S132>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/N Gain/Disabled'
 * '<S133>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/P Copy/Disabled'
 * '<S134>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Parallel P Gain/Internal Parameters'
 * '<S135>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Reset Signal/External Reset'
 * '<S136>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Saturation/Enabled'
 * '<S137>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Saturation Fdbk/Disabled'
 * '<S138>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Sum/Sum_PI'
 * '<S139>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Sum Fdbk/Disabled'
 * '<S140>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tracking Mode/Disabled'
 * '<S141>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tracking Mode Sum/Passthrough'
 * '<S142>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tsamp - Integral/TsSignalSpecification'
 * '<S143>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/Tsamp - Ngain/Passthrough'
 * '<S144>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/postSat Signal/Forward_Path'
 * '<S145>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/preInt Signal/Internal PreInt'
 * '<S146>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller2/preSat Signal/Forward_Path'
 * '<S147>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Anti-windup'
 * '<S148>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/D Gain'
 * '<S149>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/External Derivative'
 * '<S150>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Filter'
 * '<S151>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Filter ICs'
 * '<S152>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/I Gain'
 * '<S153>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Ideal P Gain'
 * '<S154>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Ideal P Gain Fdbk'
 * '<S155>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Integrator'
 * '<S156>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Integrator ICs'
 * '<S157>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/N Copy'
 * '<S158>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/N Gain'
 * '<S159>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/P Copy'
 * '<S160>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Parallel P Gain'
 * '<S161>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Reset Signal'
 * '<S162>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Saturation'
 * '<S163>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Saturation Fdbk'
 * '<S164>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Sum'
 * '<S165>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Sum Fdbk'
 * '<S166>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tracking Mode'
 * '<S167>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tracking Mode Sum'
 * '<S168>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tsamp - Integral'
 * '<S169>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tsamp - Ngain'
 * '<S170>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/postSat Signal'
 * '<S171>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/preInt Signal'
 * '<S172>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/preSat Signal'
 * '<S173>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Anti-windup/Disc. Clamping Parallel'
 * '<S174>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S175>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S176>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/D Gain/Disabled'
 * '<S177>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/External Derivative/Disabled'
 * '<S178>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Filter/Disabled'
 * '<S179>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Filter ICs/Disabled'
 * '<S180>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/I Gain/Internal Parameters'
 * '<S181>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Ideal P Gain/Passthrough'
 * '<S182>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Ideal P Gain Fdbk/Disabled'
 * '<S183>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Integrator/Discrete'
 * '<S184>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Integrator ICs/Internal IC'
 * '<S185>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/N Copy/Disabled wSignal Specification'
 * '<S186>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/N Gain/Disabled'
 * '<S187>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/P Copy/Disabled'
 * '<S188>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Parallel P Gain/Internal Parameters'
 * '<S189>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Reset Signal/External Reset'
 * '<S190>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Saturation/Enabled'
 * '<S191>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Saturation Fdbk/Disabled'
 * '<S192>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Sum/Sum_PI'
 * '<S193>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Sum Fdbk/Disabled'
 * '<S194>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tracking Mode/Disabled'
 * '<S195>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tracking Mode Sum/Passthrough'
 * '<S196>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tsamp - Integral/TsSignalSpecification'
 * '<S197>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/Tsamp - Ngain/Passthrough'
 * '<S198>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/postSat Signal/Forward_Path'
 * '<S199>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/preInt Signal/Internal PreInt'
 * '<S200>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller4/preSat Signal/Forward_Path'
 * '<S201>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Anti-windup'
 * '<S202>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/D Gain'
 * '<S203>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/External Derivative'
 * '<S204>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Filter'
 * '<S205>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Filter ICs'
 * '<S206>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/I Gain'
 * '<S207>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Ideal P Gain'
 * '<S208>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Ideal P Gain Fdbk'
 * '<S209>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Integrator'
 * '<S210>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Integrator ICs'
 * '<S211>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/N Copy'
 * '<S212>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/N Gain'
 * '<S213>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/P Copy'
 * '<S214>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Parallel P Gain'
 * '<S215>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Reset Signal'
 * '<S216>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Saturation'
 * '<S217>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Saturation Fdbk'
 * '<S218>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Sum'
 * '<S219>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Sum Fdbk'
 * '<S220>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tracking Mode'
 * '<S221>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tracking Mode Sum'
 * '<S222>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tsamp - Integral'
 * '<S223>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tsamp - Ngain'
 * '<S224>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/postSat Signal'
 * '<S225>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/preInt Signal'
 * '<S226>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/preSat Signal'
 * '<S227>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Anti-windup/Disc. Clamping Parallel'
 * '<S228>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S229>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S230>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/D Gain/Disabled'
 * '<S231>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/External Derivative/Disabled'
 * '<S232>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Filter/Disabled'
 * '<S233>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Filter ICs/Disabled'
 * '<S234>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/I Gain/Internal Parameters'
 * '<S235>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Ideal P Gain/Passthrough'
 * '<S236>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Ideal P Gain Fdbk/Disabled'
 * '<S237>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Integrator/Discrete'
 * '<S238>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Integrator ICs/Internal IC'
 * '<S239>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/N Copy/Disabled wSignal Specification'
 * '<S240>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/N Gain/Disabled'
 * '<S241>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/P Copy/Disabled'
 * '<S242>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Parallel P Gain/Internal Parameters'
 * '<S243>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Reset Signal/External Reset'
 * '<S244>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Saturation/Enabled'
 * '<S245>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Saturation Fdbk/Disabled'
 * '<S246>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Sum/Sum_PI'
 * '<S247>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Sum Fdbk/Disabled'
 * '<S248>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tracking Mode/Disabled'
 * '<S249>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tracking Mode Sum/Passthrough'
 * '<S250>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tsamp - Integral/TsSignalSpecification'
 * '<S251>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/Tsamp - Ngain/Passthrough'
 * '<S252>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/postSat Signal/Forward_Path'
 * '<S253>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/preInt Signal/Internal PreInt'
 * '<S254>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller5/preSat Signal/Forward_Path'
 * '<S255>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Anti-windup'
 * '<S256>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/D Gain'
 * '<S257>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/External Derivative'
 * '<S258>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Filter'
 * '<S259>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Filter ICs'
 * '<S260>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/I Gain'
 * '<S261>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Ideal P Gain'
 * '<S262>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Ideal P Gain Fdbk'
 * '<S263>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Integrator'
 * '<S264>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Integrator ICs'
 * '<S265>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/N Copy'
 * '<S266>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/N Gain'
 * '<S267>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/P Copy'
 * '<S268>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Parallel P Gain'
 * '<S269>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Reset Signal'
 * '<S270>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Saturation'
 * '<S271>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Saturation Fdbk'
 * '<S272>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Sum'
 * '<S273>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Sum Fdbk'
 * '<S274>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tracking Mode'
 * '<S275>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tracking Mode Sum'
 * '<S276>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tsamp - Integral'
 * '<S277>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tsamp - Ngain'
 * '<S278>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/postSat Signal'
 * '<S279>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/preInt Signal'
 * '<S280>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/preSat Signal'
 * '<S281>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Anti-windup/Disc. Clamping Parallel'
 * '<S282>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S283>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S284>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/D Gain/Disabled'
 * '<S285>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/External Derivative/Disabled'
 * '<S286>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Filter/Disabled'
 * '<S287>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Filter ICs/Disabled'
 * '<S288>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/I Gain/Internal Parameters'
 * '<S289>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Ideal P Gain/Passthrough'
 * '<S290>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Ideal P Gain Fdbk/Disabled'
 * '<S291>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Integrator/Discrete'
 * '<S292>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Integrator ICs/Internal IC'
 * '<S293>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/N Copy/Disabled wSignal Specification'
 * '<S294>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/N Gain/Disabled'
 * '<S295>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/P Copy/Disabled'
 * '<S296>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Parallel P Gain/Internal Parameters'
 * '<S297>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Reset Signal/External Reset'
 * '<S298>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Saturation/Enabled'
 * '<S299>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Saturation Fdbk/Disabled'
 * '<S300>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Sum/Sum_PI'
 * '<S301>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Sum Fdbk/Disabled'
 * '<S302>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tracking Mode/Disabled'
 * '<S303>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tracking Mode Sum/Passthrough'
 * '<S304>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tsamp - Integral/TsSignalSpecification'
 * '<S305>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/Tsamp - Ngain/Passthrough'
 * '<S306>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/postSat Signal/Forward_Path'
 * '<S307>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/preInt Signal/Internal PreInt'
 * '<S308>' : 'ControlSystems_ONLYFRONT/TC + ABS/CLTC /PID Controller6/preSat Signal/Forward_Path'
 * '<S309>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2'
 * '<S310>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4'
 * '<S311>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5'
 * '<S312>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6'
 * '<S313>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Anti-windup'
 * '<S314>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/D Gain'
 * '<S315>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/External Derivative'
 * '<S316>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Filter'
 * '<S317>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Filter ICs'
 * '<S318>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/I Gain'
 * '<S319>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Ideal P Gain'
 * '<S320>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Ideal P Gain Fdbk'
 * '<S321>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Integrator'
 * '<S322>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Integrator ICs'
 * '<S323>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/N Copy'
 * '<S324>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/N Gain'
 * '<S325>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/P Copy'
 * '<S326>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Parallel P Gain'
 * '<S327>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Reset Signal'
 * '<S328>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Saturation'
 * '<S329>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Saturation Fdbk'
 * '<S330>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Sum'
 * '<S331>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Sum Fdbk'
 * '<S332>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tracking Mode'
 * '<S333>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tracking Mode Sum'
 * '<S334>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tsamp - Integral'
 * '<S335>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tsamp - Ngain'
 * '<S336>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/postSat Signal'
 * '<S337>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/preInt Signal'
 * '<S338>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/preSat Signal'
 * '<S339>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Anti-windup/Disc. Clamping Parallel'
 * '<S340>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S341>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S342>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/D Gain/Disabled'
 * '<S343>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/External Derivative/Disabled'
 * '<S344>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Filter/Disabled'
 * '<S345>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Filter ICs/Disabled'
 * '<S346>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/I Gain/Internal Parameters'
 * '<S347>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Ideal P Gain/Passthrough'
 * '<S348>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Ideal P Gain Fdbk/Disabled'
 * '<S349>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Integrator/Discrete'
 * '<S350>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Integrator ICs/Internal IC'
 * '<S351>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/N Copy/Disabled wSignal Specification'
 * '<S352>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/N Gain/Disabled'
 * '<S353>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/P Copy/Disabled'
 * '<S354>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Parallel P Gain/Internal Parameters'
 * '<S355>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Reset Signal/External Reset'
 * '<S356>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Saturation/Enabled'
 * '<S357>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Saturation Fdbk/Disabled'
 * '<S358>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Sum/Sum_PI'
 * '<S359>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Sum Fdbk/Disabled'
 * '<S360>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tracking Mode/Disabled'
 * '<S361>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tracking Mode Sum/Passthrough'
 * '<S362>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tsamp - Integral/TsSignalSpecification'
 * '<S363>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/Tsamp - Ngain/Passthrough'
 * '<S364>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/postSat Signal/Forward_Path'
 * '<S365>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/preInt Signal/Internal PreInt'
 * '<S366>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller2/preSat Signal/Forward_Path'
 * '<S367>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Anti-windup'
 * '<S368>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/D Gain'
 * '<S369>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/External Derivative'
 * '<S370>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Filter'
 * '<S371>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Filter ICs'
 * '<S372>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/I Gain'
 * '<S373>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Ideal P Gain'
 * '<S374>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Ideal P Gain Fdbk'
 * '<S375>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Integrator'
 * '<S376>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Integrator ICs'
 * '<S377>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/N Copy'
 * '<S378>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/N Gain'
 * '<S379>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/P Copy'
 * '<S380>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Parallel P Gain'
 * '<S381>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Reset Signal'
 * '<S382>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Saturation'
 * '<S383>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Saturation Fdbk'
 * '<S384>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Sum'
 * '<S385>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Sum Fdbk'
 * '<S386>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tracking Mode'
 * '<S387>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tracking Mode Sum'
 * '<S388>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tsamp - Integral'
 * '<S389>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tsamp - Ngain'
 * '<S390>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/postSat Signal'
 * '<S391>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/preInt Signal'
 * '<S392>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/preSat Signal'
 * '<S393>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Anti-windup/Disc. Clamping Parallel'
 * '<S394>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S395>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S396>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/D Gain/Disabled'
 * '<S397>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/External Derivative/Disabled'
 * '<S398>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Filter/Disabled'
 * '<S399>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Filter ICs/Disabled'
 * '<S400>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/I Gain/Internal Parameters'
 * '<S401>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Ideal P Gain/Passthrough'
 * '<S402>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Ideal P Gain Fdbk/Disabled'
 * '<S403>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Integrator/Discrete'
 * '<S404>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Integrator ICs/Internal IC'
 * '<S405>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/N Copy/Disabled wSignal Specification'
 * '<S406>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/N Gain/Disabled'
 * '<S407>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/P Copy/Disabled'
 * '<S408>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Parallel P Gain/Internal Parameters'
 * '<S409>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Reset Signal/External Reset'
 * '<S410>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Saturation/Enabled'
 * '<S411>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Saturation Fdbk/Disabled'
 * '<S412>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Sum/Sum_PI'
 * '<S413>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Sum Fdbk/Disabled'
 * '<S414>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tracking Mode/Disabled'
 * '<S415>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tracking Mode Sum/Passthrough'
 * '<S416>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tsamp - Integral/TsSignalSpecification'
 * '<S417>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/Tsamp - Ngain/Passthrough'
 * '<S418>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/postSat Signal/Forward_Path'
 * '<S419>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/preInt Signal/Internal PreInt'
 * '<S420>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller4/preSat Signal/Forward_Path'
 * '<S421>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Anti-windup'
 * '<S422>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/D Gain'
 * '<S423>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/External Derivative'
 * '<S424>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Filter'
 * '<S425>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Filter ICs'
 * '<S426>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/I Gain'
 * '<S427>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Ideal P Gain'
 * '<S428>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Ideal P Gain Fdbk'
 * '<S429>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Integrator'
 * '<S430>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Integrator ICs'
 * '<S431>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/N Copy'
 * '<S432>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/N Gain'
 * '<S433>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/P Copy'
 * '<S434>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Parallel P Gain'
 * '<S435>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Reset Signal'
 * '<S436>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Saturation'
 * '<S437>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Saturation Fdbk'
 * '<S438>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Sum'
 * '<S439>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Sum Fdbk'
 * '<S440>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tracking Mode'
 * '<S441>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tracking Mode Sum'
 * '<S442>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tsamp - Integral'
 * '<S443>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tsamp - Ngain'
 * '<S444>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/postSat Signal'
 * '<S445>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/preInt Signal'
 * '<S446>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/preSat Signal'
 * '<S447>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Anti-windup/Disc. Clamping Parallel'
 * '<S448>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S449>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S450>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/D Gain/Disabled'
 * '<S451>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/External Derivative/Disabled'
 * '<S452>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Filter/Disabled'
 * '<S453>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Filter ICs/Disabled'
 * '<S454>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/I Gain/Internal Parameters'
 * '<S455>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Ideal P Gain/Passthrough'
 * '<S456>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Ideal P Gain Fdbk/Disabled'
 * '<S457>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Integrator/Discrete'
 * '<S458>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Integrator ICs/Internal IC'
 * '<S459>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/N Copy/Disabled wSignal Specification'
 * '<S460>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/N Gain/Disabled'
 * '<S461>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/P Copy/Disabled'
 * '<S462>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Parallel P Gain/Internal Parameters'
 * '<S463>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Reset Signal/External Reset'
 * '<S464>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Saturation/Enabled'
 * '<S465>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Saturation Fdbk/Disabled'
 * '<S466>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Sum/Sum_PI'
 * '<S467>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Sum Fdbk/Disabled'
 * '<S468>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tracking Mode/Disabled'
 * '<S469>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tracking Mode Sum/Passthrough'
 * '<S470>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tsamp - Integral/TsSignalSpecification'
 * '<S471>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/Tsamp - Ngain/Passthrough'
 * '<S472>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/postSat Signal/Forward_Path'
 * '<S473>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/preInt Signal/Internal PreInt'
 * '<S474>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller5/preSat Signal/Forward_Path'
 * '<S475>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Anti-windup'
 * '<S476>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/D Gain'
 * '<S477>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/External Derivative'
 * '<S478>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Filter'
 * '<S479>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Filter ICs'
 * '<S480>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/I Gain'
 * '<S481>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Ideal P Gain'
 * '<S482>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Ideal P Gain Fdbk'
 * '<S483>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Integrator'
 * '<S484>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Integrator ICs'
 * '<S485>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/N Copy'
 * '<S486>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/N Gain'
 * '<S487>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/P Copy'
 * '<S488>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Parallel P Gain'
 * '<S489>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Reset Signal'
 * '<S490>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Saturation'
 * '<S491>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Saturation Fdbk'
 * '<S492>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Sum'
 * '<S493>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Sum Fdbk'
 * '<S494>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tracking Mode'
 * '<S495>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tracking Mode Sum'
 * '<S496>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tsamp - Integral'
 * '<S497>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tsamp - Ngain'
 * '<S498>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/postSat Signal'
 * '<S499>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/preInt Signal'
 * '<S500>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/preSat Signal'
 * '<S501>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Anti-windup/Disc. Clamping Parallel'
 * '<S502>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S503>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S504>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/D Gain/Disabled'
 * '<S505>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/External Derivative/Disabled'
 * '<S506>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Filter/Disabled'
 * '<S507>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Filter ICs/Disabled'
 * '<S508>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/I Gain/Internal Parameters'
 * '<S509>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Ideal P Gain/Passthrough'
 * '<S510>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Ideal P Gain Fdbk/Disabled'
 * '<S511>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Integrator/Discrete'
 * '<S512>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Integrator ICs/Internal IC'
 * '<S513>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/N Copy/Disabled wSignal Specification'
 * '<S514>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/N Gain/Disabled'
 * '<S515>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/P Copy/Disabled'
 * '<S516>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Parallel P Gain/Internal Parameters'
 * '<S517>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Reset Signal/External Reset'
 * '<S518>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Saturation/Enabled'
 * '<S519>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Saturation Fdbk/Disabled'
 * '<S520>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Sum/Sum_PI'
 * '<S521>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Sum Fdbk/Disabled'
 * '<S522>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tracking Mode/Disabled'
 * '<S523>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tracking Mode Sum/Passthrough'
 * '<S524>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tsamp - Integral/TsSignalSpecification'
 * '<S525>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/Tsamp - Ngain/Passthrough'
 * '<S526>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/postSat Signal/Forward_Path'
 * '<S527>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/preInt Signal/Internal PreInt'
 * '<S528>' : 'ControlSystems_ONLYFRONT/TC + ABS/REGEN ABS/PID Controller6/preSat Signal/Forward_Path'
 * '<S529>' : 'ControlSystems_ONLYFRONT/Torque limitation1/Saturation Dynamic'
 * '<S530>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/MATLAB Function'
 * '<S531>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller'
 * '<S532>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/Reference YR generaator'
 * '<S533>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/Torque distribution'
 * '<S534>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Anti-windup'
 * '<S535>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/D Gain'
 * '<S536>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/External Derivative'
 * '<S537>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Filter'
 * '<S538>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Filter ICs'
 * '<S539>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/I Gain'
 * '<S540>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Ideal P Gain'
 * '<S541>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Ideal P Gain Fdbk'
 * '<S542>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Integrator'
 * '<S543>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Integrator ICs'
 * '<S544>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/N Copy'
 * '<S545>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/N Gain'
 * '<S546>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/P Copy'
 * '<S547>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Parallel P Gain'
 * '<S548>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Reset Signal'
 * '<S549>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Saturation'
 * '<S550>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Saturation Fdbk'
 * '<S551>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Sum'
 * '<S552>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Sum Fdbk'
 * '<S553>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tracking Mode'
 * '<S554>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tracking Mode Sum'
 * '<S555>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tsamp - Integral'
 * '<S556>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tsamp - Ngain'
 * '<S557>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/postSat Signal'
 * '<S558>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/preInt Signal'
 * '<S559>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/preSat Signal'
 * '<S560>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S561>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S562>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External'
 * '<S563>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/External/Dead Zone Dynamic'
 * '<S564>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/D Gain/Disabled'
 * '<S565>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/External Derivative/Disabled'
 * '<S566>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Filter/Disabled'
 * '<S567>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Filter ICs/Disabled'
 * '<S568>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/I Gain/External Parameters'
 * '<S569>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Ideal P Gain/Passthrough'
 * '<S570>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S571>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Integrator/Discrete'
 * '<S572>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Integrator ICs/Internal IC'
 * '<S573>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/N Copy/Disabled wSignal Specification'
 * '<S574>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/N Gain/Disabled'
 * '<S575>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/P Copy/Disabled'
 * '<S576>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Parallel P Gain/External Parameters'
 * '<S577>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Reset Signal/External Reset'
 * '<S578>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Saturation/External'
 * '<S579>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Saturation/External/Saturation Dynamic'
 * '<S580>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Saturation Fdbk/Disabled'
 * '<S581>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Sum/Sum_PI'
 * '<S582>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Sum Fdbk/Disabled'
 * '<S583>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tracking Mode/Disabled'
 * '<S584>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S585>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S586>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S587>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/postSat Signal/Forward_Path'
 * '<S588>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/preInt Signal/Internal PreInt'
 * '<S589>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/PID Controller/preSat Signal/Forward_Path'
 * '<S590>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/Reference YR generaator/MATLAB Function'
 * '<S591>' : 'ControlSystems_ONLYFRONT/Yaw Moment Control/Torque distribution/MATLAB Function'
 */
#endif                                 /* ControlSystems_ONLYFRONT_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
