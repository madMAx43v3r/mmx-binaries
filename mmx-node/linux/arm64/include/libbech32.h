#ifndef TXREF_BECH32_H
#define TXREF_BECH32_H

#ifdef __cplusplus

#include <string>
#include <vector>


namespace bech32 {

    // Represents which encoding was used for a bech32 string
    enum Encoding {
        Invalid, // no or invalid encoding was detected
        Bech32,  // encoding used original checksum constant (1)
        Bech32m  // encoding used default checksum constant (M = 0x2bc830a3)
    };

    // The Bech32 separator character
    static const char separator = '1';

    // Represents the payload within a bech32 string.
    // hrp: the human-readable part
    //  dp: the data part
    struct DecodedResult {
        Encoding encoding;
        std::string hrp;
        std::vector<unsigned char> dp;
    };

    // clean a bech32 string of any stray characters not in the allowed charset, except for
    // the separator character, which is '1'
    std::string stripUnknownChars(const std::string & bstring);

    // encode a "human-readable part" and a "data part", returning a bech32m string
    std::string encode(const std::string & hrp, const std::vector<unsigned char> & dp);

    // encode a "human-readable part" and a "data part", returning a bech32 string
    std::string encodeUsingOriginalConstant(const std::string & hrp, const std::vector<unsigned char> & dp);

    // decode a bech32 string, returning the "human-readable part" and a "data part"
    DecodedResult decode(const std::string & bstring);

    namespace limits {

        // size of the set of character values which are valid for a bech32 string
        const int VALID_CHARSET_SIZE = 32;

        // while there are only 32 valid character values in a bech32 string, other characters
        // can be present but will be stripped out. however, all character values must fall
        // within the following range.
        const int MIN_BECH32_CHAR_VALUE = 33;  // ascii '!'
        const int MAX_BECH32_CHAR_VALUE = 126; // ascii '~'

        // human-readable part of a bech32 string can only be between 1 and 83 characters long
        const int MIN_HRP_LENGTH = 1;
        const int MAX_HRP_LENGTH = 83;

        // separator character is always 1 char long
        const int SEPARATOR_LENGTH = 1;

        // checksum is always 6 chars long
        const int CHECKSUM_LENGTH = 6;

        // entire bech32 string can only be a certain size (after invalid characters are stripped out)
        const int MIN_BECH32_LENGTH = 8;  // MIN_HRP_LENGTH + '1' + CHECKSUM_LENGTH
        const int MAX_BECH32_LENGTH = 90; // MAX_HRP_LENGTH + '1' + CHECKSUM_LENGTH

    }
}

#endif // #ifdef __cplusplus

// C bindings - structs and functions

#ifndef __cplusplus
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
* Represents a bech32 encoded string.
*/
typedef struct bech32_bstring_s {
    char * string;
    size_t length;
} bech32_bstring;

/**
 * Represents which encoding was used for a bech32 string
 */
typedef enum bech32_encoding_e {
    ENCODING_INVALID, // no or invalid encoding was detected
    ENCODING_BECH32,  // encoding used original checksum constant (1)
    ENCODING_BECH32M  // encoding used default checksum constant (M = 0x2bc830a3)
} bech32_encoding;

/**
 * Represents the payload within a bech32 string.
 *
 * encoding: which encoding is used for this bech32 string (see: bech32_encoding enum)
 * hrp: the human-readable part
 * hrplen: length of the human-readable part (not including trailing NULL char)
 * dp: the data part
 * dplen: length of the data part
 */
typedef struct bech32_DecodedResult_s {
    bech32_encoding encoding;
    char * hrp;
    size_t hrplen;
    unsigned char * dp;
    size_t dplen;
} bech32_DecodedResult;

/**
 * libbech32 error codes
 */
typedef enum bech32_error_e
{
    E_BECH32_SUCCESS = 0,
    E_BECH32_UNKNOWN_ERROR,
    E_BECH32_NULL_ARGUMENT,
    E_BECH32_LENGTH_TOO_SHORT,
    E_BECH32_INVALID_CHECKSUM,
    E_BECH32_NO_MEMORY,
    E_BECH32_MAX_ERROR
} bech32_error;

/**
 * Error messages corresponding to the error codes
 */
extern const char *bech32_errordesc[];

/**
 * Returns error message string corresponding to the error code
 *
 * @param error_code the error code to convert
 *
 * @return error message string corresponding to the error code
 */
extern const char * bech32_strerror(bech32_error error_code);

