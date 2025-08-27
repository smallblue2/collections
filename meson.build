project('allocators', 'c',
  version : '0.0.1',
  meson_version : '>=1.6.0',
  default_options : [
    'c_std=c2x',
    'werror=true',
  ]
)

# project info
project_description = 'A library of custom allocators for better heap memory management in C.'
project_url = ''

# build options
install_headers = get_option('install_headers')

sources = [
  'src/arena.c'
]

allocators_static_lib = static_library('allocators',
  sources,
  install: true
)

allocators_shared_lib = library('allocators',
  sources,
  install: true
)

if install_headers
  install_headers('include/allocators/arena.h', subdir: 'allocators')
endif

# Tests
unity_dirs = include_directories('tests/unity')
arena_test_exe = executable('arena_test',
  'src/arena.c',
  'tests/test_arena.c',
  'tests/unity/src/unity.c',
  include_directories: [unity_dirs, '.'],
)

test('Arena tests', arena_test_exe)
