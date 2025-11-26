/**
 * @file outils.h
 * @brief Utility helper functions for general-purpose operations.
 *
 * This header provides small, reusable utility functions used across
 * the project.
 *
 * The utilities defined here do not rely on any external modules and can be
 * used independently throughout the codebase.
 */

#ifndef _OUTILS_H
#define _OUTILS_H

/**  
* @brief Converts an integer value into a null-terminated string.
*
* @param[out] dest  Pointer to a character buffer where the result will be written
* @param[in]  value Integer number to convert
* @param[in]  count Maximum number of characters allowed in 'dest', including '\0'
*
* @note The resulting string in 'dest' will always be safely null-terminated.
*/
void itos(char *dest, int value, int count);


#endif /* _OUTILS_H */