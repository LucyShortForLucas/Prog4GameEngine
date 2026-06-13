project(TronGame)
set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME} main.cpp
components/src/FpsTracker.cpp
 "commands/src/MoveTank.cpp" "components/TronGameSetup.h" "components/src/TronGameSetup.cpp" "commands/MoveTurret.h" "commands/src/MoveTurret.cpp" "misc/src/BuildLevel.cpp" "components/Turret.h" "components/src/Turret.cpp" "commands/ShootPlayerBullet.h" "commands/src/ShootPlayerBullet.cpp" "components/PlayerBulletBounce.h" "components/src/PlayerBulletBounce.cpp" "components/src/RepoolAfterNSeconds.cpp" "misc/PathfindGraph.h" "misc/src/PathFindGraph.cpp" "components/Pathfinding.h" "components/src/Pathfinding.cpp" "components/TeleportToRand2x2.h" "components/src/TeleportToRand2x2.cpp" "components/EnemyController.h" "components/src/EnemyController.cpp" "components/EnemyBulletCollisions.h" "components/src/EnemyBulletCollision.cpp" "components/PlayerLives.h" "components/src/PlayerLives.cpp" "components/GameSceneSelector.h" "commands/ClearAndLoad.h" "commands/src/ClearAndLoad.cpp" "components/src/GameData.cpp" "commands/NextLevel.h")

target_include_directories(${PROJECT_NAME} PUBLIC
   components commands misc
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