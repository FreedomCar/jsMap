FILE(REMOVE_RECURSE
  "libosrm_store.pdb"
  "libosrm_store.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/osrm_store.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
