FILE(REMOVE_RECURSE
  "libosrm_contract.pdb"
  "libosrm_contract.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/osrm_contract.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
