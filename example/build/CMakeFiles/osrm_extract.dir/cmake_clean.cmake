FILE(REMOVE_RECURSE
  "libosrm_extract.pdb"
  "libosrm_extract.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/osrm_extract.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
