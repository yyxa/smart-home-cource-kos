project (keys)

set (ENTITIES ${ENTITIES} Keys PARENT_SCOPE)

add_nk_idl (service_keys_idl "${RESOURCES}/ServiceKeys.idl"
            NK_MODULE          "highlevel"
            LANG               "CXX")

add_nk_cdl (server_keys_cdl   "${RESOURCES}/ServerKeys.cdl"
            NK_MODULE    "highlevel"
            LANG         "CXX")

add_dependencies (service_keys_idl server_keys_cdl)
bind_nk_targets (service_keys_idl server_keys_cdl)

add_nk_edl (Keys_edl  "${RESOURCES}/Keys.edl"
	    NK_MODULE "highlevel"
	    LANG      "CXX")

add_dependencies (Keys_edl server_keys_cdl service_keys_idl)
bind_nk_targets (Keys_edl server_keys_cdl service_keys_idl)

add_executable (Keys "src/keys.cpp")
add_dependencies (Keys Keys_edl)
bind_nk_targets (Keys Keys_edl server_keys_cdl)
