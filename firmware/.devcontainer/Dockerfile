FROM ubuntu:22.04

ARG USERNAME=pi
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

# Setup prerequisites
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    git \
    pkg-config \
    python3 \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

# https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
# P.6
RUN cd /home/pi/ \
    && mkdir pico \
    && cd pico \
    \
    && git clone https://github.com/raspberrypi/pico-sdk.git --branch 1.5.1 --single-branch --depth=1 \
    && cd pico-sdk \
    && git submodule update --init --recommend-shallow --depth=1 \
    && cd .. \
    && git clone https://github.com/raspberrypi/pico-examples.git --branch master --single-branch --depth=1

# P.7
RUN apt-get update \
    && apt-get install -y --no-install-recommends cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

# P.16
RUN apt-get update \
    && apt-get install -y --no-install-recommends minicom \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

# P.19
RUN cd /home/pi/ \
    && apt-get update \
    && apt-get install -y --no-install-recommends automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0.0-dev \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/* \
    && git clone https://github.com/raspberrypi/openocd.git --branch rp2040 --recursive --depth=1 \
    && cd openocd \
    && ./bootstrap \
    && ./configure --enable-ftdi --enable-sysfsgpio --enable-bcm2835gpio \
    && make -j4 \
    && make install

# P.22
RUN apt-get update \
    && apt-get install -y --no-install-recommends gdb-multiarch \
    && apt-get -y clean \
    && rm -rf /var/lib/apt/lists/*

ENV PICO_SDK_PATH="/home/pi/pico/pico-sdk"

# P.69
RUN cd /home/pi/ \
    && git clone https://github.com/raspberrypi/picotool.git --branch master --single-branch --depth=1 \
    && cd picotool \
    && mkdir build \
    && cd build \
    && cmake ../ \
    && make \
    && make install

RUN chown -R $USERNAME /home/pi/

# TODO: Using a non-root user, you will not be able to find Picoprobe.
# USER $USERNAME