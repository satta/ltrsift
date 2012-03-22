name        = "Protein Domain Filter"
author      = "Sascha Kastens"
version     = "1.0"
email       = "sascha.kastens@studium.uni-hamburg.de"
short_descr = "Filters out candidates without Protein domains"
description = "Filters out a candidate if it does not contain at " ..
	      "least one node of type protein_match"
			    
function filter(gn)
  target = "protein_match"
  gfi = gt.feature_node_iterator_new(gn)

  curnode = gfi:next()

  while not(curnode == nil) do

    if (curnode:get_type() == target) then
      return false
    end
    curnode = gfi:next()
  end

  return true
end
