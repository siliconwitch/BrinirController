/*
 * File: fuzzyController.c
 *
 * Code generated for Simulink model 'fuzzyController'.
 *
 * Model version                  : 1.23
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Wed Jan 28 20:09:45 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#include "fuzzyController.h"

/* Block signals (auto storage) */
B_fuzzyController_T fuzzyController_B;

/* External inputs (root inport signals with auto storage) */
ExtU_fuzzyController_T fuzzyController_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_fuzzyController_T fuzzyController_Y;
extern void fuzzyController_Actionu1(real_T rtu_0, real_T *rty_u2);
extern void fuzzyControl_IfActionSubsystem3(real_T rtu_x, real_T *rty_Out1,
  P_IfActionSubsystem3_fuzzyCon_T *localP);
extern void fuzzyControl_IfActionSubsystem2(real_T rtu_x, real_T *rty_Out1,
  P_IfActionSubsystem2_fuzzyCon_T *localP);

/*
 * Output and update for action system:
 *    '<S12>/Action: u1'
 *    '<S13>/Action: u1'
 *    '<S14>/Action: u1'
 *    '<S15>/Action: u1'
 *    '<S107>/Action: u1'
 */
void fuzzyController_Actionu1(real_T rtu_0, real_T *rty_u2)
{
  /* Inport: '<S33>/u1' */
  *rty_u2 = rtu_0;
}

/*
 * Output and update for action system:
 *    '<S40>/If Action Subsystem3'
 *    '<S41>/If Action Subsystem3'
 *    '<S42>/If Action Subsystem3'
 *    '<S55>/If Action Subsystem3'
 *    '<S56>/If Action Subsystem3'
 *    '<S65>/If Action Subsystem3'
 *    '<S66>/If Action Subsystem3'
 *    '<S67>/If Action Subsystem3'
 *    '<S80>/If Action Subsystem3'
 *    '<S81>/If Action Subsystem3'
 *    ...
 */
void fuzzyControl_IfActionSubsystem3(real_T rtu_x, real_T *rty_Out1,
  P_IfActionSubsystem3_fuzzyCon_T *localP)
{
  /* Product: '<S46>/Product ab (trimf)' incorporates:
   *  Constant: '<S46>/a'
   *  Constant: '<S46>/b'
   *  Sum: '<S46>/Sum'
   *  Sum: '<S46>/Sum1'
   */
  *rty_Out1 = (rtu_x - localP->a_Value) / (localP->b_Value - localP->a_Value);
}

/*
 * Output and update for action system:
 *    '<S40>/If Action Subsystem2'
 *    '<S41>/If Action Subsystem2'
 *    '<S42>/If Action Subsystem2'
 *    '<S55>/If Action Subsystem2'
 *    '<S56>/If Action Subsystem2'
 *    '<S65>/If Action Subsystem2'
 *    '<S66>/If Action Subsystem2'
 *    '<S67>/If Action Subsystem2'
 *    '<S80>/If Action Subsystem2'
 *    '<S81>/If Action Subsystem2'
 *    ...
 */
void fuzzyControl_IfActionSubsystem2(real_T rtu_x, real_T *rty_Out1,
  P_IfActionSubsystem2_fuzzyCon_T *localP)
{
  /* Product: '<S45>/Product cd (trimf)' incorporates:
   *  Constant: '<S45>/b'
   *  Constant: '<S45>/c'
   *  Sum: '<S45>/Sum2'
   *  Sum: '<S45>/Sum3'
   */
  *rty_Out1 = 1.0 / (localP->c_Value - localP->b_Value) * (localP->c_Value -
    rtu_x);
}

