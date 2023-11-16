# firmware

## Basic idea

Note that this is not necessarily accurate as a diagram (for starters, there are no classes in the C language).

The figure means that the ncm core library is written in a way that is independent of the Arduino and the various ICs.

[View in Mermaid Live Editor](https://mermaid.live/edit#pako:eNqdVltv2jAU_iuRn7qJMkjZoKiqxMrU8tAVLZU2bZmQFx_AWmJHjl1auv73OTeIE5NewgPJ-b7jcz_JIwo4ATRGQYiTZErxSuDIZwxHkMQ4AIcFkfPoM0dfGcX5GkSXwEDgcK5EzBOY3cyYBLFM2QUzvc7OaCk-P9-LE5CLkG-O3jl3nBJTvqaLrQE81QxfKCGAyenkgrM7EPr4V5mm7O8R4epPCM4dDhU0fOBKBHCQUvdmSldU6jRwqX2iPILX-qND3tAYxEKnkeoTWGm6fK47GPONZmtew_O1koRvWGv2vLl3622oDNav8dJijS-XdUM1U5-VlJxVT29tm1VM-Z665iFp2BQQAk6gNcArLGsmCz9UbJMKulpLG5Bm0iYPYSlrbuZWp1RAkBbMIeXdW_z3QkpgjkktiLauT59pgMMXK6y5oFvOZFUli6RovfuOU9w9tIfg2GO45SpYe4EAqJe_fVysgbSr2EI50ObJ5u3RGvXKWn3fEd3ueWtnZ9y0LQtirpZJd8UuoJaiZfxqYguV1vQcUrKkJ41pv_EngijKuLn1L-ezG3NZxAkowqubIqZskdAtLGR6a8iv9UvmyMQ7jl43TqSBeuZJHtZ3QaVdqX0zX2FBNljAzL2wib35rCjjPmRMRoPevRnxZKplPyzU1ajXr1MvtaxBJclgcOKa1KmXyhrUCFPWTPiE4FiXqKlvAXJ3DwK3gsYhWOHUdwtwrV36xpWkDH75SL-rVPwh5Dz20e_C_YqLzvvj48xlU3p8_O-5-fBZHlLWnkbpjFAzA2XyTKQ00jpA6S_PhGEoawYjdZmhsvYmUhp6ZuwsCbccauIvPDpVTsuVxZCn26hfYSfvRhMpLRwY_0q1d4fvmqIO1pqwDtda0Q7XGrJJMtqySagsUwukV65VXi5dK1hZlqiDIhB6Kon-NM7m0kdyDRH4aKxvCSyxCqWP9CRoKlaSew8sQOMlDhPoIBUTLKH4mt5JY8x-cq6fpVA70hdCJRdVDho_ons07nXQAxq77ml3eDL6OBz2R6ej_qk7fOqgbXZKv9vLL9d1R_1PJ8NBB0F22nXxSZ_-Pf0HS2njBA)

```mermaid
classDiagram
namespace ncm {
    class NcmGeneralPurposeIOInterface {
        <<interface>>
        set_low() void
        set_hi_z() void
    }
    class NcmCurrentDAConverterInterface {
        <<interface>>
        sink(double value) void
        source(double value) void
    }
    class NcmDigitalPotentiometerInterface {
        <<interface>>
        set_wiper_position(double position) void
        power_on() void
        shutdown() void
    }
    class NcmSPSTSwitchInterface {
        <<interface>>
        on() void
        off() void
    }

    class NcmButton {
        NcmGeneralPurposeIOInterface gpio
        hold() void
        release() void
    }
    class NcmHat {
        NcmButton up
        NcmButton right
        NcmButton down
        NcmButton left
        hold(NcmHatDirection direction) void
        release() void
    }
    class NcmSlidePad {
        NcmCurrentDAConverterInterface vertical
        NcmCurrentDAConverterInterface horizontal
        hold(double x, double y) void
        release() void 
    }
    class NcmTouchScreen {
        NcmDigitalPotentiometerInterface vertical
        NcmDigitalPotentiometerInterface horizontal
        NcmSPSTSwitchInterface sw
        hold(double x, double y) void
        release()void
    }
}

NcmButton ..> NcmGeneralPurposeIOInterface
NcmHat ..> NcmButton
NcmSlidePad ..> NcmCurrentDAConverterInterface
NcmTouchScreen ..> NcmDigitalPotentiometerInterface
NcmTouchScreen ..> NcmSPSTSwitchInterface

namespace Arduino {
    class GPIO {
        <<pseudo>>
        pin_size_t pin
        pinMode(pin_size_t pin, int mode) void
        digitalWrite(pin_size_t pin, int value) void
    }
    class HardwareI2C
    class HardwareSPI
}

namespace ad840x {
    class AD840X
}

namespace adg801 {
    class ADG801
}

namespace ds4432 {
    class DS4432
}

namespace main {
    class GPIOAdapter
    class DS4432Adapter
    class AD840XAdapter
    class AD840XTripleAdapter
    class ADG801Adapter
    class MainRoutine["setup/loop"]
}

GPIOAdapter *-- GPIO
GPIOAdapter --|> NcmGeneralPurposeIOInterface

DS4432 ..> HardwareI2C
DS4432Adapter *-- DS4432
DS4432Adapter --|> NcmCurrentDAConverterInterface


AD840X ..> HardwareSPI
AD840XAdapter *-- AD840X
AD840XAdapter --|> NcmDigitalPotentiometerInterface
AD840XTripleAdapter *-- AD840X
AD840XTripleAdapter --|> NcmDigitalPotentiometerInterface

ADG801 ..> GPIO
ADG801Adapter *-- ADG801
ADG801Adapter --|> NcmSPSTSwitchInterface

MainRoutine ..> GPIOAdapter
MainRoutine ..> DS4432Adapter
MainRoutine ..> AD840XAdapter
MainRoutine ..> AD840XTripleAdapter
MainRoutine ..> ADG801Adapter

MainRoutine ..> NcmButton
MainRoutine ..> NcmHat
MainRoutine ..> NcmSlidePad
MainRoutine ..> NcmTouchScreen
```