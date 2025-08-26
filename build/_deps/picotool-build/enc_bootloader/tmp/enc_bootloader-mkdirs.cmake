# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-src/enc_bootloader")
  file(MAKE_DIRECTORY "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-src/enc_bootloader")
endif()
file(MAKE_DIRECTORY
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader"
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader"
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/tmp"
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/src"
  "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/vagner/dados/vagner_vasconcelos_embarcatech_HBr_2025/projetos/projeto-final-adriana_elias_vagner_tinyml/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
