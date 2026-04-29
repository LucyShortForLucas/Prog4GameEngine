project(TronGame)
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC LucyEngine)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

if(WIN32)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_DLL_PATH}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_TTF_DLL_PATH}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    COMMENT "Copying DLLs to output directory..."
    VERBATIM
  )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Data")