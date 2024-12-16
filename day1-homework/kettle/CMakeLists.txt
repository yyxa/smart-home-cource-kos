project (kettle)

set (ENTITIES ${ENTITIES} Kettle PARENT_SCOPE)

add_nk_idl (service_kettle_idl "${RESOURCES}/ServiceKettle.idl"
            NK_MODULE          "highlevel"
            LANG               "CXX")

add_nk_cdl (server_kettle_cdl   "${RESOURCES}/ServerKettle.cdl"
            NK_MODULE    "highlevel"
            LANG         "CXX")

add_dependencies (service_kettle_idl server_kettle_cdl)
bind_nk_targets (service_kettle_idl server_kettle_cdl)

add_nk_edl (Kettle_edl  "${RESOURCES}/Kettle.edl"
	    NK_MODULE "highlevel"
	    LANG      "CXX")

add_dependencies (Kettle_edl server_kettle_cdl service_kettle_idl)
bind_nk_targets (Kettle_edl server_kettle_cdl service_kettle_idl)

add_executable (Kettle "src/kettle.cpp")
add_dependencies (Kettle Kettle_edl)
bind_nk_targets (Kettle Kettle_edl server_kettle_cdl)
