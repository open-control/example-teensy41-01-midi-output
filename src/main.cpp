/**
 * @file main.cpp
 * @brief Example 01: MIDI Output - The Absolute Minimum
 *
 * This example demonstrates the simplest possible use of Open Control:
 * sending MIDI CC messages using the UsbMidi HAL driver.
 *
 * What you'll learn:
 * - How to create and initialize a UsbMidi instance
 * - How to send MIDI CC messages
 * - Basic error handling with Result<void>
 * - Using the OC_LOG_* API for debug output
 *
 * Hardware required:
 * - Teensy 4.1 (or 4.0)
 * - USB connection to computer
 *
 * Test it:
 * - Open a MIDI monitor (MIDI-OX, Pocket MIDI, etc.)
 * - You should see CC 1 incrementing every 100ms on channel 1
 *
 * NOTE: Enable -D OC_LOG in platformio.ini build_flags to see debug output.
 *       Remove it for production (zero overhead, instant boot).
 */

#include <oc/hal/teensy/Teensy.hpp>
#include <oc/hal/teensy/UsbMidi.hpp>

// ═══════════════════════════════════════════════════════════════════════════
// Configuration - Adapt to your needs
// ═══════════════════════════════════════════════════════════════════════════

constexpr uint8_t MIDI_CHANNEL = 0;       // Channel 1 (0-indexed)
constexpr uint8_t CC_NUMBER = 1;          // Modulation wheel
constexpr uint32_t SEND_INTERVAL_MS = 100;

// ═══════════════════════════════════════════════════════════════════════════
// Global MIDI instance
// ═══════════════════════════════════════════════════════════════════════════

oc::hal::teensy::UsbMidi midi;

// ═══════════════════════════════════════════════════════════════════════════
// Arduino Entry Points
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    OC_LOG_INFO("Example 01: MIDI Output");

    if (auto r = midi.init(); !r) {
        OC_LOG_ERROR("MIDI: {}", oc::errorCodeToString(r.error().code));
        while (true) {}
    }

    OC_LOG_INFO("Ready - CC {} every {}ms", CC_NUMBER, SEND_INTERVAL_MS);
}

void loop() {
    static uint32_t lastSend = 0;
    static uint8_t value = 0;

    uint32_t now = millis();
    if (now - lastSend >= SEND_INTERVAL_MS) {
        lastSend = now;

        // Send CC message
        midi.sendCC(MIDI_CHANNEL, CC_NUMBER, value);
        OC_LOG_DEBUG("CC {} = {}", CC_NUMBER, value);

        // Increment value (wraps at 127)
        value = (value + 1) % 128;
    }

    // Process incoming MIDI (good practice even if not used)
    midi.update();
}