/**
 * Allocates memory for a bech32_DecodedResult struct based on the size of the bech32 string passed in.
 *
 * This memory must be freed using bech32_free_DecodedResult().
 *
 * @param str the bech32 string to be decoded by bech32_decode()
 *
 * @return a pointer to a new bech32_DecodedResult struct, or NULL if error
 */
extern bech32_DecodedResult * bech32_create_DecodedResult(const char *str);

/**
 * Frees memory for a bech32_DecodedResult struct.
 *
 * @param decodedResult pointer to a bech32_DecodedResult struct
 */
extern void bech32_free_DecodedResult(bech32_DecodedResult *decodedResult);

/**
 * Computes final length for a to-be-encoded bech32 string
 *
 * @param hrplen the length of the "human-readable part" string. must be > 0
 * @param dplen the length of the "data part" array
 *
 * @return length of to-be-encoded bech32 string
 */
extern size_t bech32_compute_encoded_string_length(size_t hrplen, size_t dplen);

/**
 * Allocates memory for a to-be-encoded bech32 string
 *
 * This memory must be freed using bech32_free_bstring().
 *
 * @param hrplen the length of the "human-readable part" string. must be > 0
 * @param dplen the length of the "data part" array
 *
 * @return a pointer to a new bech32_bstring struct, or NULL if error
 */
extern bech32_bstring * bech32_create_bstring(size_t hrplen, size_t dplen);

/**
 * Allocates memory for a to-be-encoded bech32 string based on the size of the bech32_DecodedResult struct
 *
 * This memory must be freed using bech32_free_bstring().
 *
 * @param decodedResult a pointer to a bech32_DecodedResult struct
 *
 * @return a pointer to a new bech32_bstring struct, or NULL if error
 */
extern bech32_bstring * bech32_create_bstring_from_DecodedResult(bech32_DecodedResult *decodedResult);

/**
 * Frees memory for a bech32 string.
 *
 * @param bstring pointer to a bech32_bstring struct
 */
extern void bech32_free_bstring(bech32_bstring *bstring);

/**
 * clean a bech32 string of any stray characters not in the allowed charset, except for the
 * separator character, which is '1'
 *
 * dstlen should be at least as large as srclen
 *
 * @param dst pointer to memory to put the cleaned string.
 * @param src pointer to the string to be cleaned.
 *
 * @return E_BECH32_SUCCESS on success, others on error (input/output is NULL, output not
 * long enough for string)
 */
extern bech32_error bech32_stripUnknownChars(
        char *dst, size_t dstlen,
        const char *src, size_t srclen);

/**
 * encode a "human-readable part" (ex: "xyz") and a "data part" (ex: {1,2,3}), returning a
 * bech32m string
 *
 * @param bstring pointer to a bech32_bstring struct to store the encoded bech32 string.
 * @param hrp pointer to the "human-readable part"
 * @param dp pointer to the "data part"
 * @param dplen the length of the "data part" array
 *
 * @return E_BECH32_SUCCESS on success, others on error (i.e., hrp/dp/bstring is NULL, bstring not
 * long enough for bech32 string)
 */
extern bech32_error bech32_encode(
        bech32_bstring *bstring,
        const char *hrp,
        const unsigned char *dp, size_t dplen);

/**
 * encode a "human-readable part" (ex: "xyz") and a "data part" (ex: {1,2,3}), returning a
 * bech32 string
 *
 * @param bstring pointer to a bech32_bstring struct to store the encoded bech32 string.
 * @param hrp pointer to the "human-readable part"
 * @param dp pointer to the "data part"
 * @param dplen the length of the "data part" array
 *
 * @return E_BECH32_SUCCESS on success, others on error (i.e., hrp/dp/bstring is NULL, bstring not
 * long enough for bech32 string)
 */
extern bech32_error bech32_encode_using_original_constant(
        bech32_bstring *bstring,
        const char *hrp,
        const unsigned char *dp, size_t dplen);

/**
 * decode a bech32 string, returning the "human-readable part" and a "data part"
 *
 * @param decodedResult pointer to struct to copy the decoded "human-readable part" and "data part"
 * @param str the bech32 string to decode
 *
 * @return E_BECH32_SUCCESS on success, others on error (i.e., decodedResult is NULL, hrp/dp not
 * long enough for decoded bech32 data)
 */
extern bech32_error bech32_decode(
        bech32_DecodedResult *decodedResult,
        const char *str);

#ifdef __cplusplus
}
#endif

#endif //TXREF_BECH32_H
