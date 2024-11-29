project (checker)

set (ENTITIES ${ENTITIES} Checker PARENT_SCOPE)

add_nk_idl (service_idl "${RESOURCES}/ServiceChecker.idl"
            NK_MODULE          "highlevel"
            LANG               "CXX")

add_nk_cdl (server_cdl   "${RESOURCES}/ServerChecker.cdl"
            NK_MODULE    "highlevel"
            LANG         "CXX")

add_dependencies (service_idl server_cdl)
bind_nk_targets (service_idl server_cdl)

add_nk_edl (Checker_edl  "${RESOURCES}/Checker.edl"
	    NK_MODULE "highlevel"
	    LANG      "CXX")

add_dependencies (Checker_edl server_cdl)
bind_nk_targets (Checker_edl server_cdl)

add_executable (Checker "src/checker.cpp")
add_dependencies (Checker Checker_edl)
bind_nk_targets (Checker Checker_edl server_cdl)
