project (einit)

# Connect the CMake library named image, which contains the solution image build scripts.
include (platform/image)
include (${KL_SDK_ROOT_PATH}/common/build-sd-image.cmake)

include (platform/nk)

find_package(kss REQUIRED)

nk_build_edl_files (klog_edl_files
                    NK_MODULE "highlevel"
                    EDL "../resources/KlogEntity.edl")

add_executable (KlogEntityHw "../klog/src/klog_entity.c")
target_link_libraries (KlogEntityHw ${klog_SYSTEM_AUDIT_LIB})
add_dependencies (KlogEntityHw klog_edl_files)

add_executable (KlogEntityQemu "../klog/src/klog_entity.c")
target_link_libraries (KlogEntityQemu ${klog_SYSTEM_AUDIT_LIB})
add_dependencies (KlogEntityQemu klog_edl_files)


# Define the ENTITIES variable with a list of programs.
# set (ENTITIES Network Checker)
set (ENTITIES Network Checker Keys Kettle KlogStorageEntity)

set (SECURITY_PSL_FILE    "src/security.psl")
set (CONNECTIONS_CFG_FILE "src/init.yaml.in")
set (QEMU_FLAGS           "-nic none")
set (QEMU_ENV             "QEMU_AUDIO_DRV=none")

set (INIT_KlogEntity_PATH "KlogEntityHw")

# Create a CMake target that can then be used
# to build the image for the hardware platform using make.
build_kos_hw_image (kos-image
        EINIT_ENTITY EinitHw
        CONNECTIONS_CFG ${CONNECTIONS_CFG_FILE}
        SECURITY_PSL ${SECURITY_PSL_FILE}
        KLOG_ENTITY KlogEntityHw
        ${ENTITIES})
target_link_libraries (KlogEntityHw kssaudit)

# Create a CMake target that can then be used
# to build the SD card image for the hardware platform using make.
build_sd_image (sd-image
        KOS_IMAGE_TARGET kos-image)

set (INIT_KlogEntity_PATH "KlogEntityQemu")

# Create a CMake target that can then be used
# to build the image for running in QEMU using make.
build_kos_qemu_image (kos-qemu-image
        EINIT_ENTITY EinitQemu
        QEMU_FLAGS ${QEMU_FLAGS}
        QEMU_ENV ${QEMU_ENV}
        CONNECTIONS_CFG ${CONNECTIONS_CFG_FILE}
        SECURITY_PSL ${SECURITY_PSL_FILE}
        KLOG_ENTITY KlogEntityQemu
        ${ENTITIES})

target_link_libraries (KlogEntityQemu kssaudit)

add_kss_pal_qemu_tests (
        PSL_FILES src/security.psl
        DEPENDS kos-qemu-image)