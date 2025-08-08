/*
    ChibiOS - Copyright (C) 2025 Niels Westphal (crashniels)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    I2Cv1/hal_i2c_lld.c
 * @brief   LPI2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */

#include "osal.h"
#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   I2C1 driver identifier.
 */
#if LPC_I2C_USE_I2C1 || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if LPC_I2C_USE_I2C1 == TRUE
  /* Driver initialization.*/  
  i2cObjectInit(&I2CD1);
  // Enable AHBCLK
  LPC_SYSCON->SYSAHBCLKCTRL |= SYSCON_SYSAHBCLKCTRL_I2C;
#endif

}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {

  if (i2cp->state == I2C_STOP) {

#if LPC_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {

    }
#endif

  }

}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  if (i2cp->state != I2C_STOP) {

#if LPC_I2C_USE_I2C1 == TRUE
    if (&I2CD1 == i2cp) {

    }
#endif

  }
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     systime_t timeout) {
  lpi2c_master_transfer_t xfer;
  status_t ret;

  xfer.flags          = kLPI2C_TransferDefaultFlag;
  xfer.slaveAddress   = addr;
  xfer.direction      = kLPI2C_Read;
  xfer.subaddress     = 0;
  xfer.subaddressSize = 0;
  xfer.data           = rxbuf;
  xfer.dataSize       = rxbytes;
  
  ret = LPI2C_MasterTransferBlocking(i2cp->i2c.lpi2c_p, &xfer);
  if (ret != kStatus_Success) {
    return MSG_RESET;
  }

  return MSG_OK;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end. <b>After a
 *                      timeout the driver must be stopped and restarted
 *                      because the bus is in an uncertain state</b>.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      systime_t timeout) {
  lpi2c_master_transfer_t xfer;
  status_t ret;

  xfer.flags          = kLPI2C_TransferDefaultFlag;
  xfer.slaveAddress   = addr;
  xfer.direction      = kLPI2C_Write;
  xfer.subaddress     = 0;
  xfer.subaddressSize = 0;
  xfer.data           = (void *)txbuf;
  xfer.dataSize       = txbytes;

  ret = LPI2C_MasterTransferBlocking(i2cp->i2c.lpi2c_p, &xfer);
  if (ret != kStatus_Success) {
    return MSG_RESET;
  }

  return MSG_OK;
}

#endif /* HAL_USE_I2C */

/** @} */
