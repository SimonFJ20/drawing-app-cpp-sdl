project(
    'sdl-drawing-app',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++20',
    ],
)

sdl2_dep = dependency('sdl2')

sources = []

subdir('src')

exe = executable(
    'sdl_drawing_app',
    sources,
    win_subsystem: 'console',
    include_directories: [
        include_directories('src'),
    ],
    dependencies: [
        sdl2_dep,
    ],
)
