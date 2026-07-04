/**
 * @author Amin Tahmasebi
 * @date 2026
 * @class UUID
 *
 * Declarations only. All Doxygen contracts for the functions below
 * live above their DEFINITIONS in uuid.c (file-level convention).
 *
 * Universally Unique Identifiers per RFC 9562 (the spec that obsoletes
 * RFC 4122), with an API modelled on Python's `uuid` module.
 *
 * A `UUID` is a 16-byte value type (big-endian field order, a.k.a.
 * network byte order). Generators return one by value; formatting writes
 * into a caller-supplied buffer. The whole module is allocation-free, so
 * there is nothing to leak.
 *
 * Implemented versions:
 *   - v4: random                       (uuid_v4)
 *   - v7: Unix-time-ordered + random   (uuid_v7)   — recommended for new keys
 *   - v3: name-based, MD5              (uuid_v3)
 *   - v5: name-based, SHA-1            (uuid_v5)
 * Plus the Nil and Max special UUIDs and the four predefined namespaces.
 *
 * Dependencies: only the c_std `secrets` module (its cross-platform CSPRNG
 * drives v4/v7). MD5 and SHA-1 are embedded, so there is no OpenSSL link.
 */

 
#ifndef UUID_H_
#define UUID_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/* #define UUID_LOGGING_ENABLE */

#ifdef UUID_LOGGING_ENABLE
    #define UUID_LOG(fmt, ...) \
        do { fprintf(stderr, "[UUID LOG] " fmt "\n", ##__VA_ARGS__); } while (0)
#else
    #define UUID_LOG(...) do { } while (0)
#endif


/* Buffer sizes for the formatting helpers (include room for the NUL). */
#define UUID_STR_LEN  37    /* "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" + NUL  */
#define UUID_URN_LEN  46    /* "urn:uuid:" + 36 + NUL                        */


/* A UUID is 16 bytes in big-endian field order. Value semantics: copy it
 * freely, compare it with uuid_equal / uuid_compare. */
typedef struct {
    unsigned char bytes[16];
} UUID;


/* RFC 9562 variant (the top bits of octet 8). */
typedef enum {
    UUID_VARIANT_NCS = 0,    /* 0xx  — reserved (NCS backward compatibility) */
    UUID_VARIANT_RFC4122,    /* 10x  — the standard RFC 4122 / 9562 variant  */
    UUID_VARIANT_MICROSOFT,  /* 110  — reserved (Microsoft)                  */
    UUID_VARIANT_FUTURE      /* 111  — reserved for future definition        */
} UuidVariant;


/* ------------------------------------------------------------------ */
/* Generators                                                         */
/* ------------------------------------------------------------------ */

UUID         uuid_v4               (void);
UUID         uuid_v7               (void);
UUID         uuid_v3               (UUID namespace_id, const char* name);
UUID         uuid_v5               (UUID namespace_id, const char* name);


/* ------------------------------------------------------------------ */
/* Special values                                                     */
/* ------------------------------------------------------------------ */

UUID         uuid_nil              (void);
UUID         uuid_max              (void);


/* ------------------------------------------------------------------  */
/* Predefined namespaces (RFC 9562 Appendix C) — for uuid_v3 / uuid_v5 */
/* ------------------------------------------------------------------  */

UUID         uuid_namespace_dns    (void);
UUID         uuid_namespace_url    (void);
UUID         uuid_namespace_oid    (void);
UUID         uuid_namespace_x500   (void);


/* ------------------------------------------------------------------  */
/* Formatting / parsing (no allocation; caller-supplied buffers)       */
/* ------------------------------------------------------------------  */

void         uuid_to_string        (UUID u, char out[UUID_STR_LEN]);
void         uuid_to_urn           (UUID u, char out[UUID_URN_LEN]);
bool         uuid_from_string      (const char* str, UUID* out);

void         uuid_to_bytes         (UUID u, unsigned char out[16]);
UUID         uuid_from_bytes       (const unsigned char in[16]);


/* ------------------------------------------------------------------ */
/* Inspection                                                         */
/* ------------------------------------------------------------------ */

int                uuid_version        (UUID u);
UuidVariant        uuid_variant        (UUID u);
bool               uuid_is_nil         (UUID u);
bool               uuid_is_max         (UUID u);
bool               uuid_equal          (UUID a, UUID b);
int                uuid_compare        (UUID a, UUID b);
unsigned long long uuid_v7_timestamp_ms(UUID u);


#ifdef __cplusplus
}
#endif

#endif
