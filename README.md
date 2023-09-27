# Camera Relay

Firmware for the Raspberry Pi Pico acting as the PWM controller between the OBC and the MAPIR Survey 3 camera.

## Pulse Profile

1. Idle:
   - **`HIGH` 1ms**
   - `LOW` 100ms
   - `HIGH` 1ms
   - `LOW` 2000ms
2. Capture
   - **`HIGH` 2ms**
   - `LOW` 100ms
   - `HIGH` 1ms
   - `LOW` 2000ms
3. Toggle USB
   - `HIGH` 1.5ms
   - `LOW` 100ms
   - `HIGH` 1ms
   - `LOW` 2000ms
