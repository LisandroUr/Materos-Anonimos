cmake_minimum_required(VERSION 3.10)
project(SwissEphemerisExample)

# Permitir que el usuario especifique dónde está Swiss Ephemeris
set(SWE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/swisseph_src/src" CACHE PATH "Directorio de include de Swiss Ephemeris")
set(SWE_LIB_DIR "${CMAKE_SOURCE_DIR}/swisseph_src/src" CACHE PATH "Directorio de librería de Swiss Ephemeris")

# Opción para indicar sistema de casas (Plácido por defecto)
set(HOUSE_SYSTEM "P" CACHE STRING "Sistema de casas para Swiss Ephemeris (P=Placido, K=Koch, R=Regiomontanus, ...)")
add_definitions(-DSYSTEM_CASA=${HOUSE_SYSTEM})

# Añadir ejecutable; asume que main.cpp está en la raíz del proyecto
add_executable(mi_programa main.cpp)

# Indicamos rutas de include y linkeo
target_include_directories(mi_programa PRIVATE ${SWE_INCLUDE_DIR})
target_link_directories(mi_programa PRIVATE ${SWE_LIB_DIR})

# Enlazar con la librería de Swiss Ephemeris; ajusta el nombre si varía
find_library(SWE_LIB NAMES swe sswlib PATHS ${SWE_LIB_DIR} NO_DEFAULT_PATH)
if (SWE_LIB)
    message(STATUS "Encontrada librería Swiss Ephemeris: ${SWE_LIB}")
    target_link_libraries(mi_programa PRIVATE ${SWE_LIB})
else()
    message(FATAL_ERROR "No se pudo encontrar la librería Swiss Ephemeris en ${SWE_LIB_DIR}")
endif()

# Copiar archivos de efemérides a directorio de build (opcional, para que se encuentren en tiempo de ejecución)
add_custom_command(TARGET mi_programa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/swisseph_src/ephe"
    "$<TARGET_FILE_DIR:mi_programa>/ephe"
)
