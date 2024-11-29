project (einit)

# Connect the CMake library named image, which contains the solution image build scripts.
include (platform/image)
include (${KL_SDK_ROOT_PATH}/common/build-sd-image.cmake)

# Define the ENTITIES variable with a list of programs.
# set (ENTITIES Network Checker)
set (ENTITIES Network Checker Keys)

set (SECURITY_PSL_FILE    "src/security.psl")
set (CONNECTIONS_CFG_FILE "src/init.yaml.in")
set (QEMU_FLAGS           "-nic none")
set (QEMU_ENV             "QEMU_AUDIO_DRV=none")

# Create a CMake target that can then be used
# to build the image for the hardware platform using make.
build_kos_hw_image (kos-image
        EINIT_ENTITY EinitHw
        CONNECTIONS_CFG ${CONNECTIONS_CFG_FILE}
        SECURITY_PSL ${SECURITY_PSL_FILE}
        ${ENTITIES})

# Create a CMake target that can then be used
# to build the SD card image for the hardware platform using make.
build_sd_image (sd-image
         KOS_IMAGE_TARGET kos-image)

# Create a CMake target that can then be used
# to build the image for running in QEMU using make.
build_kos_qemu_image (kos-qemu-image
        EINIT_ENTITY EinitQemu
        QEMU_FLAGS ${QEMU_FLAGS}
        QEMU_ENV ${QEMU_ENV}
        CONNECTIONS_CFG ${CONNECTIONS_CFG_FILE}
        SECURITY_PSL ${SECURITY_PSL_FILE}
        ${ENTITIES})

