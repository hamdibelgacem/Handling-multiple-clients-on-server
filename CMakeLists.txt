project(audio-sfu-gen-srv)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(AUDIO_INSTALL_ARCHIVE_DIR "lib")
set(AUDIO_INSTALL_LIBRARY_DIR "lib")
set(AUDIO_INSTALL_RUNTIME_DIR "bin")

set(SRC_LIST
  src/AudioChunk.cpp
  src/SilentAudioChunk.cpp
  src/SineWaveAudioChunk.cpp
  src/pub-sub.cpp
  src/Socket.cpp
  src/split.cpp
  src/HandleConnection.cpp
  src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME}
PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${AUDIO_INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${AUDIO_INSTALL_LIBRARY_DIR}
  RUNTIME DESTINATION ${AUDIO_INSTALL_RUNTIME_DIR}
)
