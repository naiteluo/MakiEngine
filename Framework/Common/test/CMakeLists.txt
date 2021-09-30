me_add_test(Test_Load_Asset test_asset_loader.cpp)
target_link_libraries(Test_Load_Asset Common)

me_add_test(Test_Gex_Parser test_gex_parser.cpp)
target_link_libraries(Test_Gex_Parser Common ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY} ${XG_LIBRARY})

me_add_test(Test_Scene test_asset_loader_scene.cpp)
target_link_libraries(Test_Scene Common ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY} ${XG_LIBRARY})