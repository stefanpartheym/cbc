/*******************************************************************************
 * Codeblock utility functions
 ******************************************************************************/

#ifndef CB_UTILS_H
#define CB_UTILS_H


/* -------------------------------------------------------------------------- */

#ifdef DEBUG
#define cb_assert(condition) \
        _cb_assert(condition, #condition, __FILE__, __LINE__, __func__)
#define cb_abort(message) \
        _cb_abort(message, __FILE__, __LINE__, __func__)

void _cb_assert(const int condition,
                const char* condition_str,
                const char* file,
                const int line,
                const char* function);
void _cb_abort(const char* message,
               const char* file,
               const int line,
               const char* function);
#else
#define cb_assert(condition) do { assert(condition); } while (0)
#define cb_abort(message) do { abort(); } while (0)
#endif /* DEBUG */


#endif /* CB_UTILS_H */
