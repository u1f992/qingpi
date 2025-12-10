# NX Automation Meta Firmware Library

Meta firmware for NX automation.

## Overview

With the Nintendo Switch (codename: NX) now equipped with the widely-used USB interface, several automated operation solutions using MCU have come to light. Presently, the prevalent method involves two wiring paths: `PC->MCU->NX` and `NX->HDMI Video Capture->PC`. Through these, video feedback is obtained, processed on the PC side, and then operations are transmitted accordingly.

This library primarily focuses on the MCU aspect mentioned above. Regardless of the microcontroller or protocol used, its firmware would be abstracted to "receive byte arrays through communication, convert it into a data structure representing the next state of the gamepad, and reflect it to the NX."

## Usage

What this library offers is only an interface to represent the communication protocol, a structure to represent the state of the gamepad, and a buffer structure to accept/reject each packet according to the given protocol and extract the state. Users must combine these to define a specific communication protocol and create functions to reflect the extracted state to the NX.

By implementing each method of `NxamfBytesProtocolInterface`, a specific communication protocol is defined. Implementations for the [NX Macro Controller v2](https://blog.bzl-web.com/entry/2020/01/20/165719) and [Poke-Controller Modified](https://github.com/Moi-poke/Poke-Controller-Modified) are included as examples.

`NxamfBytesBuffer` holds a protocol as a member and provides `append` function for packets. The protocol determines acceptance or rejection. If ready, it provides a pointer to NxamfGamepadState (note that the state must be freed using delete), otherwise it returns NULL. You can also explicitly reset the buffer using `clear`.

Below is a conceptual class diagram in a Java-like notation.

```mermaid
classDiagram
  class NxamfBytesProtocolInterface
  <<interface>>NxamfBytesProtocolInterface
  NxamfBytesProtocolInterface : +is_acceptable(uint8_t packet, uint8_t buffer[], size_t length) bool
  NxamfBytesProtocolInterface : +is_ready(uint8_t buffer[], size_t length) bool
  NxamfBytesProtocolInterface : +convert(uint8_t buffer[], size_t length, NxamfGamepadState *state)

  class PokeConProtocol
  PokeConProtocol ..|> NxamfBytesProtocolInterface

  class Nxmc2Protocol
  Nxmc2Protocol ..|> NxamfBytesProtocolInterface

  class NxamfProtocolMultiplexer
  NxamfProtocolMultiplexer : -NxamfBytesProtocolInterface protocols[]
  NxamfProtocolMultiplexer : +size_t ready_index

  NxamfProtocolMultiplexer ..|> NxamfBytesProtocolInterface
  NxamfBytesProtocolInterface --* NxamfProtocolMultiplexer

  class NxamfBytesBuffer
  NxamfBytesBuffer : -NxamfBytesProtocolInterface *protocol
  NxamfBytesBuffer : +append(uint8_t packet) NxamfGamepadState *
  NxamfBytesBuffer : +clear()

  NxamfBytesBuffer ..> NxamfBytesProtocolInterface
```

## Dependency

- [kokke/tiny-regex-c](https://github.com/kokke/tiny-regex-c) included (public domain).
