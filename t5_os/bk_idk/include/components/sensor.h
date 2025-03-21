// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_include.h>
#include <common/bk_err.h>

/*******************************************************************************
*                      Function Declarations
*******************************************************************************/

/**
 * @brief     sensor component initialization
 *
 * Create sensor deamon task and initialize sensor information
 *
 * This API should be called before any other sensor APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sensor_init(void);

/**
 * @brief     sensor component uninstallation
 *
 * Turn off the sensor component
 * Destroy sensor information and sensor deamon
 *
 * This API should be called after any other sensor APIs.
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_sensor_deinit(void);

/**
 * @brief     Obtain current temperature of MCU
 *  parameter: temperature 
 *               current temperature to return
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NOT_INIT: sensor information not init
 *    - BK_ERR_TRY_AGAIN: sensor information not ready
 *    - BK_ERR_PARAM: invalid parameter, temperature should not NULL
*/
bk_err_t bk_sensor_get_current_temperature(float *temperature);

/**
 * @brief     Obtain current voltage on VBAT
 *  parameter: voltage 
 *               current voltage to return
 *
 * @return
 *    - BK_OK: succeed
 *    - BK_ERR_NOT_INIT: sensor information not init
 *    - BK_ERR_TRY_AGAIN: sensor information not ready
 *    - BK_ERR_PARAM: invalid parameter, voltage should not NULL
*/
bk_err_t bk_sensor_get_current_voltage(float *voltage);

#ifdef __cplusplus
}
#endif
