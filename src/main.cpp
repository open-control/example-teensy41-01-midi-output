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
 *
 * Hardware required:
 * - Teensy 4.1 (or 4.0)
 * - USB connection to computer
 *
 * Test it:
 * - Open a MIDI monitor (MIDI-OX, Pocket MIDI, etc.)
 * - You should see CC 1 incrementing every 100ms on channel 1
 */

#include <Arduino.h>
#include <oc/teensy/UsbMidi.hpp>

// ═══════════════════════════════════════════════════════════════════════════
// Configuration
// ═══════════════════════════════════════════════════════════════════════════

constexpr uint8_t MIDI_CHANNEL = 0;  // Channel 1 (0-indexed)
constexpr uint8_t CC_NUMBER = 1;     // Modulation wheel
constexpr uint32_t SEND_INTERVAL_MS = 100;

// ═══════════════════════════════════════════════════════════════════════════
// Global MIDI instance
// ═══════════════════════════════════════════════════════════════════════════

oc::teensy::UsbMidi midi;

// ═══════════════════════════════════════════════════════════════════════════
// Arduino Entry Points
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
    // Wait for serial (optional, for debug output)
    Serial.begin(115200);
    while (!Serial && millis() < 2000) {}

    Serial.println("\n[Example 01] MIDI Output");
    Serial.println("========================\n");

    // Initialize MIDI
    auto result = midi.init();
    if (!result) {
        Serial.printf("[ERROR] MIDI init failed: %s\n",
                      oc::core::errorCodeToString(result.error().code));
        while (true) { delay(1000); }
    }

    Serial.println("[OK] MIDI initialized");
    Serial.printf("Sending CC %d on channel %d every %dms\n\n",
                  CC_NUMBER, MIDI_CHANNEL + 1, SEND_INTERVAL_MS);
}

void loop() {
    static uint32_t lastSend = 0;
    static uint8_t value = 0;

    uint32_t now = millis();
    if (now - lastSend >= SEND_INTERVAL_MS) {
        lastSend = now;

        // Send CC message
        midi.sendCC(MIDI_CHANNEL, CC_NUMBER, value);
        Serial.printf("CC %d = %d\n", CC_NUMBER, value);

        // Increment value (wraps at 127)
        value = (value + 1) % 128;
    }

    // Process incoming MIDI (good practice even if not used)
    midi.update();
}
