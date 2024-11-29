project (network)

set (ENTITIES ${ENTITIES} Network PARENT_SCOPE)

add_nk_idl (service_client_idl "${RESOURCES}/ServiceChecker.idl"
            NK_MODULE          "highlevel"
            LANG               "CXX")

add_nk_edl (network_edl      "${RESOURCES}/Network.edl"
            NK_MODULE   "highlevel"
            LANG        "CXX")

add_executable (Network "src/network.cpp")
add_dependencies (Network network_edl service_client_idl)

bind_nk_targets (Network network_edl service_client_idl)
