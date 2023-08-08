// Copyright 2023 Vrushabh Gada
/// @file
/// @brief Support for BlueStar A/C protocols.


// Supports:
//   Brand: BlueStar

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kBluestarTick = 500;
const uint16_t kBluestarHdrMark = 3400;
const uint16_t kBluestarHdrSpace = 1334;
const uint16_t kBluestarBitMark = 600;
const uint16_t kBluestarOneSpace = 1000;
const uint16_t kBluestarZeroSpace = kBluestarBitMark;
const uint16_t kBluestarMinGap = 40000;

#if SEND_BLUESTAR
/// Send a Bluestar Toilet formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
void IRsend::sendBluestar(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kBluestarHdrMark, kBluestarHdrSpace,
              kBluestarBitMark, kBluestarOneSpace,
              kBluestarBitMark, kBluestarZeroSpace,
              kBluestarBitMark, kBluestarMinGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}
#endif  // SEND_BLUESTAR

#if DECODE_BLUESTAR
/// Decode the supplied Bluestar Toilet message.
/// Status: Stable / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
bool IRrecv::decodeBluestar(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kBluestarBits)
    return false;  // We expect Bluestar to be a certain sized message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kBluestarHdrMark, kBluestarHdrSpace,
                    kBluestarBitMark, kBluestarOneSpace,
                    kBluestarBitMark, kBluestarZeroSpace,
                    kBluestarBitMark, kBluestarMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = decode_type_t::BLUESTAR;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_BLUESTAR
