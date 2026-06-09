project(TronGame)
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.cpp
components/src/FpsTracker.cpp
 "commands/MovePhysicsBody.h" "commands/src/MovePhysicsBody.cpp" "components/TronGameSetup.h" "components/src/TronGameSetup.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
   components commands
)

target_link_libraries(${TARGET_NAME} PUBLIC LucyEngine)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

if(WIN32)
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
  set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

  add_custom_command(
  TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_DLL_PATH}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_TTF_DLL_PATH}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3_mixer::SDL3_mixer> "$<TARGET_FILE_DIR:${TARGET_NAME}>"
  COMMENT "Copying SDL3 DLLs (core, ttf, mixer) to output directory..."
  VERBATIM
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Data")