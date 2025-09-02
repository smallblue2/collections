project('collections', 'c',
  version : '0.0.2',
  meson_version : '>=1.6.0',
  default_options : [
    'c_std=c2x',
    'werror=true',
  ]
)

# project info
project_description = 'A library of custom collections for better developer experience in C.'
project_url = 'git@github.com:smallblue2/collections.git'

# build options
install_headers = get_option('install_headers')

sources = [
  'src/arena.c',
  'src/parray.c'
]

collections_static_lib = static_library('collections',
  sources,
  install: true
)

collections_shared_lib = library('collections',
  sources,
  install: true
)

if install_headers
  install_headers('include/collections/arena.h', subdir: 'collections')
  install_headers('include/collections/parray.h', subdir: 'collections')
endif

pkgconfig = import('pkgconfig')

pkgconfig.generate(
  name: 'collections',
  description: project_description,
  version: meson.project_version(),
  libraries: [collections_shared_lib],
  subdirs: 'collections'
)

# Tests
unity_dirs = include_directories('tests/unity')

arena_test_exe = executable('arena_test',
  'src/arena.c',
  'tests/test_arena.c',
  'tests/unity/src/unity.c',
  include_directories: [unity_dirs, '.'],
)

parray_test_exe = executable('parray_test',
  'src/parray.c',
  'tests/test_parray.c',
  'tests/unity/src/unity.c',
  include_directories: [unity_dirs, '.'],
)

test('Arena tests', arena_test_exe)
test('Parray tests', parray_test_exe)
