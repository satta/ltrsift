name        = "Reference Match Filter"
author      = "Sascha Steinbiss"
version     = "1.0"
email       = "steinbiss@zbh.uni-hamburg.de"
short_descr = "Selects candidates with long reference matches"
description = "Selects a candidate if it contains a match of length " ..
              ">= 80% of total element length."

function filter(gn)
  length = 0
  gfi = gt.feature_node_iterator_new(gn)
  node = gfi:next()
  while not (node == nil) do
    if (node:get_type() == "LTR_retrotransposon") then
      range = node:get_range()
      length = range:get_end() - range:get_start() + 1
    end
    node = gfi:next()
  end
  if (length == 0) then
    return true
  end

  gfi = gt.feature_node_iterator_new(gn)
  node = gfi:next()
  while not (node == nil) do
    if (node:get_type() == "nucleotide_match") then
      mrange = node:get_range()
      t = node:get_attribute("target")
      if mrange:get_end() - mrange:get_start() + 1 > length * 0.8 then
        return false
      end
    end
    node = gfi:next()
  end
  return true
end
