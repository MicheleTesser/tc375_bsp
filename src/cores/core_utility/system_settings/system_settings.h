/**
 * @file system_settings.h
 * @brief Interface for managing runtime-configurable system settings.
 */

#ifndef __SYSTEM_SETTINGS__
#define __SYSTEM_SETTINGS__

#include <stdint.h>
#include "../../../lib/DPS/dps.h"

/**
 * @brief Opaque handle for system settings management.
 *
 * This object must be initialized with `system_settings_init()` before use.
 * Internally aligned to 4 bytes and contains implementation-specific data.
 */
typedef struct __attribute__((aligned(4))) {
  const uint8_t private_data[12];
} SytemSettingOwner_h;

/**
 * @brief Initializes the system settings module.
 *
 * Allocates and prepares all internal resources, CAN mailboxes, and DPS monitoring
 * required to manage the settings.
 *
 * @param self Pointer to a preallocated SytemSettingOwner_h instance
 * @return 0 on success, negative value on error
 */
int8_t system_settings_init(SytemSettingOwner_h* const restrict self) __attribute__((__nonnull__(1)));

/**
 * @brief Processes received CAN messages to update system settings.
 *
 * This function should be called periodically to allow DPS communication to function.
 *
 * @param self Pointer to an initialized SytemSettingOwner_h instance
 * @return 0 on success or if no message is received, negative on error
 */
int8_t system_settings_update(SytemSettingOwner_h* const restrict self) __attribute__((__nonnull__(1)));

/**
 * @brief add a var to monitor system
 * 
 * @return 0 on success, negative on error
 */
int8_t system_settings_push_var(const enum DPS_PRIMITIVE_TYPES type,
    void* const p_data, post_update post_update_f, const char name[VAR_NAME_LENGTH]);

/**
 * @brief destroy system_settings object
 */
int8_t system_settings_destroy(SytemSettingOwner_h* const restrict self)__attribute__((__nonnull__(1)));

#endif // !__SYSTEM_SETTINGS__
