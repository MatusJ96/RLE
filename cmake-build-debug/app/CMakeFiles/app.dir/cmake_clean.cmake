file(REMOVE_RECURSE
  "../bin/app.exe"
  "../bin/app.exe.manifest"
  "../bin/app.pdb"
  "../lib/libapp.dll.a"
  "CMakeFiles/app.dir/main.c.obj"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/app.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
