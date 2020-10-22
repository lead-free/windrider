# yaccs user config file.

# If the compiler is not in the PATH we need to tell yaccs where to find it.
set(yaccs_compiler_paths /Applications/ARM/bin/)

# We can also adjust other options (some are configuration dependent; refer to the documentation).
# This, for example, will use march=native.
set(yaccs_DEFAULT_ARCH Generic)

# Load the configuration.
include(cmake/yaccs/cortex-m_gcc-arm_m4_cxx14.cmake)

# This is optional but useful to get a sense of what is happening.
# This command will print various information about the build configuration.
# yaccs_system_info()

# This is also optional but it helps by neatly organizing the build tree.
yaccs_init_build_tree()
