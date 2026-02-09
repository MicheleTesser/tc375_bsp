#ifndef __CAR_TV_ALG__
#define __CAR_TV_ALG__

#include "../../../engines/engines.h"
#include <stdint.h>

struct TVInputArgs{
  float acc_x;                        /* '<Root>/m/s^2' */
  float acc_y;                        /* '<Root>/m/s^2' */
  float yaw_rate;                     /* '<Root>/omega_z */
  float throttle;                     /* '<Root>/throttle' */
  float brake;                        /* '<Root>/brake' */
  float steering_angle;               /* '<Root>/deg' */
  float current;                      /* '<Root>/lem' */
  float voltage;                      /* '<Root>/battery_pack_tension' */
  float limit_power;                  /* '<Root>/power limit KW' */
  float torque_max;                   /* '<Root>/power limit KW' */
  float torque_regen_max;             /* '<Root>/power limit KW' */
  float power_repartition;            /* '<Root>/power to rear' */
  float regen_max_current;            /* '<Root>/power to rear' */
  float Vcell_min;                    /* '<Root>/min volts hv' */
  uint8_t TV_active;                  /* '<Root>/bool'*/
  uint8_t TC_strat;                   /* '<Root>/nat'*/
  uint8_t REGEN_active;               /* '<Root>/bool'*/
  float rpm[4];                       /* '<Root>/rpm' */
};

void tv_alg_init(void);
void tv_alg_update(void);
void tv_alg_compute(const struct TVInputArgs* const restrict input,
        float o_posTorquesNM[__NUM_OF_ENGINES__]);

#endif // !__CAR_TV_ALG__
