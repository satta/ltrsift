name        = "Full Protein Domain Set"
author      = "Sascha Steinbiss"
version     = "1.0"
email       = "steinbiss@zbh.uni-hamburg.de"
short_descr = "Filters out candidates without full protein domain set"
description = "Filters out a candidate if it does not contain at " ..
              "least one match for each RT, PR, INT and RH."

type = {}
type.RVT_1        = "RT"
type.RVT_thumb    = "RT"
type.RVP          = "PR"
type.rve          = "INT"
type.Integrase_Zn = "INT"
type.Integrase    = "INT"
type.RNase_H      = "RH"

function filter(gn)
  local present = {}
  gfi = gt.feature_node_iterator_new(gn)
  node = gfi:next()
  while not (node == nil) do
    if (node:get_type() == "protein_match") then
      domain = node:get_attribute("name")
      if not (type[domain] == nil) then
        present[type[domain]] = true
      end
    end
    node = gfi:next()
  end
  if present.RT and present.PR and present.INT and present.RH then
    return false
  end
  return true
end