/* Model step function */
void fuzzyController_step(void)
{
  /* local block i/o variables */
  real_T rtb_Merge_ch;
  real_T rtb_Merge_ic;
  real_T rtb_Merge_cf;
  real_T rtb_Merge_j2;
  real_T rtb_Merge_kn;
  real_T rtb_Merge_ok;
  real_T rtb_Merge_cy;
  int32_T i;

  /* Product: '<S1>/Product' incorporates:
   *  Inport: '<Root>/gyroGain'
   *  Inport: '<Root>/gyroYaw'
   */
  fuzzyController_B.Weighting_a = fuzzyController_U.gyroYaw *
    fuzzyController_U.gyroGain;

  /* Saturate: '<S1>/Saturation5' incorporates:
   *  Inport: '<Root>/steeringSignal'
   */
  if (fuzzyController_B.Weighting_a >= fuzzyController_P.Saturation5_UpperSat) {
    fuzzyController_B.Saturation5[0] = fuzzyController_P.Saturation5_UpperSat;
  } else if (fuzzyController_B.Weighting_a <=
             fuzzyController_P.Saturation5_LowerSat) {
    fuzzyController_B.Saturation5[0] = fuzzyController_P.Saturation5_LowerSat;
  } else {
    fuzzyController_B.Saturation5[0] = fuzzyController_B.Weighting_a;
  }

  if (fuzzyController_U.steeringSignal >= fuzzyController_P.Saturation5_UpperSat)
  {
    fuzzyController_B.Saturation5[1] = fuzzyController_P.Saturation5_UpperSat;
  } else if (fuzzyController_U.steeringSignal <=
             fuzzyController_P.Saturation5_LowerSat) {
    fuzzyController_B.Saturation5[1] = fuzzyController_P.Saturation5_LowerSat;
  } else {
    fuzzyController_B.Saturation5[1] = fuzzyController_U.steeringSignal;
  }

  /* End of Saturate: '<S1>/Saturation5' */

  /* If: '<S127>/If' incorporates:
   *  Constant: '<S129>/0'
   *  Constant: '<S130>/0'
   */
  if ((fuzzyController_B.Saturation5[1] < -3.0) ||
      (fuzzyController_B.Saturation5[1] > 1.0)) {
    /* Outputs for IfAction SubSystem: '<S127>/If Action Subsystem' incorporates:
     *  ActionPort: '<S129>/Action Port'
     */
    fuzzyController_B.Merge = fuzzyController_P._Value_ho;

    /* End of Outputs for SubSystem: '<S127>/If Action Subsystem' */
  } else if (fuzzyController_B.Saturation5[1] == -1.0) {
    /* Outputs for IfAction SubSystem: '<S127>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S130>/Action Port'
     */
    fuzzyController_B.Merge = fuzzyController_P._Value_g;

    /* End of Outputs for SubSystem: '<S127>/If Action Subsystem1' */
  } else if (fuzzyController_B.Saturation5[1] < -1.0) {
    /* Outputs for IfAction SubSystem: '<S127>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S132>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Saturation5[1],
      &fuzzyController_B.Merge, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3);

    /* End of Outputs for SubSystem: '<S127>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S127>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S131>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Saturation5[1],
      &fuzzyController_B.Merge, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2);

    /* End of Outputs for SubSystem: '<S127>/If Action Subsystem2' */
  }

  /* End of If: '<S127>/If' */

  /* Product: '<S108>/Weighting' incorporates:
   *  Constant: '<S108>/Weight'
   *  Product: '<S108>/andorMethod'
   */
  fuzzyController_B.Weighting_a = fuzzyController_P.Weight_Value *
    fuzzyController_B.Merge;

  /* Product: '<S108>/impMethod' incorporates:
   *  Constant: '<S113>/left'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod[i] = fuzzyController_B.Weighting_a *
      fuzzyController_P.left_Value[i];
  }

  /* End of Product: '<S108>/impMethod' */

  /* If: '<S128>/If' incorporates:
   *  Constant: '<S133>/0'
   *  Constant: '<S134>/0'
   */
  if ((fuzzyController_B.Saturation5[1] < -1.0) ||
      (fuzzyController_B.Saturation5[1] > 3.0)) {
    /* Outputs for IfAction SubSystem: '<S128>/If Action Subsystem' incorporates:
     *  ActionPort: '<S133>/Action Port'
     */
    fuzzyController_B.Merge_b = fuzzyController_P._Value_pu;

    /* End of Outputs for SubSystem: '<S128>/If Action Subsystem' */
  } else if (fuzzyController_B.Saturation5[1] == 1.0) {
    /* Outputs for IfAction SubSystem: '<S128>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S134>/Action Port'
     */
    fuzzyController_B.Merge_b = fuzzyController_P._Value_hv;

    /* End of Outputs for SubSystem: '<S128>/If Action Subsystem1' */
  } else if (fuzzyController_B.Saturation5[1] < 1.0) {
    /* Outputs for IfAction SubSystem: '<S128>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S136>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Saturation5[1],
      &fuzzyController_B.Merge_b, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_c);

    /* End of Outputs for SubSystem: '<S128>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S128>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S135>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Saturation5[1],
      &fuzzyController_B.Merge_b, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_j);

    /* End of Outputs for SubSystem: '<S128>/If Action Subsystem2' */
  }

  /* End of If: '<S128>/If' */

  /* Product: '<S109>/Weighting' incorporates:
   *  Constant: '<S109>/Weight'
   *  Product: '<S109>/andorMethod'
   */
  fuzzyController_B.Weighting_i = fuzzyController_P.Weight_Value_l *
    fuzzyController_B.Merge_b;

  /* Product: '<S109>/impMethod' incorporates:
   *  Constant: '<S113>/right'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_a[i] = fuzzyController_B.Weighting_i *
      fuzzyController_P.right_Value[i];
  }

  /* End of Product: '<S109>/impMethod' */

  /* If: '<S118>/If' incorporates:
   *  Constant: '<S123>/0'
   *  Constant: '<S124>/0'
   */
  if ((fuzzyController_B.Saturation5[0] < -2.0) ||
      (fuzzyController_B.Saturation5[0] > 0.0)) {
    /* Outputs for IfAction SubSystem: '<S118>/If Action Subsystem' incorporates:
     *  ActionPort: '<S123>/Action Port'
     */
    fuzzyController_B.Merge_k = fuzzyController_P._Value_i1;

    /* End of Outputs for SubSystem: '<S118>/If Action Subsystem' */
  } else if (fuzzyController_B.Saturation5[0] == -1.0) {
    /* Outputs for IfAction SubSystem: '<S118>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S124>/Action Port'
     */
    fuzzyController_B.Merge_k = fuzzyController_P._Value_l5;

    /* End of Outputs for SubSystem: '<S118>/If Action Subsystem1' */
  } else if (fuzzyController_B.Saturation5[0] < -1.0) {
    /* Outputs for IfAction SubSystem: '<S118>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S126>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Saturation5[0],
      &fuzzyController_B.Merge_k, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_k);

    /* End of Outputs for SubSystem: '<S118>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S118>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S125>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Saturation5[0],
      &fuzzyController_B.Merge_k, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_o);

    /* End of Outputs for SubSystem: '<S118>/If Action Subsystem2' */
  }

  /* End of If: '<S118>/If' */

  /* Product: '<S110>/Weighting' incorporates:
   *  Constant: '<S110>/Weight'
   *  Product: '<S110>/andorMethod'
   */
  fuzzyController_B.Weighting_k = fuzzyController_B.Merge_k *
    fuzzyController_B.Merge * fuzzyController_P.Weight_Value_f;

  /* Product: '<S110>/impMethod' incorporates:
   *  Constant: '<S113>/left'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_k[i] = fuzzyController_B.Weighting_k *
      fuzzyController_P.left_Value[i];
  }

  /* End of Product: '<S110>/impMethod' */

  /* If: '<S117>/If' incorporates:
   *  Constant: '<S119>/0'
   *  Constant: '<S120>/0'
   */
  if ((fuzzyController_B.Saturation5[0] < 0.0) ||
      (fuzzyController_B.Saturation5[0] > 2.0)) {
    /* Outputs for IfAction SubSystem: '<S117>/If Action Subsystem' incorporates:
     *  ActionPort: '<S119>/Action Port'
     */
    fuzzyController_B.Merge_bs = fuzzyController_P._Value_ll;

    /* End of Outputs for SubSystem: '<S117>/If Action Subsystem' */
  } else if (fuzzyController_B.Saturation5[0] == 1.0) {
    /* Outputs for IfAction SubSystem: '<S117>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S120>/Action Port'
     */
    fuzzyController_B.Merge_bs = fuzzyController_P._Value_fz;

    /* End of Outputs for SubSystem: '<S117>/If Action Subsystem1' */
  } else if (fuzzyController_B.Saturation5[0] < 1.0) {
    /* Outputs for IfAction SubSystem: '<S117>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S122>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Saturation5[0],
      &fuzzyController_B.Merge_bs, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_b);

    /* End of Outputs for SubSystem: '<S117>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S117>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S121>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Saturation5[0],
      &fuzzyController_B.Merge_bs, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_e);

    /* End of Outputs for SubSystem: '<S117>/If Action Subsystem2' */
  }

  /* End of If: '<S117>/If' */

  /* Product: '<S111>/Weighting' incorporates:
   *  Constant: '<S111>/Weight'
   *  Product: '<S111>/andorMethod'
   */
  fuzzyController_B.Weighting_h = fuzzyController_B.Merge_bs *
    fuzzyController_B.Merge_b * fuzzyController_P.Weight_Value_a;

  /* Sum: '<S106>/AggMethod1' incorporates:
   *  Constant: '<S113>/right'
   *  Product: '<S111>/impMethod'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod[i] = ((fuzzyController_B.impMethod[i] +
      fuzzyController_B.impMethod_a[i]) + fuzzyController_B.impMethod_k[i]) +
      fuzzyController_B.Weighting_h * fuzzyController_P.right_Value[i];
  }

  /* End of Sum: '<S106>/AggMethod1' */

  /* Sum: '<S107>/Sum1' */
  fuzzyController_B.d0 = fuzzyController_B.impMethod[0];
  for (i = 0; i < 100; i++) {
    fuzzyController_B.d0 += fuzzyController_B.impMethod[i + 1];
  }

  fuzzyController_B.Sum1 = fuzzyController_B.d0;

  /* End of Sum: '<S107>/Sum1' */

  /* If: '<S107>/If' incorporates:
   *  Constant: '<S115>/One'
   */
  if (fuzzyController_B.Sum1 <= 0.0) {
    /* Outputs for IfAction SubSystem: '<S107>/Action: One' incorporates:
     *  ActionPort: '<S115>/Action Port'
     */
    fuzzyController_B.Merge_c = fuzzyController_P.One_Value_l;

    /* End of Outputs for SubSystem: '<S107>/Action: One' */
  } else {
    /* Outputs for IfAction SubSystem: '<S107>/Action: u1' incorporates:
     *  ActionPort: '<S116>/Action Port'
     */
    fuzzyController_Actionu1(fuzzyController_B.Sum1, &fuzzyController_B.Merge_c);

    /* End of Outputs for SubSystem: '<S107>/Action: u1' */
  }

  /* End of If: '<S107>/If' */

  /* Switch: '<S106>/Switch' incorporates:
   *  Constant: '<S106>/MidRange'
   *  Constant: '<S106>/Zero'
   *  Product: '<S107>/Averaging (COA)'
   *  RelationalOperator: '<S106>/Zero Firing Strength?'
   *  Sum: '<S106>/Total Firing Strength'
   */
  if ((((fuzzyController_B.Weighting_a + fuzzyController_B.Weighting_i) +
        fuzzyController_B.Weighting_k) + fuzzyController_B.Weighting_h >
       fuzzyController_P.Zero_Value) >= fuzzyController_P.Switch_Threshold) {
    /* Product: '<S107>/Product (COA)' incorporates:
     *  Constant: '<S107>/x data'
     */
    for (i = 0; i < 101; i++) {
      fuzzyController_B.impMethod[i] *= fuzzyController_P.xdata_Value[i];
    }

    /* End of Product: '<S107>/Product (COA)' */

    /* Sum: '<S107>/Sum' */
    fuzzyController_B.Weighting_i = fuzzyController_B.impMethod[0];
    for (i = 0; i < 100; i++) {
      fuzzyController_B.Weighting_i += fuzzyController_B.impMethod[i + 1];
    }

    /* End of Sum: '<S107>/Sum' */
    fuzzyController_B.Switch = fuzzyController_B.Weighting_i /
      fuzzyController_B.Merge_c;
  } else {
    fuzzyController_B.Switch = fuzzyController_P.MidRange_Value;
  }

  /* End of Switch: '<S106>/Switch' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Inport: '<Root>/SteeringReverse '
   */
  if (fuzzyController_U.SteeringReverse > fuzzyController_P.Switch_Threshold_n)
  {
    /* Outport: '<Root>/steeringOutput' */
    fuzzyController_Y.steeringOutput = fuzzyController_B.Switch;
  } else {
    /* Outport: '<Root>/steeringOutput' incorporates:
     *  Gain: '<S1>/Gain'
     */
    fuzzyController_Y.steeringOutput = fuzzyController_P.Gain_Gain *
      fuzzyController_B.Switch;
  }

  /* End of Switch: '<S1>/Switch' */

  /* If: '<S55>/If' incorporates:
   *  Constant: '<S57>/0'
   *  Constant: '<S58>/0'
   *  Inport: '<Root>/powerBias'
   */
  if ((fuzzyController_U.powerBias < -0.0) || (fuzzyController_U.powerBias > 2.0))
  {
    /* Outputs for IfAction SubSystem: '<S55>/If Action Subsystem' incorporates:
     *  ActionPort: '<S57>/Action Port'
     */
    fuzzyController_B.Merge_m = fuzzyController_P._Value_e;

    /* End of Outputs for SubSystem: '<S55>/If Action Subsystem' */
  } else if (fuzzyController_U.powerBias == 1.0) {
    /* Outputs for IfAction SubSystem: '<S55>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S58>/Action Port'
     */
    fuzzyController_B.Merge_m = fuzzyController_P._Value_p;

    /* End of Outputs for SubSystem: '<S55>/If Action Subsystem1' */
  } else if (fuzzyController_U.powerBias < 1.0) {
    /* Outputs for IfAction SubSystem: '<S55>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S60>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.powerBias,
      &fuzzyController_B.Merge_m, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_d);

    /* End of Outputs for SubSystem: '<S55>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S55>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S59>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.powerBias,
      &fuzzyController_B.Merge_m, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_b);

    /* End of Outputs for SubSystem: '<S55>/If Action Subsystem2' */
  }

  /* End of If: '<S55>/If' */

  /* Product: '<S19>/Weighting' incorporates:
   *  Constant: '<S19>/Weight'
   *  Product: '<S19>/andorMethod'
   */
  fuzzyController_B.Weighting_h = fuzzyController_P.Weight_Value_b *
    fuzzyController_B.Merge_m;

  /* Product: '<S19>/impMethod' incorporates:
   *  Constant: '<S16>/neutral'
   *  Constant: '<S17>/neutral'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_f1[i] = fuzzyController_B.Weighting_h *
      fuzzyController_P.neutral_Value[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_f1[i + 101] = fuzzyController_B.Weighting_h *
      fuzzyController_P.neutral_Value_h[i];
  }

  /* End of Product: '<S19>/impMethod' */

  /* If: '<S91>/If' incorporates:
   *  Constant: '<S97>/0'
   *  Constant: '<S98>/0'
   *  Inport: '<Root>/throttleSignal'
   */
  if ((fuzzyController_U.throttleSignal < -1.0) ||
      (fuzzyController_U.throttleSignal > 1.0)) {
    /* Outputs for IfAction SubSystem: '<S91>/If Action Subsystem' incorporates:
     *  ActionPort: '<S97>/Action Port'
     */
    fuzzyController_B.Merge_k4 = fuzzyController_P._Value_af;

    /* End of Outputs for SubSystem: '<S91>/If Action Subsystem' */
  } else if (fuzzyController_U.throttleSignal == 0.0) {
    /* Outputs for IfAction SubSystem: '<S91>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S98>/Action Port'
     */
    fuzzyController_B.Merge_k4 = fuzzyController_P._Value_b;

    /* End of Outputs for SubSystem: '<S91>/If Action Subsystem1' */
  } else if (fuzzyController_U.throttleSignal < 0.0) {
    /* Outputs for IfAction SubSystem: '<S91>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S100>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_k4, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_co);

    /* End of Outputs for SubSystem: '<S91>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S91>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S99>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_k4, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_f);

    /* End of Outputs for SubSystem: '<S91>/If Action Subsystem2' */
  }

  /* End of If: '<S91>/If' */

  /* Product: '<S20>/Weighting' incorporates:
   *  Constant: '<S20>/Weight'
   *  Product: '<S20>/andorMethod'
   */
  fuzzyController_B.Weighting_k = fuzzyController_P.Weight_Value_h *
    fuzzyController_B.Merge_k4;

  /* Product: '<S20>/impMethod' incorporates:
   *  Constant: '<S10>/neutral'
   *  Constant: '<S11>/neutral'
   *  Constant: '<S16>/neutral'
   *  Constant: '<S17>/neutral'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_p[i] = fuzzyController_B.Weighting_k *
      fuzzyController_P.neutral_Value[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_p[i + 101] = fuzzyController_B.Weighting_k *
      fuzzyController_P.neutral_Value_h[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_p[i + 202] = fuzzyController_B.Weighting_k *
      fuzzyController_P.neutral_Value_he[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_p[i + 303] = fuzzyController_B.Weighting_k *
      fuzzyController_P.neutral_Value_hb[i];
  }

  /* End of Product: '<S20>/impMethod' */

  /* If: '<S92>/If' incorporates:
   *  Constant: '<S101>/0'
   *  Constant: '<S102>/0'
   *  Inport: '<Root>/throttleSignal'
   */
  if ((fuzzyController_U.throttleSignal < -2.0) ||
      (fuzzyController_U.throttleSignal > 0.0)) {
    /* Outputs for IfAction SubSystem: '<S92>/If Action Subsystem' incorporates:
     *  ActionPort: '<S101>/Action Port'
     */
    fuzzyController_B.Merge_j = fuzzyController_P._Value_i3;

    /* End of Outputs for SubSystem: '<S92>/If Action Subsystem' */
  } else if (fuzzyController_U.throttleSignal == -1.0) {
    /* Outputs for IfAction SubSystem: '<S92>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S102>/Action Port'
     */
    fuzzyController_B.Merge_j = fuzzyController_P._Value_f;

    /* End of Outputs for SubSystem: '<S92>/If Action Subsystem1' */
  } else if (fuzzyController_U.throttleSignal < -1.0) {
    /* Outputs for IfAction SubSystem: '<S92>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S104>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_j, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_l);

    /* End of Outputs for SubSystem: '<S92>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S92>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S103>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_j, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_c);

    /* End of Outputs for SubSystem: '<S92>/If Action Subsystem2' */
  }

  /* End of If: '<S92>/If' */

  /* Product: '<S21>/Weighting' incorporates:
   *  Constant: '<S21>/Weight'
   *  Product: '<S21>/andorMethod'
   */
  fuzzyController_B.Weighting_i = fuzzyController_P.Weight_Value_hu *
    fuzzyController_B.Merge_j;

  /* Product: '<S21>/impMethod' incorporates:
   *  Constant: '<S10>/reverse'
   *  Constant: '<S11>/reverse'
   *  Constant: '<S16>/reverse'
   *  Constant: '<S17>/reverse'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_c[i] = fuzzyController_B.Weighting_i *
      fuzzyController_P.reverse_Value[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_c[i + 101] = fuzzyController_B.Weighting_i *
      fuzzyController_P.reverse_Value_b[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_c[i + 202] = fuzzyController_B.Weighting_i *
      fuzzyController_P.reverse_Value_c[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_c[i + 303] = fuzzyController_B.Weighting_i *
      fuzzyController_P.reverse_Value_n[i];
  }

  /* End of Product: '<S21>/impMethod' */

  /* If: '<S41>/If' incorporates:
   *  Constant: '<S47>/0'
   *  Constant: '<S48>/0'
   *  Inport: '<Root>/frontDifferential'
   */
  if ((fuzzyController_U.frontDifferential < -1.0) ||
      (fuzzyController_U.frontDifferential > 1.0)) {
    /* Outputs for IfAction SubSystem: '<S41>/If Action Subsystem' incorporates:
     *  ActionPort: '<S47>/Action Port'
     */
    fuzzyController_B.Merge_g = fuzzyController_P._Value_i;

    /* End of Outputs for SubSystem: '<S41>/If Action Subsystem' */
  } else if (fuzzyController_U.frontDifferential == 0.0) {
    /* Outputs for IfAction SubSystem: '<S41>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S48>/Action Port'
     */
    fuzzyController_B.Merge_g = fuzzyController_P._Value_h;

    /* End of Outputs for SubSystem: '<S41>/If Action Subsystem1' */
  } else if (fuzzyController_U.frontDifferential < 0.0) {
    /* Outputs for IfAction SubSystem: '<S41>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S50>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.frontDifferential,
      &fuzzyController_B.Merge_g, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_a);

    /* End of Outputs for SubSystem: '<S41>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S41>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S49>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.frontDifferential,
      &fuzzyController_B.Merge_g, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_d);

    /* End of Outputs for SubSystem: '<S41>/If Action Subsystem2' */
  }

  /* End of If: '<S41>/If' */

  /* If: '<S80>/If' incorporates:
   *  Constant: '<S82>/0'
   *  Constant: '<S83>/0'
   */
  if ((fuzzyController_B.Switch < -2.0) || (fuzzyController_B.Switch > 0.0)) {
    /* Outputs for IfAction SubSystem: '<S80>/If Action Subsystem' incorporates:
     *  ActionPort: '<S82>/Action Port'
     */
    fuzzyController_B.Merge_my = fuzzyController_P._Value_pq;

    /* End of Outputs for SubSystem: '<S80>/If Action Subsystem' */
  } else if (fuzzyController_B.Switch == -1.0) {
    /* Outputs for IfAction SubSystem: '<S80>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S83>/Action Port'
     */
    fuzzyController_B.Merge_my = fuzzyController_P._Value_a;

    /* End of Outputs for SubSystem: '<S80>/If Action Subsystem1' */
  } else if (fuzzyController_B.Switch < -1.0) {
    /* Outputs for IfAction SubSystem: '<S80>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S85>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Switch,
      &fuzzyController_B.Merge_my, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_p);

    /* End of Outputs for SubSystem: '<S80>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S80>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S84>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Switch,
      &fuzzyController_B.Merge_my, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_p);

    /* End of Outputs for SubSystem: '<S80>/If Action Subsystem2' */
  }

  /* End of If: '<S80>/If' */

  /* If: '<S90>/If' incorporates:
   *  Constant: '<S93>/0'
   *  Constant: '<S94>/0'
   *  Inport: '<Root>/throttleSignal'
   */
  if ((fuzzyController_U.throttleSignal < -0.0) ||
      (fuzzyController_U.throttleSignal > 2.0)) {
    /* Outputs for IfAction SubSystem: '<S90>/If Action Subsystem' incorporates:
     *  ActionPort: '<S93>/Action Port'
     */
    fuzzyController_B.Merge_o = fuzzyController_P._Value_aj;

    /* End of Outputs for SubSystem: '<S90>/If Action Subsystem' */
  } else if (fuzzyController_U.throttleSignal == 1.0) {
    /* Outputs for IfAction SubSystem: '<S90>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S94>/Action Port'
     */
    fuzzyController_B.Merge_o = fuzzyController_P._Value_ju;

    /* End of Outputs for SubSystem: '<S90>/If Action Subsystem1' */
  } else if (fuzzyController_U.throttleSignal < 1.0) {
    /* Outputs for IfAction SubSystem: '<S90>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S96>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_o, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_g);

    /* End of Outputs for SubSystem: '<S90>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S90>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S95>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.throttleSignal,
      &fuzzyController_B.Merge_o, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_pl);

    /* End of Outputs for SubSystem: '<S90>/If Action Subsystem2' */
  }

  /* End of If: '<S90>/If' */

  /* Product: '<S24>/Weighting' incorporates:
   *  Constant: '<S24>/Weight'
   *  Product: '<S24>/andorMethod'
   */
  fuzzyController_B.Weighting_a = fuzzyController_B.Merge_my *
    fuzzyController_B.Merge_o * fuzzyController_B.Merge_g *
    fuzzyController_P.Weight_Value_bt;

  /* Product: '<S24>/impMethod' incorporates:
   *  Constant: '<S16>/neutral'
   *  Constant: '<S17>/forward'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_g[i] = fuzzyController_B.Weighting_a *
      fuzzyController_P.neutral_Value[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_g[i + 101] = fuzzyController_B.Weighting_a *
      fuzzyController_P.forward_Value[i];
  }

  /* End of Product: '<S24>/impMethod' */

  /* If: '<S81>/If' incorporates:
   *  Constant: '<S86>/0'
   *  Constant: '<S87>/0'
   */
  if ((fuzzyController_B.Switch < -0.0) || (fuzzyController_B.Switch > 2.0)) {
    /* Outputs for IfAction SubSystem: '<S81>/If Action Subsystem' incorporates:
     *  ActionPort: '<S86>/Action Port'
     */
    fuzzyController_B.Merge_h = fuzzyController_P._Value_n4;

    /* End of Outputs for SubSystem: '<S81>/If Action Subsystem' */
  } else if (fuzzyController_B.Switch == 1.0) {
    /* Outputs for IfAction SubSystem: '<S81>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S87>/Action Port'
     */
    fuzzyController_B.Merge_h = fuzzyController_P._Value_py;

    /* End of Outputs for SubSystem: '<S81>/If Action Subsystem1' */
  } else if (fuzzyController_B.Switch < 1.0) {
    /* Outputs for IfAction SubSystem: '<S81>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S89>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_B.Switch,
      &fuzzyController_B.Merge_h, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_db);

    /* End of Outputs for SubSystem: '<S81>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S81>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S88>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_B.Switch,
      &fuzzyController_B.Merge_h, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_g);

    /* End of Outputs for SubSystem: '<S81>/If Action Subsystem2' */
  }

  /* End of If: '<S81>/If' */

  /* Product: '<S25>/Weighting' incorporates:
   *  Constant: '<S25>/Weight'
   *  Product: '<S25>/andorMethod'
   */
  fuzzyController_B.Weighting = fuzzyController_B.Merge_h *
    fuzzyController_B.Merge_o * fuzzyController_B.Merge_g *
    fuzzyController_P.Weight_Value_fj;

  /* Product: '<S25>/impMethod' incorporates:
   *  Constant: '<S16>/forward'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_j[i] = fuzzyController_B.Weighting *
      fuzzyController_P.forward_Value_a[i];
  }

  /* Product: '<S26>/Weighting' incorporates:
   *  Constant: '<S26>/Weight'
   *  Product: '<S26>/andorMethod'
   */
  fuzzyController_B.Weighting_c = fuzzyController_P.Weight_Value_o *
    fuzzyController_B.Merge_o;
  for (i = 0; i < 101; i++) {
    /* Product: '<S25>/impMethod' incorporates:
     *  Constant: '<S17>/neutral'
     */
    fuzzyController_B.impMethod_j[i + 101] = fuzzyController_B.Weighting *
      fuzzyController_P.neutral_Value_h[i];

    /* Product: '<S26>/impMethod' incorporates:
     *  Constant: '<S16>/forward'
     */
    fuzzyController_B.impMethod_pd[i] = fuzzyController_B.Weighting_c *
      fuzzyController_P.forward_Value_a[i];
  }

  /* Product: '<S26>/impMethod' incorporates:
   *  Constant: '<S10>/forward'
   *  Constant: '<S11>/forward'
   *  Constant: '<S17>/forward'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_pd[i + 101] = fuzzyController_B.Weighting_c *
      fuzzyController_P.forward_Value[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_pd[i + 202] = fuzzyController_B.Weighting_c *
      fuzzyController_P.forward_Value_e[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_pd[i + 303] = fuzzyController_B.Weighting_c *
      fuzzyController_P.forward_Value_c[i];
  }

  /* Sum: '<S9>/AggMethod1' */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod[i] = ((((fuzzyController_B.impMethod_f1[i] +
      fuzzyController_B.impMethod_p[i]) + fuzzyController_B.impMethod_c[i]) +
      fuzzyController_B.impMethod_g[i]) + fuzzyController_B.impMethod_j[i]) +
      fuzzyController_B.impMethod_pd[i];
  }

  /* End of Sum: '<S9>/AggMethod1' */

  /* Sum: '<S12>/Sum1' */
  fuzzyController_B.d0 = fuzzyController_B.impMethod[0];
  for (i = 0; i < 100; i++) {
    fuzzyController_B.d0 += fuzzyController_B.impMethod[i + 1];
  }

  fuzzyController_B.Sum1_f = fuzzyController_B.d0;

  /* End of Sum: '<S12>/Sum1' */

  /* If: '<S12>/If' incorporates:
   *  Constant: '<S32>/One'
   */
  if (fuzzyController_B.Sum1_f <= 0.0) {
    /* Outputs for IfAction SubSystem: '<S12>/Action: One' incorporates:
     *  ActionPort: '<S32>/Action Port'
     */
    fuzzyController_B.Merge_jp = fuzzyController_P.One_Value;

    /* End of Outputs for SubSystem: '<S12>/Action: One' */
  } else {
    /* Outputs for IfAction SubSystem: '<S12>/Action: u1' incorporates:
     *  ActionPort: '<S33>/Action Port'
     */
    fuzzyController_Actionu1(fuzzyController_B.Sum1_f,
      &fuzzyController_B.Merge_jp);

    /* End of Outputs for SubSystem: '<S12>/Action: u1' */
  }

  /* End of If: '<S12>/If' */

  /* Sum: '<S9>/AggMethod2' */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_a[i] = ((((fuzzyController_B.impMethod_f1[i +
      101] + fuzzyController_B.impMethod_p[i + 101]) +
      fuzzyController_B.impMethod_c[i + 101]) + fuzzyController_B.impMethod_g[i
      + 101]) + fuzzyController_B.impMethod_j[i + 101]) +
      fuzzyController_B.impMethod_pd[i + 101];
  }

  /* End of Sum: '<S9>/AggMethod2' */

  /* Sum: '<S13>/Sum1' */
  fuzzyController_B.d0 = fuzzyController_B.impMethod_a[0];
  for (i = 0; i < 100; i++) {
    fuzzyController_B.d0 += fuzzyController_B.impMethod_a[i + 1];
  }

  fuzzyController_B.Sum1_l = fuzzyController_B.d0;

  /* End of Sum: '<S13>/Sum1' */

  /* If: '<S13>/If' incorporates:
   *  Constant: '<S34>/One'
   */
  if (fuzzyController_B.Sum1_l <= 0.0) {
    /* Outputs for IfAction SubSystem: '<S13>/Action: One' incorporates:
     *  ActionPort: '<S34>/Action Port'
     */
    fuzzyController_B.Merge_i = fuzzyController_P.One_Value_n;

    /* End of Outputs for SubSystem: '<S13>/Action: One' */
  } else {
    /* Outputs for IfAction SubSystem: '<S13>/Action: u1' incorporates:
     *  ActionPort: '<S35>/Action Port'
     */
    fuzzyController_Actionu1(fuzzyController_B.Sum1_l,
      &fuzzyController_B.Merge_i);

    /* End of Outputs for SubSystem: '<S13>/Action: u1' */
  }

  /* End of If: '<S13>/If' */

  /* If: '<S56>/If' incorporates:
   *  Constant: '<S61>/0'
   *  Constant: '<S62>/0'
   *  Inport: '<Root>/powerBias'
   */
  if ((fuzzyController_U.powerBias < -2.0) || (fuzzyController_U.powerBias >
       -0.0)) {
    /* Outputs for IfAction SubSystem: '<S56>/If Action Subsystem' incorporates:
     *  ActionPort: '<S61>/Action Port'
     */
    fuzzyController_B.Merge_n = fuzzyController_P._Value_j;

    /* End of Outputs for SubSystem: '<S56>/If Action Subsystem' */
  } else if (fuzzyController_U.powerBias == -1.0) {
    /* Outputs for IfAction SubSystem: '<S56>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S62>/Action Port'
     */
    fuzzyController_B.Merge_n = fuzzyController_P._Value_mx;

    /* End of Outputs for SubSystem: '<S56>/If Action Subsystem1' */
  } else if (fuzzyController_U.powerBias < -1.0) {
    /* Outputs for IfAction SubSystem: '<S56>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S64>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.powerBias,
      &fuzzyController_B.Merge_n, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_f);

    /* End of Outputs for SubSystem: '<S56>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S56>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S63>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.powerBias,
      &fuzzyController_B.Merge_n, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_ga);

    /* End of Outputs for SubSystem: '<S56>/If Action Subsystem2' */
  }

  /* End of If: '<S56>/If' */

  /* Product: '<S18>/Weighting' incorporates:
   *  Constant: '<S18>/Weight'
   *  Product: '<S18>/andorMethod'
   */
  fuzzyController_B.Weighting_e = fuzzyController_P.Weight_Value_k *
    fuzzyController_B.Merge_n;

  /* Product: '<S18>/impMethod' incorporates:
   *  Constant: '<S10>/neutral'
   *  Constant: '<S11>/neutral'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_j[i] = fuzzyController_B.Weighting_e *
      fuzzyController_P.neutral_Value_he[i];
  }

  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_j[i + 101] = fuzzyController_B.Weighting_e *
      fuzzyController_P.neutral_Value_hb[i];
  }

  /* End of Product: '<S18>/impMethod' */

  /* If: '<S66>/If' incorporates:
   *  Constant: '<S72>/0'
   *  Constant: '<S73>/0'
   *  Inport: '<Root>/rearDifferential'
   */
  if ((fuzzyController_U.rearDifferential < -1.0) ||
      (fuzzyController_U.rearDifferential > 1.0)) {
    /* Outputs for IfAction SubSystem: '<S66>/If Action Subsystem' incorporates:
     *  ActionPort: '<S72>/Action Port'
     */
    rtb_Merge_ch = fuzzyController_P._Value_d;

    /* End of Outputs for SubSystem: '<S66>/If Action Subsystem' */
  } else if (fuzzyController_U.rearDifferential == 0.0) {
    /* Outputs for IfAction SubSystem: '<S66>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S73>/Action Port'
     */
    rtb_Merge_ch = fuzzyController_P._Value_no;

    /* End of Outputs for SubSystem: '<S66>/If Action Subsystem1' */
  } else if (fuzzyController_U.rearDifferential < 0.0) {
    /* Outputs for IfAction SubSystem: '<S66>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S75>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.rearDifferential,
      &rtb_Merge_ch, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_i);

    /* End of Outputs for SubSystem: '<S66>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S66>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S74>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.rearDifferential,
      &rtb_Merge_ch, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_a);

    /* End of Outputs for SubSystem: '<S66>/If Action Subsystem2' */
  }

  /* End of If: '<S66>/If' */

  /* Product: '<S22>/Weighting' incorporates:
   *  Constant: '<S22>/Weight'
   *  Product: '<S22>/andorMethod'
   */
  fuzzyController_B.Weighting_o = fuzzyController_B.Merge_my *
    fuzzyController_B.Merge_o * rtb_Merge_ch * fuzzyController_P.Weight_Value_g;

  /* Product: '<S22>/impMethod' incorporates:
   *  Constant: '<S10>/neutral'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_g[i] = fuzzyController_B.Weighting_o *
      fuzzyController_P.neutral_Value_he[i];
  }

  /* Product: '<S23>/Weighting' incorporates:
   *  Constant: '<S23>/Weight'
   *  Product: '<S23>/andorMethod'
   */
  fuzzyController_B.ZeroFiringStrength_n = fuzzyController_B.Merge_h *
    fuzzyController_B.Merge_o * rtb_Merge_ch * fuzzyController_P.Weight_Value_j;
  for (i = 0; i < 101; i++) {
    /* Product: '<S22>/impMethod' incorporates:
     *  Constant: '<S11>/forward'
     */
    fuzzyController_B.impMethod_g[i + 101] = fuzzyController_B.Weighting_o *
      fuzzyController_P.forward_Value_c[i];

    /* Product: '<S23>/impMethod' incorporates:
     *  Constant: '<S10>/forward'
     */
    fuzzyController_B.impMethod_f1[i] = fuzzyController_B.ZeroFiringStrength_n *
      fuzzyController_P.forward_Value_e[i];
  }

  /* Product: '<S23>/impMethod' incorporates:
   *  Constant: '<S11>/neutral'
   */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_f1[i + 101] =
      fuzzyController_B.ZeroFiringStrength_n *
      fuzzyController_P.neutral_Value_hb[i];
  }

  /* Sum: '<S9>/AggMethod3' */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.impMethod_k[i] = ((((fuzzyController_B.impMethod_p[i + 202]
      + fuzzyController_B.impMethod_j[i]) + fuzzyController_B.impMethod_c[i +
      202]) + fuzzyController_B.impMethod_g[i]) +
      fuzzyController_B.impMethod_f1[i]) + fuzzyController_B.impMethod_pd[i +
      202];
  }

  /* End of Sum: '<S9>/AggMethod3' */

  /* Sum: '<S14>/Sum1' */
  fuzzyController_B.d0 = fuzzyController_B.impMethod_k[0];
  for (i = 0; i < 100; i++) {
    fuzzyController_B.d0 += fuzzyController_B.impMethod_k[i + 1];
  }

  fuzzyController_B.Sum1_h = fuzzyController_B.d0;

  /* End of Sum: '<S14>/Sum1' */

  /* If: '<S14>/If' incorporates:
   *  Constant: '<S36>/One'
   */
  if (fuzzyController_B.Sum1_h <= 0.0) {
    /* Outputs for IfAction SubSystem: '<S14>/Action: One' incorporates:
     *  ActionPort: '<S36>/Action Port'
     */
    rtb_Merge_ic = fuzzyController_P.One_Value_g;

    /* End of Outputs for SubSystem: '<S14>/Action: One' */
  } else {
    /* Outputs for IfAction SubSystem: '<S14>/Action: u1' incorporates:
     *  ActionPort: '<S37>/Action Port'
     */
    fuzzyController_Actionu1(fuzzyController_B.Sum1_h, &rtb_Merge_ic);

    /* End of Outputs for SubSystem: '<S14>/Action: u1' */
  }

  /* End of If: '<S14>/If' */

  /* Sum: '<S9>/AggMethod4' */
  for (i = 0; i < 101; i++) {
    fuzzyController_B.AggMethod4[i] = ((((fuzzyController_B.impMethod_j[i + 101]
      + fuzzyController_B.impMethod_p[i + 303]) +
      fuzzyController_B.impMethod_c[i + 303]) + fuzzyController_B.impMethod_g[i
      + 101]) + fuzzyController_B.impMethod_f1[i + 101]) +
      fuzzyController_B.impMethod_pd[i + 303];
  }

  /* End of Sum: '<S9>/AggMethod4' */

  /* Sum: '<S15>/Sum1' */
  fuzzyController_B.d0 = fuzzyController_B.AggMethod4[0];
  for (i = 0; i < 100; i++) {
    fuzzyController_B.d0 += fuzzyController_B.AggMethod4[i + 1];
  }

  fuzzyController_B.Sum1_b = fuzzyController_B.d0;

  /* End of Sum: '<S15>/Sum1' */

  /* If: '<S15>/If' incorporates:
   *  Constant: '<S38>/One'
   */
  if (fuzzyController_B.Sum1_b <= 0.0) {
    /* Outputs for IfAction SubSystem: '<S15>/Action: One' incorporates:
     *  ActionPort: '<S38>/Action Port'
     */
    rtb_Merge_cf = fuzzyController_P.One_Value_o;

    /* End of Outputs for SubSystem: '<S15>/Action: One' */
  } else {
    /* Outputs for IfAction SubSystem: '<S15>/Action: u1' incorporates:
     *  ActionPort: '<S39>/Action Port'
     */
    fuzzyController_Actionu1(fuzzyController_B.Sum1_b, &rtb_Merge_cf);

    /* End of Outputs for SubSystem: '<S15>/Action: u1' */
  }

  /* End of If: '<S15>/If' */

  /* Switch: '<S9>/Switch' incorporates:
   *  Constant: '<S9>/MidRange'
   *  Constant: '<S9>/Zero'
   *  Product: '<S15>/Averaging (COA)'
   *  RelationalOperator: '<S9>/Zero Firing Strength?'
   *  Sum: '<S9>/Total Firing Strength'
   */
  if (((((((((fuzzyController_B.Weighting_e + fuzzyController_B.Weighting_h) +
             fuzzyController_B.Weighting_k) + fuzzyController_B.Weighting_i) +
           fuzzyController_B.Weighting_o) +
          fuzzyController_B.ZeroFiringStrength_n) +
         fuzzyController_B.Weighting_a) + fuzzyController_B.Weighting) +
       fuzzyController_B.Weighting_c > fuzzyController_P.Zero_Value_j) >=
      fuzzyController_P.Switch_Threshold_k) {
    /* Product: '<S12>/Product (COA)' incorporates:
     *  Constant: '<S12>/x data'
     */
    for (i = 0; i < 101; i++) {
      fuzzyController_B.impMethod[i] *= fuzzyController_P.xdata_Value_m[i];
    }

    /* End of Product: '<S12>/Product (COA)' */

    /* Sum: '<S12>/Sum' */
    fuzzyController_B.Weighting_a = fuzzyController_B.impMethod[0];
    for (i = 0; i < 100; i++) {
      fuzzyController_B.Weighting_a += fuzzyController_B.impMethod[i + 1];
    }

    /* End of Sum: '<S12>/Sum' */

    /* Product: '<S12>/Averaging (COA)' */
    fuzzyController_B.Weighting_i = fuzzyController_B.Weighting_a /
      fuzzyController_B.Merge_jp;

    /* Product: '<S13>/Product (COA)' incorporates:
     *  Constant: '<S13>/x data'
     */
    for (i = 0; i < 101; i++) {
      fuzzyController_B.impMethod_a[i] *= fuzzyController_P.xdata_Value_my[i];
    }

    /* End of Product: '<S13>/Product (COA)' */

    /* Sum: '<S13>/Sum' */
    fuzzyController_B.Weighting_a = fuzzyController_B.impMethod_a[0];
    for (i = 0; i < 100; i++) {
      fuzzyController_B.Weighting_a += fuzzyController_B.impMethod_a[i + 1];
    }

    /* End of Sum: '<S13>/Sum' */

    /* Product: '<S13>/Averaging (COA)' */
    fuzzyController_B.Weighting_c = fuzzyController_B.Weighting_a /
      fuzzyController_B.Merge_i;

    /* Product: '<S14>/Product (COA)' incorporates:
     *  Constant: '<S14>/x data'
     */
    for (i = 0; i < 101; i++) {
      fuzzyController_B.impMethod_k[i] *= fuzzyController_P.xdata_Value_i[i];
    }

    /* End of Product: '<S14>/Product (COA)' */

    /* Sum: '<S14>/Sum' */
    fuzzyController_B.Weighting_a = fuzzyController_B.impMethod_k[0];
    for (i = 0; i < 100; i++) {
      fuzzyController_B.Weighting_a += fuzzyController_B.impMethod_k[i + 1];
    }

    /* End of Sum: '<S14>/Sum' */

    /* Product: '<S14>/Averaging (COA)' */
    fuzzyController_B.Weighting_e = fuzzyController_B.Weighting_a / rtb_Merge_ic;

    /* Product: '<S15>/Product (COA)' incorporates:
     *  Constant: '<S15>/x data'
     */
    for (i = 0; i < 101; i++) {
      fuzzyController_B.AggMethod4[i] *= fuzzyController_P.xdata_Value_j[i];
    }

    /* End of Product: '<S15>/Product (COA)' */

    /* Sum: '<S15>/Sum' */
    fuzzyController_B.Weighting_a = fuzzyController_B.AggMethod4[0];
    for (i = 0; i < 100; i++) {
      fuzzyController_B.Weighting_a += fuzzyController_B.AggMethod4[i + 1];
    }

    /* End of Sum: '<S15>/Sum' */
    fuzzyController_B.Weighting_a /= rtb_Merge_cf;
  } else {
    fuzzyController_B.Weighting_i = fuzzyController_P.MidRange_Value_i[0];
    fuzzyController_B.Weighting_c = fuzzyController_P.MidRange_Value_i[1];
    fuzzyController_B.Weighting_e = fuzzyController_P.MidRange_Value_i[2];
    fuzzyController_B.Weighting_a = fuzzyController_P.MidRange_Value_i[3];
  }

  /* End of Switch: '<S9>/Switch' */

  /* Outport: '<Root>/FLWheelOutput' */
  fuzzyController_Y.FLWheelOutput = fuzzyController_B.Weighting_i;

  /* Outport: '<Root>/FRWheelOutput' */
  fuzzyController_Y.FRWheelOutput = fuzzyController_B.Weighting_c;

  /* Outport: '<Root>/BLWheelOutput' */
  fuzzyController_Y.BLWheelOutput = fuzzyController_B.Weighting_e;

  /* Outport: '<Root>/BRWheelOutput' */
  fuzzyController_Y.BRWheelOutput = fuzzyController_B.Weighting_a;

  /* If: '<S40>/If' incorporates:
   *  Constant: '<S43>/0'
   *  Constant: '<S44>/0'
   *  Inport: '<Root>/frontDifferential'
   */
  if ((fuzzyController_U.frontDifferential < -2.0) ||
      (fuzzyController_U.frontDifferential > 0.0)) {
    /* Outputs for IfAction SubSystem: '<S40>/If Action Subsystem' incorporates:
     *  ActionPort: '<S43>/Action Port'
     */
    rtb_Merge_j2 = fuzzyController_P._Value;

    /* End of Outputs for SubSystem: '<S40>/If Action Subsystem' */
  } else if (fuzzyController_U.frontDifferential == -1.0) {
    /* Outputs for IfAction SubSystem: '<S40>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S44>/Action Port'
     */
    rtb_Merge_j2 = fuzzyController_P._Value_n;

    /* End of Outputs for SubSystem: '<S40>/If Action Subsystem1' */
  } else if (fuzzyController_U.frontDifferential < -1.0) {
    /* Outputs for IfAction SubSystem: '<S40>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S46>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.frontDifferential,
      &rtb_Merge_j2, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_n);

    /* End of Outputs for SubSystem: '<S40>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S40>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S45>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.frontDifferential,
      &rtb_Merge_j2, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_c0);

    /* End of Outputs for SubSystem: '<S40>/If Action Subsystem2' */
  }

  /* End of If: '<S40>/If' */

  /* If: '<S42>/If' incorporates:
   *  Constant: '<S51>/0'
   *  Constant: '<S52>/0'
   *  Inport: '<Root>/frontDifferential'
   */
  if ((fuzzyController_U.frontDifferential < -0.0) ||
      (fuzzyController_U.frontDifferential > 2.0)) {
    /* Outputs for IfAction SubSystem: '<S42>/If Action Subsystem' incorporates:
     *  ActionPort: '<S51>/Action Port'
     */
    rtb_Merge_kn = fuzzyController_P._Value_o;

    /* End of Outputs for SubSystem: '<S42>/If Action Subsystem' */
  } else if (fuzzyController_U.frontDifferential == 1.0) {
    /* Outputs for IfAction SubSystem: '<S42>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S52>/Action Port'
     */
    rtb_Merge_kn = fuzzyController_P._Value_m;

    /* End of Outputs for SubSystem: '<S42>/If Action Subsystem1' */
  } else if (fuzzyController_U.frontDifferential < 1.0) {
    /* Outputs for IfAction SubSystem: '<S42>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S54>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.frontDifferential,
      &rtb_Merge_kn, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_ng);

    /* End of Outputs for SubSystem: '<S42>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S42>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S53>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.frontDifferential,
      &rtb_Merge_kn, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_p0);

    /* End of Outputs for SubSystem: '<S42>/If Action Subsystem2' */
  }

  /* End of If: '<S42>/If' */

  /* If: '<S65>/If' incorporates:
   *  Constant: '<S68>/0'
   *  Constant: '<S69>/0'
   *  Inport: '<Root>/rearDifferential'
   */
  if ((fuzzyController_U.rearDifferential < -2.0) ||
      (fuzzyController_U.rearDifferential > 0.0)) {
    /* Outputs for IfAction SubSystem: '<S65>/If Action Subsystem' incorporates:
     *  ActionPort: '<S68>/Action Port'
     */
    rtb_Merge_ok = fuzzyController_P._Value_mg;

    /* End of Outputs for SubSystem: '<S65>/If Action Subsystem' */
  } else if (fuzzyController_U.rearDifferential == -1.0) {
    /* Outputs for IfAction SubSystem: '<S65>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S69>/Action Port'
     */
    rtb_Merge_ok = fuzzyController_P._Value_iq;

    /* End of Outputs for SubSystem: '<S65>/If Action Subsystem1' */
  } else if (fuzzyController_U.rearDifferential < -1.0) {
    /* Outputs for IfAction SubSystem: '<S65>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S71>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.rearDifferential,
      &rtb_Merge_ok, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_o);

    /* End of Outputs for SubSystem: '<S65>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S65>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S70>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.rearDifferential,
      &rtb_Merge_ok, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_l);

    /* End of Outputs for SubSystem: '<S65>/If Action Subsystem2' */
  }

  /* End of If: '<S65>/If' */

  /* If: '<S67>/If' incorporates:
   *  Constant: '<S76>/0'
   *  Constant: '<S77>/0'
   *  Inport: '<Root>/rearDifferential'
   */
  if ((fuzzyController_U.rearDifferential < 0.0) ||
      (fuzzyController_U.rearDifferential > 2.0)) {
    /* Outputs for IfAction SubSystem: '<S67>/If Action Subsystem' incorporates:
     *  ActionPort: '<S76>/Action Port'
     */
    rtb_Merge_cy = fuzzyController_P._Value_l;

    /* End of Outputs for SubSystem: '<S67>/If Action Subsystem' */
  } else if (fuzzyController_U.rearDifferential == 1.0) {
    /* Outputs for IfAction SubSystem: '<S67>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S77>/Action Port'
     */
    rtb_Merge_cy = fuzzyController_P._Value_in;

    /* End of Outputs for SubSystem: '<S67>/If Action Subsystem1' */
  } else if (fuzzyController_U.rearDifferential < 1.0) {
    /* Outputs for IfAction SubSystem: '<S67>/If Action Subsystem3' incorporates:
     *  ActionPort: '<S79>/Action Port'
     */
    fuzzyControl_IfActionSubsystem3(fuzzyController_U.rearDifferential,
      &rtb_Merge_cy, (P_IfActionSubsystem3_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem3_m);

    /* End of Outputs for SubSystem: '<S67>/If Action Subsystem3' */
  } else {
    /* Outputs for IfAction SubSystem: '<S67>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S78>/Action Port'
     */
    fuzzyControl_IfActionSubsystem2(fuzzyController_U.rearDifferential,
      &rtb_Merge_cy, (P_IfActionSubsystem2_fuzzyCon_T *)
      &fuzzyController_P.IfActionSubsystem2_m);

    /* End of Outputs for SubSystem: '<S67>/If Action Subsystem2' */
  }

  /* End of If: '<S67>/If' */

  /* SignalConversion: '<S2>/TmpSignal ConversionAtAnimation1Inport1' incorporates:
   *  Inport: '<Root>/frontDifferential'
   *  Inport: '<Root>/powerBias'
   *  Inport: '<Root>/rearDifferential'
   *  Inport: '<Root>/throttleSignal'
   */
  fuzzyController_B.TmpSignalConversionAtAnimation1[0] =
    fuzzyController_B.Switch;
  fuzzyController_B.TmpSignalConversionAtAnimation1[1] =
    fuzzyController_U.throttleSignal;
  fuzzyController_B.TmpSignalConversionAtAnimation1[2] =
    fuzzyController_U.frontDifferential;
  fuzzyController_B.TmpSignalConversionAtAnimation1[3] =
    fuzzyController_U.rearDifferential;
  fuzzyController_B.TmpSignalConversionAtAnimation1[4] =
    fuzzyController_U.powerBias;
}

/* Model initialize function */
void fuzzyController_initialize(void)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) &fuzzyController_B), 0,
                sizeof(B_fuzzyController_T));

  /* external inputs */
  (void) memset((void *)&fuzzyController_U, 0,
                sizeof(ExtU_fuzzyController_T));

  /* external outputs */
  (void) memset((void *)&fuzzyController_Y, 0,
                sizeof(ExtY_fuzzyController_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
